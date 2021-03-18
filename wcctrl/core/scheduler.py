import time
import threading
import datetime

from wcctrl.core.wordclock import WordClock
from wcctrl.component.led.ws2812b import LEDStrip
from wcctrl.component.rtc.ds1302 import DS1302
from wcctrl.component.sensor.motion.rcwl0516 import RCWL0516
from wcctrl.component.sensor.brightness.bh1750 import BH1750
from wcctrl.config.user import UserConfig


class WordClockScheduler(threading.Thread):
    """
    Thread class with a stop() method. The thread itself has to check
    regularly for the stopped() condition.
    """

    def __init__(self, led_strip: LEDStrip, wordclock: WordClock, rtc: DS1302, motion: RCWL0516, brightness: BH1750,
                 user_config: UserConfig):
        super(WordClockScheduler, self).__init__()
        self.__stop_event = threading.Event()

        self.__led_strip = led_strip
        self.__wordclock = wordclock
        self.__rtc = rtc
        self.__motion = motion
        self.__brightness = brightness
        self.__user_config = user_config

        self.__step_time = 0.1  # Unit in ms

        self.__brightness_auto_prev = self.__user_config.get_brightness().auto
        self.__brightness_interval = 1  # Unit in second
        self.__brightness_counter_max = int(self.__brigthness_interval / self.__step_time)
        self.__brightness_counter = 0

        self.__light_always_on_prev = self.__user_config.get_light().always_on
        self.__light_timeout_prev = self.__user_config.get_light().timeout
        self.__light_timeout_ctr = 0

    def stop(self):
        self.__stop_event.set()

    def join(self, *args, **kwargs):
        self.stop()
        super(WordClockScheduler, self).join(*args, **kwargs)

    def run(self):

        # While Loop in separate Thread
        while not self.__stop_event.is_set():
            time_start = datetime.datetime.now()

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
            print("Task takes: " + str(time_delta_sec))

            # Calculate the difference to the time step
            if time_delta_sec < self.__step_time:
                # Sleep the rest of the time
                time.sleep(self.__step_time - time_delta_sec)
            else:
                print("Tasks takes too long :" + str(time_delta_sec))

        print("WordClock Scheduler stopped...!")
        print("Turn Off All LEDs...")
        self.__led_strip.turn_off_all_leds()

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

        if light_config.always_on:  # Always ON state
            self.__wordclock.show_clock(True)
        else:
            if light_config.timeout == 0:  # Always OFF state
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
                lux = self.__brightness.get_result()
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

        pass
