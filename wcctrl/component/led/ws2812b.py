from wcctrl.type.color import ColorRGB
from neopixel import *


class LEDStrip(Adafruit_NeoPixel):
    def __init__(self, led_count, led_pin, led_freq, led_dma, led_invert, led_brightness, led_channel):
        Adafruit_NeoPixel.__init__(self, led_count, led_pin, led_freq, led_dma, led_invert, led_brightness, led_channel)

        self.__led_count = led_count

    def turn_on_led(self, led_nr: int, c: ColorRGB):
        self.setPixelColor(led_nr, Color(c.green, c.red, c.blue))

    def turn_on_leds(self, led_list: list, c: ColorRGB):
        for led in led_list:
            self.turn_on_led(led, c)

    def turn_off_led(self, led_nr: int):
        self.setPixelColor(led_nr, Color(0, 0, 0))

    def turn_off_leds(self, led_list: list):
        for led in led_list:
            self.turn_off_led(led)

    def turn_off_all_leds(self):
        for led in range(0, self.__led_count):
            self.turn_off_led(led)
