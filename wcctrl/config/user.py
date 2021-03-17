import os
import json
from pathlib import Path

from wcctrl.type.color import *
from wcctrl.type.brightness import *
from wcctrl.type.light import *


class UserConfig:
    __instance = None

    @staticmethod
    def get_instance():
        """ Static access method. """
        if UserConfig.__instance is None:
            UserConfig()
        return UserConfig.__instance

    def __init__(self):
        """ Virtually private constructor. """
        if UserConfig.__instance is not None:
            raise Exception("Cannot create multiple instances of this class. Singleton!")
        else:
            UserConfig.__instance = self

        # Path variables
        self.__home_path = os.path.abspath(str(Path.home()))
        self.__config_path = os.path.abspath(os.path.join(self.__home_path, ".wordclock"))
        self.__config_file = os.path.abspath(os.path.join(self.__config_path, "wordclock.json"))

        # System config
        self.__config = dict()

        # Check if file exists
        if not os.path.isfile(self.__config_file):  # File does not exists
            if not os.path.isdir(self.__config_path):  # Directory does not exists
                os.mkdir(self.__config_path)

            self.__init_config()
            self.__update_config()
        else:  # Read from existing file
            with open(self.__config_file, "r") as f:
                self.__config = json.load(f)

    def __init_config(self):
        self.__config["color"] = dict()
        self.__config["brightness"] = dict()
        self.__config["light"] = dict()
        self.__config["color"]["background"] = ColorRGB(0, 0, 0).__dict__
        self.__config["color"]["active_letter"] = ColorRGB(255, 255, 255).__dict__
        self.__config["color"]["minute"] = ColorRGB(125, 0, 125).__dict__
        self.__config["color"]["second"] = ColorRGB(0, 255, 0).__dict__
        self.__config["color"]["ampm"] = ColorRGB(255, 0, 0).__dict__
        self.__config["brightness"] = Brightness(True, 255, 50).__dict__
        self.__config["light"] = Light(False, 60).__dict__

    def __update_config(self):
        with open(self.__config_file, "w") as f:
            f.write(json.dumps(self.__config, indent=2))
        pass

    def get_config(self) -> dict:
        return self.__config

    def set_color_background(self, val: ColorRGB):
        self.__config["color"]["background"] = val.__dict__
        self.__update_config()

    def set_color_active_letter(self, val: ColorRGB):
        self.__config["color"]["active_letter"] = val.__dict__
        self.__update_config()

    def set_color_minute(self, val: ColorRGB):
        self.__config["color"]["minute"] = val.__dict__
        self.__update_config()

    def set_color_second(self, val: ColorRGB):
        self.__config["color"]["second"] = val.__dict__
        self.__update_config()

    def set_color_ampm(self, val: ColorRGB):
        self.__config["color"]["second"] = val.__dict__
        self.__update_config()

    def set_brightness(self, val: Brightness):
        self.__config["brightness"] = val.__dict__
        self.__update_config()

    def set_light(self, val: Light):
        self.__config["light"] = val.__dict__
        self.__update_config()

    def get_color_background(self) -> ColorRGB:
        return ColorRGB(self.__config["color"]["background"]["red"],
                        self.__config["color"]["background"]["green"],
                        self.__config["color"]["background"]["blue"]
                        )

    def get_color_active_letter(self) -> ColorRGB:
        return ColorRGB(self.__config["color"]["active_letter"]["red"],
                        self.__config["color"]["active_letter"]["green"],
                        self.__config["color"]["active_letter"]["blue"]
                        )

    def get_color_minute(self) -> ColorRGB:
        return ColorRGB(self.__config["color"]["minute"]["red"],
                        self.__config["color"]["minute"]["green"],
                        self.__config["color"]["minute"]["blue"]
                        )

    def get_color_second(self) -> ColorRGB:
        return ColorRGB(self.__config["color"]["second"]["red"],
                        self.__config["color"]["second"]["green"],
                        self.__config["color"]["second"]["blue"]
                        )

    def get_color_ampm(self) -> ColorRGB:
        return ColorRGB(self.__config["color"]["ampm"]["red"],
                        self.__config["color"]["ampm"]["green"],
                        self.__config["color"]["ampm"]["blue"]
                        )

    def get_brightness(self) -> Brightness:
        return Brightness(self.__config["brightness"]["auto"],
                          self.__config["brightness"]["max"],
                          self.__config["brightness"]["value"]
                          )

    def get_light(self) -> Light:
        return Light(self.__config["light"]["always_on"],
                     self.__config["light"]["timeout"]
                     )
