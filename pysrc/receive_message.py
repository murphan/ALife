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
        handle_frame_data(data_map["frame"])

    elif message_type == "init":
        handle_frame_data(data_map["frame"])
        handle_control_data(data_map["controls"])
        handle_settings_data(data_map["settings"])

    elif message_type == "organism_data":
        print("organism data received")

    elif message_type == "controls":
        handle_control_data(data_map["controls"])
        if 'frame' in data_map:
            handle_frame_data(data_map["frame"])

    elif message_type == "settings":
        handle_settings_data(data_map["settings"])

    elif message_type == "tree":
        handle_tree_data(data_map["tree"])


def handle_tree_data(tree):
    Global_access.tree = tree
    Global_access.set_should_render(True)


def handle_frame_data(frame_data_map):
    height = frame_data_map["height"]
    width = frame_data_map["width"]

    latest_frame = {
        "size": (width, height),
        "ids": frame_data_map["organisms"],
        "should_render": True,
    }

    if 'grid' in frame_data_map:
        latest_frame['grid'] = base64.b64decode(frame_data_map['grid'])
    elif 'tree' in frame_data_map:
        latest_frame['tree'] = frame_data_map['tree']

    Global_access.latest_frame = latest_frame


def handle_control_data(controls):
    """
    update internal control values to match what the backend says is correct
    do not send a message back
    """
    Global_access.controls = controls
    Global_access.set_should_render(True)


def handle_settings_data(settings):
    factors = settings["factors"]

    for factor in range(1):
        noise = Global_access.noises[factor]
        noise.use_noise = factors[factor]["useNoise"]
        noise.center = factors[factor]["center"]
        noise.speed = factors[factor]["speed"]
        noise.depth = factors[factor]["amplitude"]
        noise.scale = factors[factor]["scale"]
