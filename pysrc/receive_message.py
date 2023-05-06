import base64
import json

import Global_access


def decode_message(conn):
    """
    This will decode the message that is sent from the c++ application. This will be
    started in setup Environment and will be in its own process. It will loop
    infinitely looking for messages

    :param conn: connection to receive messages from
    :type conn: socket
    """
    while not Global_access.EXIT:
        message_buf = bytes()  # The buffer to append message information to

        header = conn.recv(4)
        if header:
            length = int.from_bytes(header, "big")
            remaining = length

            while remaining > 0:
                message_part = conn.recv(remaining)
                message_buf += message_part
                remaining -= len(message_part)

            process_data(message_buf)


def process_data(message_buf: bytes):
    data_map = json.loads(message_buf)

    message_type = data_map["type"]
    if message_type == "frame":
        handle_environment_data(data_map["environment"])

    elif message_type == "init":
        handle_environment_data(data_map["environment"])
        handle_control_data(data_map["control"])
        handle_settings_data(data_map["settings"])

    elif message_type == "organism_data":
        print("organism data received")

    elif message_type == "control":
        handle_control_data(data_map["control"])

    elif message_type == "settings":
        handle_settings_data(data_map["settings"])

    elif message_type == "tree":
        handle_tree_data(data_map["tree"])


def handle_tree_data(tree):
    Global_access.tree = tree
    Global_access.set_should_render(True)


def handle_environment_data(environment_data_map):
    height = environment_data_map["height"]
    width = environment_data_map["width"]

    Global_access.latest_frame = {
        "size": (width, height),
        "buffer": base64.b64decode(environment_data_map["grid"]),
        "uuids": environment_data_map["organisms"],
        "should_render": True,
    }


def handle_control_data(control):
    """
    update internal control values to match what the backend says is correct
    do not send a message back
    """
    Global_access.fps = control["fps"]
    Global_access.updateDisplay = control["updateDisplay"]
    Global_access.running = control["playing"]


def handle_settings_data(settings):
    factors = settings["factors"]

    for factor in range(3):
        noise = Global_access.noises[factor]
        noise.use_noise = factors[factor]["useNoise"]
        noise.center = factors[factor]["center"]
        noise.speed = factors[factor]["speed"]
        noise.depth = factors[factor]["amplitude"]
        noise.scale = factors[factor]["scale"]
