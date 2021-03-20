import pickle

from wcctrl.core.scheduler import *
from wcctrl.config.system import *
from wcctrl.config.user import *
from wcctrl.component.rtc.ds1302 import *
from wcctrl.component.led.ws2812b import *
from wcctrl.component.sensor.brightness.bh1750 import *
from wcctrl.component.sensor.motion.rcwl0516 import *

user_config = UserConfig()
shared_memory = "/dev/shm/wc_shm"


def write_shm():
    with open(shared_memory, "wb") as f:
        cfg = user_config.get_config()
        f.write(pickle.dumps(cfg))


def init_shm():
    if not os.path.isfile(shared_memory):
        write_shm()


if __name__ == '__main__':

    # Init shared memory
    init_shm()

    led_strip = LEDStrip(SystemConfig.LED_COUNT, SystemConfig.LED_PIN, SystemConfig.LED_FREQ_HZ,
                         SystemConfig.LED_DMA, SystemConfig.LED_INVERT, SystemConfig.LED_BRIGHTNESS,
                         SystemConfig.LED_CHANNEL)

    led_strip.begin()
    rtc = DS1302()
    brightness_sensor = BH1750()
    motion_sensor = RCWL0516(24)
    wordclock = WordClock(user_config, led_strip)
    scheduler = WordClockScheduler(led_strip, wordclock, rtc, motion_sensor, brightness_sensor, user_config)

    try:
        # Run Wordclock Controller
        scheduler.run()

    except KeyboardInterrupt:
        print("WordClock has been stopped...!")
        print("Turn OFF all LEDs...")
        led_strip.turn_off_all_leds()
        led_strip.show()
