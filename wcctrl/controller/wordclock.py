import time
from neopixel import *


class WordClock(Adafruit_NeoPixel):
    def __init__(self, led_count: int, led_pin: int, led_freq: int, led_dma: int, led_invert: bool, led_brightness: int,
                 led_channel: int):
        super.__init__(led_count, led_pin, led_freq,
                       led_dma, led_invert, led_brightness, led_channel)

        self.__led_count = led_count
        self.__led_pin = led_pin
        self.__led_freq = led_freq
        self.__led_dma = led_dma
        self.__led_brightness = led_brightness
        self.__led_channel = led_channel
