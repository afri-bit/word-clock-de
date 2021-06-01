from unittest import TestCase

from wcctrl.hardware.sensor.brightness.bh1750 import *

import smbus


class TestBH1750(TestCase):
    def __init__(self):
        # bus = smbus.SMBus(0) # Rev 1 Pi uses 0
        self.bus = smbus.SMBus(1)  # Rev 2 Pi uses 1
        self.sensor = BH1750(self.bus)

    def test_sensor(self):
        print("Sensitivity: {:d}".format(self.sensor.mtreg))

        for measurefunc, name in [(self.sensor.measure_low_res, "Low Res "), (self.sensor.measure_high_res, "HighRes "),
                                  (self.sensor.measure_high_res2, "HighRes2")]:
            print("{} Light Level : {:3.2f} lx".format(name, measurefunc()))

        print("--------")

        self.sensor.set_sensitivity((self.sensor.mtreg + 10) % 255)
