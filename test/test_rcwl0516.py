from unittest import TestCase

from wcctrl.hardware.sensor.motion.rcwl0516 import *

import time


class TestRCWL0516(TestCase):
    def test_motion_detection(self):
        sensor = RCWL0516()

        for i in range(0, 5):
            print("Motion detection: " + str(sensor.motion_detected()))
            time.sleep(1)
