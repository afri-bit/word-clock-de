import os
import pickle

from wcctrl.core.scheduler import WordClockScheduler, WordClock
from wcctrl.config.system import SystemConfig
from wcctrl.config.user import UserConfig
from wcctrl.component.rtc.ds1302 import DS1302
from wcctrl.component.led.ws2812b import LEDStrip
from wcctrl.component.sensor.brightness.bh1750 import BH1750
from wcctrl.component.sensor.motion.rcwl0516 import RCWL0516
from wcctrl.utils.logger import get_logger

logger = get_logger(__name__)

user_config = UserConfig()


def write_shm() -> None:
    """
    Write user config to shared memory
    """
    try:
        with open(SystemConfig.SHARED_MEMORY, "wb") as f:
            cfg = user_config.get_config()
            f.write(pickle.dumps(cfg))
        logger.info("Shared memory written successfully.")
    except Exception as e:
        logger.error(f"Failed to write shared memory: {e}")


def init_shm() -> None:
    """
    Initialize shared memory
    """
    if not os.path.isfile(SystemConfig.SHARED_MEMORY):
        write_shm()
    else:
        logger.info("Shared memory already initialized.")


def main():
    """
    Main function to run the WordClock
    """

    logger.info("Starting WordClock...")

    # Init shared memory
    init_shm()

    try:
        led_strip = LEDStrip(
            SystemConfig.LED_COUNT,
            SystemConfig.LED_PIN,
            SystemConfig.LED_FREQ_HZ,
            SystemConfig.LED_DMA,
            SystemConfig.LED_INVERT,
            SystemConfig.LED_BRIGHTNESS,
            SystemConfig.LED_CHANNEL,
        )

        # Initialize the LED strip
        led_strip.begin()
        logger.info("LED strip initialized.")

        # Initialize the DS1302 RTC
        rtc = DS1302()
        logger.info("RTC initialized.")

        # Initialize the sensors
        brightness_sensor = BH1750()
        logger.info("Brightness sensor initialized.")

        # Initialize the motion sensor
        motion_sensor = RCWL0516(24)
        logger.info("Motion sensor initialized.")

        # Initialize the WordClock
        wordclock = WordClock(user_config, led_strip)
        logger.info("WordClock instanced initialized.")

        # Initialize the WordClock Scheduler
        scheduler = WordClockScheduler(led_strip,
                                       wordclock,
                                       rtc,
                                       motion_sensor,
                                       brightness_sensor,
                                       user_config
                                       )
        logger.info("Scheduler initialized.")

        # Run Wordclock Controller
        scheduler.run()
    except KeyboardInterrupt:
        logger.info("WordClock has been stopped by user.")
    except Exception as e:
        logger.error(f"An unexpected error occurred: {e}")
    finally:
        logger.info("Turn OFF all LEDs...")
        led_strip.turn_off_all_leds()
        led_strip.show()
        logger.info("All LEDs turned off.")

    logger.info("WordClock has been stopped...!")

if __name__ == "__main__":
    main()
