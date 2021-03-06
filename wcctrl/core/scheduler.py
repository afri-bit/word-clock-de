import os
import time
import datetime
import pickle
import logging
import ntplib

from wcctrl.core.wordclock import WordClock
from wcctrl.component.led.ws2812b import LEDStrip
from wcctrl.component.rtc.ds1302 import DS1302
from wcctrl.component.sensor.motion.rcwl0516 import RCWL0516
from wcctrl.component.sensor.brightness.bh1750 import BH1750
from wcctrl.config.user import UserConfig


class WordClockScheduler:
    """
    Thread class with a stop() method. The thread itself has to check
    regularly for the stopped() condition.
    """

    def __init__(self, led_strip: LEDStrip, wordclock: WordClock, rtc: DS1302, motion: RCWL0516, brightness: BH1750,
                 user_config: UserConfig):
        self.__running = True

        self.__led_strip = led_strip
        self.__wordclock = wordclock
        self.__rtc = rtc
        self.__motion = motion
        self.__brightness = brightness
        self.__user_config = user_config

        self.__step_time = 0.1  # Unit in ms

        self.__brightness_auto_prev = self.__user_config.get_brightness().auto
        self.__brightness_interval = 2  # Unit in second
        self.__brightness_counter_max = int(self.__brightness_interval / self.__step_time)
        self.__brightness_counter = 0

        self.__light_always_on_prev = self.__user_config.get_light().always_on
        self.__light_timeout_prev = self.__user_config.get_light().timeout
        self.__light_timeout_ctr = 0

        # Initialize the logging format for monitoring, including timestamps
        logging.basicConfig(
            format='%(asctime)s %(levelname)-8s %(message)s',
            level=logging.INFO,
            datefmt='%Y-%m-%d %H:%M:%S')

        self.__ntp_client = ntplib.NTPClient()

        # RTC will be updated every 10 minutes from the ntp server
        self.__update_time_interval = 600  # Unit in second
        self.__update_time_counter_max = int(self.__update_time_interval / self.__step_time)
        self.__update_time_counter = 0

    def run(self):

        # Update the time when the application starts
        self.__update_rtc()

        # While Loop in separate Thread
        while self.__running:
            time_start = datetime.datetime.now()

            # Update config from shm
            shared_memory = "/dev/shm/wc_shm"
            if os.path.getsize(shared_memory) > 0:
                with open(shared_memory, "rb") as f:
                    self.__user_config.set_config(pickle.loads(f.read()))

            if self.__update_time_counter >= self.__update_time_counter_max:
                self.__update_rtc()
                self.__update_time_counter = 0  # Reset the counter to zero
            else:
                self.__update_time_counter += 1

            self.__process_light()

            self.__process_brightness()

            # Get the time from RTC
            dt = self.__rtc.read_datetime()

            # Set the wordclock
            self.__wordclock.set_time(dt.hour, dt.minute, dt.second)

            # Calculate the time needed to process task above
            time_end = datetime.datetime.now()
            time_delta = time_end - time_start
            time_delta_sec = time_delta.total_seconds()
            # print("Task takes: " + str(time_delta_sec))

            # Calculate the difference to the time step
            if time_delta_sec < self.__step_time:
                # Sleep the rest of the time
                time.sleep(self.__step_time - time_delta_sec)
            else:
                logging.warning("Tasks takes too long: "  + str(time_delta_sec))

    def __process_light(self):
        light_config = self.__user_config.get_light()

        # This section is to check the previous state of the always_on flag
        # If there is a state change then from always_on to timeout then reset counter
        # Falling Edge
        if self.__light_always_on_prev and not light_config.always_on:
            self.__light_timeout_ctr = 0

        self.__light_always_on_prev = light_config.always_on

        light_timeout = light_config.timeout  # Unit in second
        light_timeout_ctr = int(light_timeout / self.__step_time)

        if self.__light_timeout_prev != light_timeout:
            self.__light_timeout_ctr = 0

        self.__light_timeout_prev = light_timeout

        if light_config.always_on:  # Always ON state
            self.__wordclock.show_clock(True)
        elif light_config.timeout == 0:  # Always OFF state
            self.__wordclock.show_clock(False)
        else:  # Based on motion detection state
            if self.__motion.motion_detected():  # Motion detected
                self.__wordclock.show_clock(True)
                self.__light_timeout_ctr = 0
            else:  # No motion detected
                self.__light_timeout_ctr += 1

                if self.__light_timeout_ctr >= light_timeout_ctr:
                    self.__light_timeout_ctr = 0
                    self.__wordclock.show_clock(False)

    def __process_brightness(self):
        brightness_config = self.__user_config.get_brightness()

        if self.__brightness_auto_prev and not brightness_config.auto:
            self.__brightness_counter = 0

        self.__brightness_auto_prev = brightness_config.auto

        if brightness_config.auto:  # Auto Brightness
            self.__brightness_counter += 1
            if self.__brightness_counter >= self.__brightness_counter_max:
                lux = self.__brightness.read_light()
                max_brightness = brightness_config.max

                led_brightness = int()

                if lux >= 150:  # 100 %
                    led_brightness = int(1 * max_brightness)
                elif lux >= 130:  # 90 %
                    led_brightness = int(0.9 * max_brightness)
                elif lux >= 110:  # 80 %
                    led_brightness = int(0.8 * max_brightness)
                elif lux >= 80:  # 70 %
                    led_brightness = int(0.7 * max_brightness)
                elif lux >= 60:  # 60 %
                    led_brightness = int(0.6 * max_brightness)
                elif lux >= 40:  # 50 %
                    led_brightness = int(0.5 * max_brightness)
                elif lux >= 20:  # 40 %
                    led_brightness = int(0.4 * max_brightness)
                elif lux >= 10:  # 30 %
                    led_brightness = int(0.3 * max_brightness)
                elif lux >= 5:  # 20 %
                    led_brightness = int(0.2 * max_brightness)
                elif lux >= 0:  # 10 %
                    led_brightness = int(0.1 * max_brightness)

                self.__brightness_counter = 0

                self.__led_strip.setBrightness(led_brightness)

        else:  # Manual configuration of Brightness
            brightness_value = int(brightness_config.value / 100 * brightness_config.max)
            self.__led_strip.setBrightness(brightness_value)

    def __update_rtc(self):
        try:
            response = self.__ntp_client.request('0.de.pool.ntp.org', version=3)
            dt = datetime.datetime.fromtimestamp(response.tx_time)
            self.__rtc.write_datetime(dt)
        except:
            logging.warning("Unable to update Real Time Clock from NTP server")
