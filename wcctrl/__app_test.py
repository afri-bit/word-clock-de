import os
import time
import datetime
import smbus

from wcctrl.core.scheduler import *
from wcctrl.config.system import *
from wcctrl.config.user import *
from wcctrl.component.rtc.ds1302 import *
from wcctrl.component.led.ws2812b import *
from wcctrl.component.sensor.brightness.bh1750 import *
from wcctrl.component.sensor.motion.rcwl0516 import *

if __name__ == "__main__":
    led_strip = LEDStrip(SystemConfig.LED_COUNT, SystemConfig.LED_PIN, SystemConfig.LED_FREQ_HZ,
                     SystemConfig.LED_DMA, SystemConfig.LED_INVERT, SystemConfig.LED_BRIGHTNESS,
                     SystemConfig.LED_CHANNEL)
    led_strip.begin()
    user_config = UserConfig()
    rtc = DS1302()
    rtc.write_datetime(datetime.datetime.now())
    brightness_sensor = BH1750()
    motion_sensor = RCWL0516(24)
    wc = WordClock(user_config, led_strip)
    scheduler = WordClockScheduler(led_strip, wc, rtc, motion_sensor, brightness_sensor, user_config)

    try:
        scheduler.start()

        while True:
            time.sleep(0)

    except KeyboardInterrupt:
        scheduler.join()

