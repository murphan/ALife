import json
import socket
import Global_access


def send_message(conn: socket.socket, message_type: str, data=""):
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
    jsonPart = create_message(message_type, data)
    message = len(jsonPart).to_bytes(4, "big") + jsonPart.encode("utf-8")

    conn.send(message)


# TODO split these into separate functions and use dependency injection for send_message
def create_message(message_type, data=None):
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
        return json.dumps({"type": "request_all"})
    elif message_type == "controls":
        return json.dumps({
            "type": "controls",
            "controls": Global_access.controls
        })
    elif message_type == "new_filled":
        return json.dumps({
            "type": "new_filled",
            "data": [{
                "x": str(data[0]),
                "y": str(data[1]),
                "square_type": str(data[2])
            }]
        })
    elif message_type == "settings":
        return json.dumps({
            "type": "settings",
            "settings": {
                "factors": list(map(lambda noise: {
                    "useNoise": noise.use_noise,
                    "center": noise.center,
                    "speed": noise.speed,
                    "scale": noise.scale,
                    "amplitude": noise.depth,
                }, Global_access.noises)),
                "mutations": [{
                    "insertion": Global_access.repro_insertion,
                    "deletion": Global_access.repro_deletion,
                    "substitution": Global_access.repro_substitution,
                }]
            },
        })
    elif message_type == "init":
        return json.dumps({
            "type": "init"
        })
    elif message_type == "tree":
        return json.dumps({
            "type": "tree"
        })
    elif message_type == 'reset':
        return json.dumps({
            "type": "reset"
        })
