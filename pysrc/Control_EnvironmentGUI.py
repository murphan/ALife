import sys
from tkinter import *
import pygame
import base64

GREEN = '#0c871b'
YELLOW = '#fafa16'
BLACK = '#000000'

CLICK_TYPE = "Organism"
ENVIRONMENT_GRID = []


class EnvironmentControl:
    def __init__(self, *args, **kwargs):
        """
        This is the initialization of the environment window controls that
        will need to be kept track of throughout the run
        """
        # TODO: Make these correspond to the correct values in setup_environmentGUI
        self.height = 1
        self.width = 1
        self.running = False

    def define_grid(self, width, height):
        """
        This will set the correct size of the environment window reference grid

        :param width: width of the environment
        :param type: int, float

        :param height: height of the environment
        :param type: int, float
        """
        global ENVIRONMENT_GRID
        ENVIRONMENT_GRID = [[0for x in range(int(height))] for y in range(int(width))]

    def set_speed(self, speed):
        """
        This is where the speed of the environment is set from the settings window

        :param speed: The speed of the environment. Ex. "2x" or ".5x"
        :param type: string
        """
        settings = self.env_settings
        if settings.speed == float(speed[:-1]):
            return
        else:
            settings.speed = float(speed[:-1])
            EnvironmentControl.send_message(self, settings.conn, "Speed", settings.speed)

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

    def start(self, conn):
        """
        This is where the environment is started up again when it is selected in the environment window
        """
        if self.running:
            return
        else:
            self.running = True
            self.send_message(conn, "Control", self.running)

    def stop(self, conn):
        """
        This is the call to stop whenever it is clicked in the environment window
        """
        if not self.running:
            return
        else:
            self.running = False
            self.send_message(conn, "Control", self.running)

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

    def set_env_vars(self, temperature, light, oxygen):
        """
        This will set each of the three environment variables

        :param temperature: The temperature of the environment in degrees Celsius
        :param type: int

        :param light: The brightness of the environment in percentage
        :param type: int

        :param oxygen: The oxygen level of the environment in percentage
        :param type: int
        """
        settings = self.env_settings
        if settings.temperature == temperature and settings.light == light and settings.oxygen == oxygen:
            return
        else:
            settings.temperature = int(temperature) if temperature != "" else 0
            settings.light = int(light) if light != "" else 0
            settings.oxygen = int(oxygen) if oxygen != "" else 0
            data = settings.temperature.to_bytes(1, "Big", signed=True), \
                settings.light.to_bytes(1, "Big", signed=True), \
                settings.oxygen.to_bytes(1, "Big", signed=True)
            EnvironmentControl.send_message(self, settings.conn, "Environment Variables", data)

    def square_clicked(self, event, envVars, conn):
        """
        This will fill in a square when it is clicked in the environment

        :param event: the clicked event used for finding the clicked position
        :param type: event type

        :param envVars: Variables from the environment window
        :param type: pygame.display

        :param conn: connection to send message with
        :param type: socket
        """
        global ENVIRONMENT_GRID, CLICK_TYPE
        mpos_x, mpos_y = event.pos
        coord = mpos_x // 10, mpos_y // 10
        rect = pygame.Rect(coord[0] * 10, coord[1] * 10,
                           10, 10)
        # Check that the coordinates are within the bounds of the environment (only check height)
        if coord[1] >= envVars.environment_size[1]:
            return
        if ENVIRONMENT_GRID[coord[0]][coord[1]] != 0:
            if ENVIRONMENT_GRID[coord[0]][coord[1]] == 1:
                return  # This is food
            if ENVIRONMENT_GRID[coord[0]][coord[1]] == 2:
                return  # This is a wall cell
            else:
                org_id = ENVIRONMENT_GRID[coord[0]][coord[1]]
                self.send_message(conn, "Request", org_id)
        else:
            # Grid coords are different in python than they are in c++
            y_coord = int(envVars.environment_size[1] - coord[1] - 1)
            if CLICK_TYPE == 'Organism':
                pygame.draw.rect(envVars.SCREEN, GREEN, rect)
                ENVIRONMENT_GRID[coord[0]][coord[1]] = 1000
                self.send_message(conn, "New Filled", (coord[0], y_coord, "Organism"))
            elif CLICK_TYPE == 'Food':
                pygame.draw.rect(envVars.SCREEN, YELLOW, rect)
                ENVIRONMENT_GRID[coord[0]][coord[1]] = 1
                self.send_message(conn, "New Filled", (coord[0], y_coord, "Food"))
            else:
                pygame.draw.rect(envVars.SCREEN, BLACK, rect)
                ENVIRONMENT_GRID[coord[0]][coord[1]] = 2
                self.send_message(conn, "New Filled", (coord[0], y_coord, "Wall"))

    def click_type(self, clicked_type):
        """
        When a square is clicked in the environment, the user can
        specify what they want it to be

        :param clicked_type: The type of square the user will fill with
        :param type: String
        """
        global CLICK_TYPE
        CLICK_TYPE = clicked_type

    def decode_message(self, conn):
        """
        This will decode the message that is sent from the c++ application. This will be
        started in setup Environment and will be in its own process. It will loop
        infinitely looking for messages

        NOTE: Self was passed from setup_environment and so this will be a setup_environment instance of self
        """
        while True:
            message_buf = ''  # The buffer to append message information to

            message = conn.recv(1024)
            if message:
                length = int.from_bytes(message[:4], "big")
                message = message[4:]
                message = message.decode("utf-8", errors="ignore")
                while length:
                    if length < 1022:
                        message_buf += message
                        length = 0
                        break
                    else:
                        message_buf += message
                        length -= 1022
                    message = conn.recv(1024).decode("utf-8", errors="ignore")

                print("message received")

            # TODO: Process the commands here

    def send_message(self, conn, message_type, data=None):
        """
        This is the spot where a message can be sent to the c++ application

        NOTE: Self was passed from setup_environment and so this will be a setup_environment instance of self

        :param conn: connection to send the message to
        :param type: socket

        :param message_type: The type of message that will be sent (ex. Control, Request, Environment Variables)
        :param type: string

        :param data: Optional argument for passing data that may be needed in c++
        :param type: Any
        """
        # TODO: Check the message type and create a message that is different based on the type that is given
        message = EnvironmentControl.create_message(self, message_type, data)
        length = len(message)
        if len(str(length)) < 4:
            length = str(length).zfill(4)
            message = str(length).encode("utf-8") + message.encode("utf-8")
        else:
            message = str(length).encode("utf-8") + message.encode("utf-8")

        conn.send(message)

    def create_message(self, message_type, data=None):
        """
        This function will generate a new message based on the message type and the data that is with it
        :param message_type: The type of message that will be sent (ex. Control, Request, Environment Variables)
        :param type: string

        :param data: Optional argument for passing data that may be needed in c++
        :param type: Any
        """
        if message_type == "Control" or message_type == "Request" \
           or message_type == "Speed" or message_type == "Request All":
            return "{type: " + message_type + ", data: " + str(data) + "}"
        elif message_type == "Environment Variables":
            formatted_data = "{temperature: " + str(data[0]) + ", light: " \
                             + str(data[1]) + ", oxygen: " + str(data[2]) + "}"
            return "{type: " + message_type + ", data: " + formatted_data + "}"
        elif message_type == "New Filled":
            formatted_data = "{x: " + str(data[0]) + ", y: " + str(data[1]) + ", type: " + str(data[2]) + "}"
            return "{type: " + message_type + ", data: " + formatted_data + "}"
