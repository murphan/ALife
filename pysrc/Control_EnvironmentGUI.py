import sys
from tkinter import *
import pygame
import json

import Global_access
import Organism_cell
import Environment_cell


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
            EnvironmentControl.send_message(self, self.env_settings.conn, "control")

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
            self.send_message(conn, "control")

    def stop(self, conn):
        """
        This is the call to stop whenever it is clicked in the environment window
        """
        if not Global_access.running:
            return
        else:
            Global_access.running = False
            self.send_message(conn, "control")

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
        # Grid coords are different in python than they are in c++ for y coordinates
        coord = mpos_x // 10, int(Global_access.environment_size[1] - (mpos_y // 10) - 1)

        # Check that the coordinates are within the bounds of the environment (only check height)
        if coord[1] < 0:
            return

        # This block checks if an organism is in the cell and then requests data if it is
        if Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["organism"] is not None:
            org_id = Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["organism"].organism_id
            self.send_message(conn, "request", org_id)
        # If the cell wasn't filled with an organism we check if it is empty (not food or a wall cell but empty)
        elif Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"].tile_type == -2:
            if Global_access.CLICK_TYPE == 'Organism':
                self.fill_cell(coord[0], coord[1], Global_access.GREEN)
                new_cell = Organism_cell.OrganismCell(0, 0, 0, 0, 0, 0, 0, 0, 0, coord[0], coord[1])
                Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["organism"] = new_cell
                self.send_message(conn, "new_filled", (coord[0], coord[1], "organism"))
            elif Global_access.CLICK_TYPE == 'Food':
                self.fill_cell(coord[0], coord[1], Global_access.YELLOW)
                new_cell = Environment_cell.EnvironmentCell(0, 0, 0, 0, 0, 0, 0, 0)
                Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"] = new_cell
                self.send_message(conn, "new_filled", (coord[0], coord[1], "food"))
            else:
                self.fill_cell(coord[0], coord[1], Global_access.BLACK)
                new_cell = Environment_cell.EnvironmentCell(0, 0, 0, 0, -1, 0, 0, 0)
                Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"] = new_cell
                self.send_message(conn, "new_filled", (coord[0], coord[1], "wall"))

    def fill_cell(self, x, y, color):
        """
        This will fill in a cell when the information is passed from c++
        Grid coords are different in python than they are in c++.
        This converts from the c++ coordinate style to python

        :param x: x coordinate
        :type x: int

        :param y: y coordinate
        :type y: int

        :param color: color to fill cell with
        :type color: Global_access.color
        """
        y = int(Global_access.environment_size[1] - y - 1)
        rect = pygame.Rect(x * 10, y * 10,
                           10, 10)
        pygame.draw.rect(Global_access.SCREEN, color, rect)

    def click_type(self, clicked_type):
        """
        When a square is clicked in the environment, the user can
        specify what they want it to be

        :param clicked_type: The type of square the user will fill with
        :param type: String
        """
        Global_access.change_click_type(clicked_type)

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
        if message_type == "request" or message_type == "request_all":
            return "{type:" + message_type + ",data:" + str(data) + "}"
        elif message_type == "control":
            formatted_data = "{playing:" + str(Global_access.running) + \
                             ",speed:" + str(Global_access.speed) + "}"
        elif message_type == "environment_variables":
            formatted_data = "{temperature:" + str(data[0]) + \
                             ",light:" + str(data[1]) + \
                             ",oxygen:" + str(data[2]) + "}"
        elif message_type == "new_filled":
            formatted_data = "{x:" + str(data[0]) + \
                             ",y:" + str(data[1]) + \
                             ",type:" + str(data[2]) + "}"

        return "{type:" + message_type + ",data:" + formatted_data + "}"
