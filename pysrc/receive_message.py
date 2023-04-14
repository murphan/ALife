import base64
import ast
import random
import json
import pygame.display

import Global_access
import Control_EnvironmentGUI
import Environment_cell
import Organism_cell


def decode_message(self, conn):
    """
    This will decode the message that is sent from the c++ application. This will be
    started in setup Environment and will be in its own process. It will loop
    infinitely looking for messages

    :param self: instance of setup_environment self that can be passed but isn't used
    :type self: setup_environment instance

    :param conn: connection to receive messages from
    :type conn: socket

    NOTE: Self was passed from setup_environment and so this will be a setup_environment instance of self
    """

    while True and not Global_access.EXIT:
        message_buf = ''  # The buffer to append message information to

        length = conn.recv(4)
        if length:
            length = int.from_bytes(length, "big")
            length_same = length
            while length > 0:
                if length < 1024:
                    message = conn.recv(length).decode(errors="ignore")
                    message_buf += message
                    length = 0
                else:
                    message = conn.recv(1024).decode(errors="ignore")
                    message_buf += message
                    length -= len(message)

            data_map = json.loads(message_buf[:length_same])

            message_type = data_map["type"]
            if message_type == "frame":
                handle_environment_data(self, data_map["environment"])

            elif message_type == "init":
                handle_environment_data(self, data_map["environment"])
                handle_control_data(self, data_map["control"])
                handle_settings_data(data_map["settings"])

            elif message_type == "organism_data":
                print("organism data received")

            elif message_type == "control":
                handle_control_data(self, data_map["control"])

            elif message_type == "settings":
                handle_settings_data(data_map["settings"])


def handle_environment_data(self, environment_data_map):
    height = environment_data_map["height"]
    width = environment_data_map["width"]

    # TODO make grid dynamically sized
    # Global_access.define_grid(width, height)

    render_grid(
        self,
        buffer=base64.b64decode(environment_data_map["grid"]),
        uuids=environment_data_map["organisms"],
        width=width,
        height=height
    )

    Global_access.new_frame = True


def handle_control_data(self, control):
    """
    update internal control values to match what the backend says is correct
    do not send a message back
    """
    Global_access.change_fps(control["fps"])
    Global_access.updateDisplay = control["updateDisplay"]
    Global_access.running = control["playing"]


def handle_settings_data(settings):
    factors = settings["factors"]

    # sorry
    noise =factors[0]
    Global_access.temp_noise = int(noise["useNoise"])
    Global_access.temperature = noise["center"]
    Global_access.temp_speed = noise["speed"]
    Global_access.temp_scale = noise["scale"]
    Global_access.temp_depth = noise["amplitude"]

    noise = factors[1]
    Global_access.light_noise = int(noise["useNoise"])
    Global_access.light = noise["center"]
    Global_access.light_speed = noise["speed"]
    Global_access.light_scale = noise["scale"]
    Global_access.light_depth = noise["amplitude"]

    noise = factors[2]
    Global_access.oxygen_noise = int(noise["useNoise"])
    Global_access.oxygen = noise["center"]
    Global_access.oxygen_speed = noise["speed"]
    Global_access.oxygen_scale = noise["scale"]
    Global_access.oxygen_depth = noise["amplitude"]


def render_grid(self, buffer: bytes, uuids: list, width: int, height: int):
    BYTES_PER_TILE = 9

    for y in range(height):
        for x in range(width):
            baseIndex = (y * width + x) * BYTES_PER_TILE

            meta_byte      = int.from_bytes(buffer[baseIndex + 0:baseIndex + 1], "big", signed=False)
            organism_index = int.from_bytes(buffer[baseIndex + 1:baseIndex + 3], "big", signed=False)
            color_0        = int.from_bytes(buffer[baseIndex + 3:baseIndex + 6], "big", signed=False)
            color_1        = int.from_bytes(buffer[baseIndex + 6:baseIndex + 9], "big", signed=False)

            draw_circle = meta_byte >> 7 == 1
            meta_type = meta_byte & 0x7f

            cell = Environment_cell.EnvironmentCell(0, 0, 0, meta_type, 0, 0, 0)

            Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x, y, color_0)

            if draw_circle:
               Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x, y, color_1, circle=True)

            Global_access.ENVIRONMENT_GRID[x][y]["environment"] = cell
            if meta_type == Global_access.TILE_TYPE_ORGANISM:
                Global_access.ENVIRONMENT_GRID[x][y]["organism"] = uuids[organism_index]
            else:
                Global_access.ENVIRONMENT_GRID[x][y]["organism"] = None
