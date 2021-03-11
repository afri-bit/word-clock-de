from unittest import TestCase

from wcctrl.hardware.rtc.ds1302 import *

import datetime


class TestDS1302(TestCase):
    def __init__(self):
        self.rtc = DS1302()

    def test_get_time(self):
        print(self.rtc.read_datetime())
        self.rtc.close()
        pass

    def test_set_time(self):
        self.rtc.write_datetime(datetime.datetime.utcnow())

        self.rtc.close()
        pass