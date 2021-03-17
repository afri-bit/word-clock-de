import os
import time
import datetime

from wcctrl.core.wordclock import *
from wcctrl.config.system import *
from wcctrl.config.user import *
from wcctrl.component.led.ws2812b import *

if __name__ == "__main__":
    strip = LEDStrip(SystemConfig.LED_COUNT, SystemConfig.LED_PIN, SystemConfig.LED_FREQ_HZ,
                     SystemConfig.LED_DMA, SystemConfig.LED_INVERT, SystemConfig.LED_BRIGHTNESS,
                     SystemConfig.LED_CHANNEL)
    strip.begin()
    usr_cfg = UserConfig()
    wc = WordClock(user_cfg=usr_cfg, led_strip=strip)
    wc.show_clock(True)

    try:
        while True:
            dt = datetime.datetime.now()
            wc.set_time(dt.hour, dt.minute, dt.second)
            time.sleep(0.1)

    except KeyboardInterrupt:
        strip.turn_off_all_leds()
        strip.show()

    pass

