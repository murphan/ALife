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

    def set_fps(self, fps):
        """
        This is where the fps of the environment is set from the settings window

        :param fps: The fps of the environment.
        :param type: int
        """
        if Global_access.fps == fps:
            return
        else:
            Global_access.change_fps(fps)
            EnvironmentControl.send_message(self, self.env_settings.conn, "control")

    def set_temp_noise(self, noise):
        """
        This will set if there is noise or not

        :param noise: Value of if there is noise or not. Either 0 or 1
        :param noise: Int
        """
        Global_access.temp_noise = noise
        EnvironmentControl.send_message(self, self.env_settings.conn, "settings")

    def set_temp_noise_levels(self, noise_type, value):
        """
        This will set the noise levels of the temp variable

        :param noise_type: temp noise type (ex. scale, depth, speed)
        :type noise_type: String

        :param value: temp noise value
        :type value: float
        """
        if noise_type == 'scale':
            Global_access.temp_scale = value
        elif noise_type == 'depth':
            Global_access.temp_depth = value
        elif noise_type == 'speed':
            Global_access.temp_speed = value
        EnvironmentControl.send_message(self, self.env_settings.conn, "settings")

    def set_temp_value(self, value):
        """
        if the temperature variable isn't using noise levels, it will
        use the noise value which is set here

        :param value: Value to set the temperature variable to
        :type value: float
        """
        Global_access.temperature = value
        EnvironmentControl.send_message(self, self.env_settings.conn, "settings")

    def set_light_noise(self, noise):
        """
        This will set if there is noise or not

        :param noise: Value of if there is noise or not. Either 0 or 1
        :param noise: Int
        """
        Global_access.light_noise = noise
        EnvironmentControl.send_message(self, self.env_settings.conn, "settings")

    def set_light_noise_levels(self, noise_type, value):
        """
        This will set the noise levels of the light variable

        :param noise_type: light noise type (ex. scale, depth, speed)
        :type noise_type: String

        :param value: light noise value
        :type value: float
        """
        if noise_type == 'scale':
            Global_access.light_scale = value
        elif noise_type == 'depth':
            Global_access.light_depth = value
        elif noise_type == 'speed':
            Global_access.light_speed = value
        EnvironmentControl.send_message(self, self.env_settings.conn, "settings")

    def set_light_value(self, value):
        """
        if the light variable isn't using noise levels, it will
        use the noise value which is set here

        :param value: Value to set the light variable to
        :type value: float
        """
        Global_access.light = value
        EnvironmentControl.send_message(self, self.env_settings.conn, "settings")

    def set_oxygen_noise(self, noise):
        """
        This will set if there is noise or not

        :param noise: Value of if there is noise or not. Either 0 or 1
        :param noise: Int
        """
        Global_access.oxygen_noise = noise
        EnvironmentControl.send_message(self, self.env_settings.conn, "settings")

    def set_oxygen_noise_levels(self, noise_type, value):
        """
        This will set the noise levels of the oxygen variable

        :param noise_type: oxygen noise type (ex. scale, depth, speed)
        :type noise_type: String

        :param value: oxygen noise value
        :type value: float
        """
        if noise_type == 'scale':
            Global_access.oxygen_scale = value
        elif noise_type == 'depth':
            Global_access.oxygen_depth = value
        elif noise_type == 'speed':
            Global_access.oxygen_speed = value
        EnvironmentControl.send_message(self, self.env_settings.conn, "settings")

    def set_oxygen_value(self, value):
        """
        if the oxygen variable isn't using noise levels, it will
        use the noise value which is set here

        :param value: Value to set the oxygen variable to
        :type value: float
        """
        Global_access.oxygen = value
        EnvironmentControl.send_message(self, self.env_settings.conn, "settings")

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
        elif Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"] is not None and \
            Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"].tile_type == Global_access.TILE_TYPE_EMPTY:
            if Global_access.CLICK_TYPE == 'Organism':
                self.fill_cell(coord[0], coord[1], Global_access.org_colors[1])
                new_cell = Organism_cell.OrganismCell(0, 0, 0, 0, 0, 0, 0, 0, 0, coord[0], coord[1])
                Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["organism"] = new_cell
                self.send_message(conn, "new_filled", (coord[0], coord[1], "organism"))
            elif Global_access.CLICK_TYPE == 'Food':
                self.fill_cell(coord[0], coord[1], Global_access.YELLOW)
                new_cell = Environment_cell.EnvironmentCell(0, 0, 0, 0, 0, 0, 0, 0)
                Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"] = new_cell
                self.send_message(conn, "new_filled", (coord[0], coord[1], "food"))
            else:  # Wall
                self.fill_cell(coord[0], coord[1], Global_access.WHITE)
                new_cell = Environment_cell.EnvironmentCell(0, 0, 0, 0, -1, 0, 0, 0)
                Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"] = new_cell
                self.send_message(conn, "new_filled", (coord[0], coord[1], "wall"))

    def fill_cell(self, x, y, color, circle = False):
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

        if circle:
            pygame.draw.circle(Global_access.second_surface, color, (x * 10 + 5, y * 10 + 5), 5)
        else:
            pygame.draw.rect(Global_access.second_surface, color, (x * 10, y * 10, 10, 10))

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
        jsonPart = EnvironmentControl.create_message(self, message_type, data)
        message = len(jsonPart).to_bytes(4, "big") + jsonPart.encode("utf-8")

        conn.send(message)

    def create_message(self, message_type, data=None):
        """
        This function will generate a new message based on the message type and the data that is with it
        :param message_type: The type of message that will be sent (ex. Control, Request, Environment Variables)
        :param type: string

        :param data: Optional argument for passing data that may be needed in c++
        :param type: Any
        """
        if message_type == "request":
            return json.dumps({
                "type": "request",
                "id": data,
            })
        elif message_type == "request_all":
            # unused
            return json.dumps({})
        elif message_type == "control":
            return json.dumps({
                "type": "control",
                "control": {
                    "playing": Global_access.running,
                    "fps": Global_access.fps,
                    "updateDisplay": Global_access.updateDisplay
                }
            })
        elif message_type == "new_filled":
            new_data = {"x": str(data[0]),
                        "y": str(data[1]),
                        "type": str(data[2])}
            formatted_data = {"data": new_data}
            formatted = {message_type: formatted_data}
            return json.dumps(formatted)
        elif message_type == "settings":
            return json.dumps({
                "type": "settings",
                "settings": {
                    "factors": [
                        {
                            "useNoise": bool(Global_access.temp_noise),
                            "center": Global_access.temperature,
                            "speed": Global_access.temp_speed,
                            "scale": Global_access.temp_scale,
                            "amplitude": Global_access.temp_depth,
                        },
                        {
                            "useNoise": bool(Global_access.light_noise),
                            "center": Global_access.light,
                            "speed": Global_access.light_speed,
                            "scale": Global_access.light_scale,
                            "amplitude": Global_access.light_depth,
                        },
                        {
                            "useNoise": bool(Global_access.oxygen_noise),
                            "center": Global_access.oxygen,
                            "speed": Global_access.oxygen_speed,
                            "scale": Global_access.oxygen_scale,
                            "amplitude": Global_access.oxygen_depth,
                        }
                    ]
                }
            })
        elif message_type == "init":
            return json.dumps({
                "type": "init"
            })
