import base64
import ast
import random

import Global_access
import Control_EnvironmentGUI
import Environment_cell
import Organism_cell


def decode_message(self, conn):
    """
    This will decode the message that is sent from the c++ application. This will be
    started in setup Environment and will be in its own process. It will loop
    infinitely looking for messages

    :param conn: connection to receive messages from
    :type conn: socket

    :param self: instance of setup_environment self that can be passed but isn't used
    :type self: setup_environment instance

    NOTE: Self was passed from setup_environment and so this will be a setup_environment instance of self
    """
    while True and not Global_access.EXIT:
        message_buf = ''  # The buffer to append message information to
        first_time = True  # Flag for if this is the first of the message received

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
                elif first_time:
                    message_buf += message
                    length -= 1020
                    first_time = False
                else:
                    message_buf += message
                    length -= 1024

                if length < 1020:
                    message = conn.recv(length).decode(errors="ignore")
                else:
                    message = conn.recv(1024).decode(errors="ignore")
            print("message received")

            data_map = ast.literal_eval(message_buf[:length_same])
            message_type = data_map["type"]
            if message_type == "environment_frame":
                height = data_map["height"]
                width = data_map["width"]
                # Global_access.define_grid(width, height)   Uncomment when making grid dynamically sized
                data_map["grid"] = base64.b64decode(data_map["grid"])
                data_map["grid"] = [data_map["grid"][i: i + 11] for i in range(0, len(data_map["grid"]), 11)]
                decode_grid(data_map["grid"], width, height)
                decode_organisms(self, data_map["organisms"])
            if message_type == data_map["organism_data"]:
                pass


def decode_grid(grid_data, width, height):
    """
    This will decode the grid that is passed from c++

    :param grid_data: the list of grid information
    :type grid_data: list

    :param width: width of the grid
    :type width: int

    :param height: height of the grid
    :type height: int
    """
    index = 0
    for x in range(width):
        for y in range(height):
            data = grid_data[index]
            factor_0 = int.from_bytes(data[:1], "big", signed=True)
            factor_1 = int.from_bytes(data[1:2], "big", signed=True)
            factor_2 = int.from_bytes(data[2:3], "big", signed=True)
            factor_3 = int.from_bytes(data[3:4], "big", signed=True)
            tile_type = int.from_bytes(data[4:5], "big", signed=True)
            food_energy = int.from_bytes(data[5:6], "big")
            food_age = int.from_bytes(data[6:9], "big")
            pressure = int.from_bytes(data[9:], "big", signed=True)
            cell = Environment_cell.EnvironmentCell(factor_0, factor_1, factor_2, factor_3,
                                                    tile_type, food_energy, food_age, pressure)
            Global_access.ENVIRONMENT_GRID[x][y]["environment"] = cell
            Global_access.ENVIRONMENT_GRID[x][y]["organism"] = None
            if cell.tile_type == -1:
                Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x, y, Global_access.BLACK)
            if cell.tile_type == 0:
                Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x, y, Global_access.YELLOW)
            if cell.tile_type == 1:
                Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x, y, Global_access.ORANGE)
            if cell.tile_type == 2:
                Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x, y, Global_access.RED)
            if cell.tile_type == 3:
                Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x, y, Global_access.PINK)
            index += 1


def decode_organisms(self, organisms):
    """
    This will decode the organisms that are passed from c++ and put them into objects

    :param self: instance of setup_environment self that can be passed but isn't used
    :type self: setup_environment instance

    :param organisms: The list of organisms that are passed from c++
    :type organisms: list
    """
    for each in organisms:
        organism = Organism_cell.OrganismCell(each["age"], each["body"], each["down"], each["energy"], each["id"],
                                              each["left"], each["right"], each["rotation"], each["up"], each["x"],
                                              each["y"])
        display_organism(self, organism)


def display_organism(self, organism):
    """
    This will loop through the string given for the body of the organism and render them

    :param self: instance of setup_environment self that can be passed but isn't used
    :type self: setup_environment instance

    :param organism: The organism to display
    :type organism: OrganismCell instance
    """
    index = 0  # Index of the body string that we are at
    body = base64.b64decode(organism.body)
    top_left = organism.x + organism.up, organism.y + organism.left
    width = organism.right - organism.left + 1
    height = organism.up - organism.down + 1
    for x in range(width):
        for y in range(height):
            x_pos = organism.x + x
            y_pos = organism.y + y
            body_cell_num = body[index]
            Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x_pos, y_pos,
                                                                Global_access.org_colors[body_cell_num])
            Global_access.ENVIRONMENT_GRID[x_pos][y_pos]["organism"] = organism
            index += 1

