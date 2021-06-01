from gpiozero import DigitalInputDevice


class RCWL0516:
    def __init__(self, gpio_num: int):
        self.__radar = radar = DigitalInputDevice(gpio_num, pull_up=False, bounce_time=2.0)

    def motion_detected(self):
        return bool(self.__radar.value)
