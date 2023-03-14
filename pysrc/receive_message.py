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


def handle_environment_data(self, environment):
    height = environment["height"]
    width = environment["width"]
    # Global_access.define_grid(width, height)   Uncomment when making grid dynamically sized
    environment["grid"] = base64.b64decode(environment["grid"])
    environment["grid"] = [environment["grid"][i: i + 10] for i in range(0, len(environment["grid"]), 10)]
    decode_grid(self, environment["grid"], width, height)

    draw_factors_grid(self, width, height)
    draw_food_walls(self, width, height)

    decode_organisms(self, environment["organisms"])
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

    # ok this is just a massive pain, like
    # this data should be reallllly be grouped into a struct and then into an array (like how it is in the message)
    # to avoid this stupid code duplication
    # also why the hell is using noise internally stored as an int when it is always used like a boolean
    noise =factors[0]
    Global_access.temp_noise = int(noise["useNoise"])
    Global_access.temperature = noise["center"]
    Global_access.temp_speed = noise["speed"]
    Global_access.temp_scale = noise["scale"]
    Global_access.temp_depth = noise["amplitude"]

    # in general, if you find yourself making a set of identical variables that are all prefixed by something
    # then you should put them into a dictionary (or whatever you python people call it) named that prefix
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


def decode_grid(self, grid_data, width, height):
    """
    This will decode the grid that is passed from c++

    :param self: instance of setup_environment self that can be passed but isn't used
    :type self: setup_environment instance

    :param grid_data: the list of grid information
    :type grid_data: list

    :param width: width of the grid
    :type width: int

    :param height: height of the grid
    :type height: int
    """
    for y in range(height):
        for x in range(width):
            data = grid_data[y * width + x]

            factor_0    = int.from_bytes(data[:1], "big", signed=True)
            factor_1    = int.from_bytes(data[1:2], "big", signed=True)
            factor_2    = int.from_bytes(data[2:3], "big", signed=True)
            tile_type   = int.from_bytes(data[3:4], "big", signed=True)
            food_energy = int.from_bytes(data[4:5], "big", signed=True)
            food_age    = int.from_bytes(data[5:9], "big", signed=True)
            pressure    = int.from_bytes(data[9:], "big", signed=True)

            cell = Environment_cell.EnvironmentCell(factor_0, factor_1, factor_2, tile_type, food_energy, food_age, pressure)

            Global_access.ENVIRONMENT_GRID[x][y]["environment"] = cell
            Global_access.ENVIRONMENT_GRID[x][y]["organism"] = None


def draw_factors_grid(self, width, height):
    pixel_array = pygame.PixelArray(Global_access.factors_surface)

    for y in range(height):
        for x in range(width):
            cell = Global_access.ENVIRONMENT_GRID[x][y]["environment"]

            pixel_array[x, y] = (cell.factor_0 + 128, cell.factor_1 + 128, cell.factor_2 + 128)

    pixel_array.close()

    Global_access.second_surface.blit(pygame.transform.smoothscale(Global_access.factors_surface, (width * 10, height * 10)), (0, 0))


def draw_food_walls(self, width, height):
    for y in range(height):
        for x in range(width):
            cell = Global_access.ENVIRONMENT_GRID[x][y]["environment"]

            if cell.tile_type == -1:
                Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x, y, Global_access.WHITE)
            elif cell.tile_type in Global_access.food_colors:
                Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x, y, Global_access.food_colors[cell.tile_type], True)


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

    for j in range(organism.down, organism.up + 1):
        for i in range(organism.left, organism.right + 1):
            y_pos = organism.y + j
            x_pos = organism.x + i

            body_cell_num = body[index]
            if body_cell_num > 0:
                Control_EnvironmentGUI.EnvironmentControl.fill_cell(self, x_pos, y_pos,
                                                                Global_access.org_colors[body_cell_num])
                Global_access.ENVIRONMENT_GRID[x_pos][y_pos]["organism"] = organism
            index += 1

