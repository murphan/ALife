import sys
from tkinter import *
import pygame
import base64
import json
import ast
import random

import Global_access


class EnvironmentControl:
    def __init__(self, *args, **kwargs):
        """
        This is the initialization of the environment window controls that
        will need to be kept track of throughout the run
        """
        pass

    def set_speed(self, speed):
        """
        This is where the speed of the environment is set from the settings window

        :param speed: The speed of the environment. Ex. "2x" or ".5x"
        :param type: string
        """
        if Global_access.speed == float(speed[:-1]):
            return
        else:
            Global_access.change_speed(float(speed[:-1]))
            EnvironmentControl.send_message(self, self.env_settings.conn, "speed", Global_access.speed)

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
        if Global_access.running:
            return
        else:
            Global_access.running = True
            self.send_message(conn, "control", "true")

    def stop(self, conn):
        """
        This is the call to stop whenever it is clicked in the environment window
        """
        if not Global_access.running:
            return
        else:
            Global_access.running = False
            self.send_message(conn, "control", "false")

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
        if Global_access.temperature == temperature and Global_access.light == light and Global_access.oxygen == oxygen:
            return
        else:
            Global_access.change_temperature(int(temperature) if temperature != "" else 0)
            Global_access.change_light(int(light) if light != "" else 0)
            Global_access.change_oxygen(int(oxygen) if oxygen != "" else 0)
            data = Global_access.temperature, Global_access.light, Global_access.oxygen
            EnvironmentControl.send_message(self, settings.conn, "environment_variables", data)

    def square_clicked(self, event, envVars, conn):
        """
        This will fill in a square when it is clicked in the environment

        :param event: the clicked event used for finding the clicked position
        :param type: event type

        :param envVars: Variables from the environment window
        :param type: setupenvironment instance

        :param conn: connection to send message with
        :param type: socket
        """
        mpos_x, mpos_y = event.pos
        coord = mpos_x // 10, mpos_y // 10
        rect = pygame.Rect(coord[0] * 10, coord[1] * 10,
                           10, 10)
        # Check that the coordinates are within the bounds of the environment (only check height)
        if coord[1] >= Global_access.environment_size[1]:
            return
        if Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]] != 0:
            if Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]] == 1:
                return  # This is food
            if Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]] == 2:
                return  # This is a wall cell
            else:
                org_id = Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]
                self.send_message(conn, "request", org_id)
        else:
            # Grid coords are different in python than they are in c++
            y_coord = int(Global_access.environment_size[1] - coord[1] - 1)
            if Global_access.CLICK_TYPE == 'Organism':
                pygame.draw.rect(Global_access.SCREEN, Global_access.GREEN, rect)
                Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]] = 1000
                self.send_message(conn, "new_filled", (coord[0], y_coord, "organism"))
            elif Global_access.CLICK_TYPE == 'Food':
                pygame.draw.rect(Global_access.SCREEN, Global_access.YELLOW, rect)
                Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]] = 1
                self.send_message(conn, "new_filled", (coord[0], y_coord, "food"))
            else:
                pygame.draw.rect(Global_access.SCREEN, Global_access.BLACK, rect)
                Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]] = 2
                self.send_message(conn, "new_filled", (coord[0], y_coord, "wall"))

    def fill_cell(self, x, y):
        """
        This will fill in a cell when the information is passed from c++

        """
        rect = pygame.Rect(x * 10, y * 10,
                           10, 10)
        # y_coord = int(envVars.environment_size[1] - coord[1] - 1)
        pygame.draw.rect(Global_access.SCREEN, Global_access.GREEN, rect)
        Global_access.ENVIRONMENT_GRID[x][y] = 1000

    def click_type(self, clicked_type):
        """
        When a square is clicked in the environment, the user can
        specify what they want it to be

        :param clicked_type: The type of square the user will fill with
        :param type: String
        """
        Global_access.change_click_type(clicked_type)

    def decode_message(self, conn):
        """
        This will decode the message that is sent from the c++ application. This will be
        started in setup Environment and will be in its own process. It will loop
        infinitely looking for messages

        NOTE: Self was passed from setup_environment and so this will be a setup_environment instance of self
        """
        while True and not Global_access.EXIT:
            message_buf = ''  # The buffer to append message information to

            message = conn.recv(1024)
            if message:
                length = int.from_bytes(message[:4], "big")
                length_same = length
                message = message[4:]
                message = message.decode(errors="ignore")
                while length:
                    if length < 1020:
                        message_buf += message
                        length = 0
                        break
                    else:
                        message_buf += message
                        length -= 1020

                    if length < 1020:
                        message = conn.recv(length).decode(errors="ignore")
                    else:
                        message = conn.recv(1024).decode(errors="ignore")
                print("message received")

                data_map = ast.literal_eval(message_buf[:length_same])
                message_type = data_map["type"]
                if message_type == "environment_frame":
                    pass
                    data_map["grid"] = data_map["grid"].split("/")
                    for cell in data_map["grid"]:
                        self.fill_cell(random.randint(0, 100), random.randint(0, 50))
                        pass

            # TODO: Process the commands here

    def send_message(self, conn, message_type, data=""):
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
        if message_type == "control" or message_type == "request" \
           or message_type == "speed" or message_type == "request_all":
            return "{type:" + message_type + ",data:" + str(data) + "}"
        elif message_type == "environment_variables":
            formatted_data = "{temperature:" + str(data[0]) + ",light:" \
                             + str(data[1]) + ",oxygen:" + str(data[2]) + "}"
            return "{type: " + message_type + ",data:" + formatted_data + "}"
        elif message_type == "new_filled":
            formatted_data = "{x:" + str(data[0]) + ",y:" + str(data[1]) + ",type:" + str(data[2]) + "}"
            return "{type:" + message_type + ",data:" + formatted_data + "}"
