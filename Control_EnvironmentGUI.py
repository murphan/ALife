from tkinter import *


class EnvironmentControl:
    def __init__(self, *args, **kwargs):
        self.speed = 1
        # TODO: Make these correspond to the correct values in setup_environmentGUI
        self.height = 1
        self.width = 1

    def set_speed(self, speed):
        self.speed = int(speed[:-1])

    def set_size(self, width, height):
        self.height = height
        self.width = width

    def start(self):
        print("start")
        pass

    def stop(self):
        print("stop")
        pass

    def settings_window(self):
        print("settings")
        pass

    def set_width(self, width):
        print(f"width: {width}")
        pass

    def set_height(self, height):
        print(f"height: {height}")
        pass

    def set_temperature(self, temperature):
        print(f"temperature: {temperature}")
        pass

    def set_light(self, brightness):
        print(f"brightness: {brightness}")
        pass

    def set_oxygen(self, level):
        print(f"oxygen level: {level}")
        pass
