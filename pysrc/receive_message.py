import base64
import ast
import random

import Global_access
import Control_EnvironmentGUI
import Environment_cell
import Organism_cell

TILE_TYPE = {-2: "Empty",
             -1: "Wall",
             0: "Food 0",
             1: "Food 1",
             2: "Food 2",
             3: "Food 3"}

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
                height = data_map["height"]
                width = data_map["width"]
                # Global_access.define_grid(width, height)   Uncomment when making grid dynamically sized
                data_map["grid"] = base64.b64decode(data_map["grid"])
                data_map["grid"] = [data_map["grid"][i: i +11] for i in range(0, len(data_map["grid"]), 11)]
                decode_grid(data_map["grid"], width, height)
                decode_organisms(self, data_map["organisms"])


def decode_grid(grid_data, width, height):
    index = 0
    length = len(grid_data)
    while index < length:
        for x in range(width):
            for y in range(height):
                data = grid_data[index]
                factor_0 = int.from_bytes(data[:1], "big")
                factor_1 = int.from_bytes(data[1:2], "big")
                factor_2 = int.from_bytes(data[2:3], "big")
                factor_3 = int.from_bytes(data[3:4], "big")
                tile_type = int.from_bytes(data[4:5], "big")
                food_energy = int.from_bytes(data[5:6], "big")
                food_age = int.from_bytes(data[6:9], "big")
                pressure = int.from_bytes(data[9:], "big")
                Global_access.ENVIRONMENT_GRID[x][y]["environment"] = Environment_cell.EnvironmentCell(factor_0, factor_1, factor_2,
                                                                                        factor_3, tile_type,
                                                                                        food_energy, food_age, pressure)
                index += 1


def decode_organisms(self, organisms):
    for each in organisms:
        organism = Organism_cell.OrganismCell(each["age"], each["body"], each["centerX"], each["centerY"], each["energy"],
                                   each["height"], each["id"], each["rotation"], each["width"], each["x"], each["y"])
        Global_access.ENVIRONMENT_GRID[organism.x][organism.y]["organism"] = organism
        Control_EnvironmentGUI.EnvironmentControl.fill_cell(self,
                                                            organism.x,
                                                            organism.y)
