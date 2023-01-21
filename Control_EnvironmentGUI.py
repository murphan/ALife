from tkinter import *
import pygame


GREEN = '#0c871b'


class EnvironmentControl:
    def __init__(self, *args, **kwargs):
        """
        This is the initialization of the environment window controls that
        will need to be kept track of throughout the run
        """
        self.speed = 1
        # TODO: Make these correspond to the correct values in setup_environmentGUI
        self.height = 1
        self.width = 1
        self.clicktype = 'Organism'

    def set_speed(self, speed):
        """
        This is where the speed of the environment is set from the settings window

        :param speed: The speed of the environment. Ex. "2x" or ".5x"
        :param type: string
        """
        self.speed = int(speed[:-1])

    def set_size(self, width, height):
        """
        This is where the size is changed together. This may be called at a
        different time from the individual setter functions

        :param width: Width of the window
        :param type: int

        :param height: Height of the window
        :param type: int
        """
        self.height = height
        self.width = width

    def start(self):
        """
        This is where the environment is started up again when it is selected in the environment window
        """
        # print("start")
        pass

    def stop(self):
        """
        This is the call to stop whenever it is clicked in the environment window
        """
        /print("stop")
        pass

    def set_width(self, width):
        """
        This is where the width is set and adjusted from the settings window

        :param width: The width of the window
        :Param type: int
        """
        # print(f"width: {width}")
        self.width = width

    def set_height(self, height):
        """
        This is where the height is set and adjusted from the settings window

        :param height: The height of the window
        :param type: int
        """
        # print(f"height: {height}")
        self.height = height

    def set_temperature(self, temperature):
        """
        This is where the temperature level is set and changed from the settings window

        :param temperature: The temperature of the environment in degrees Celsius
        :param type: int
        """
        # print(f"temperature: {temperature}")
        pass

    def set_light(self, brightness):
        """
        This is where the light level will be set and changed from the settings window

        :param brightness: The brightness of the environment in percentage
        :param type: int
        """
        # print(f"brightness: {brightness}")
        pass

    def set_oxygen(self, level):
        """
        This is where the oxygen level will be set and changed from the settings window

        :param level: The oxygen level of the environment in percentage
        :param type: int
        """
        # print(f"oxygen level: {level}")
        pass

    def square_clicked(self, event, SCREEN):
        """
        This will fill in a square when it is clicked in the environment

        :param event: the clicked event used for finding the clicked position
        :param type: event type

        :param SCREEN: the SCREEN parameters from the environment window
        :param type: pygame.display
        """
        mpos_x, mpos_y = event.pos
        coord = mpos_x // 10, mpos_y // 10
        rect = pygame.Rect(coord[0] * 10, coord[1] * 10,
                           10, 10)
        # TODO: Might need to change the color of this rectangle fill
        pygame.draw.rect(SCREEN, GREEN, rect)
        self.environment_grid[coord[0]][coord[1]] = 1

    def click_type(self, clicked_type):
        """
        When a square is clicked in the environment, the user can
        specify what they want it to be

        :param clicked_type: The type of square the user will fill with
        :param type: String
        """
        self.clicktype = clicked_type
