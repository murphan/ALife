import socket

import pygame

import Global_access
from send_message import send_message_full
from send_message import send_message


def set_fps(conn: socket.socket, fps: int):
    if Global_access.controls['fps'] != fps:
        Global_access.controls['fps'] = fps
        send_message(conn, "controls")


def toggle_start_stop(conn: socket.socket):
    Global_access.controls['playing'] = not Global_access.controls['playing']
    send_message(conn, "controls")


def toggle_display_mode(conn: socket.socket):
    Global_access.controls['displayMode'] = Global_access.DISPLAY_MODE_ENVIRONMENT if \
        Global_access.controls['displayMode'] == Global_access.DISPLAY_MODE_TREE else Global_access.DISPLAY_MODE_TREE
    send_message(conn, "controls")


def set_active_node(conn: socket.socket, uuid: int | None):
    Global_access.controls['activeNode'] = uuid
    send_message(conn, "controls")


def toggle_do_highlight(conn: socket.socket):
    Global_access.controls['doHighlight'] = not Global_access.controls['doHighlight']
    send_message_full(conn, {
        "type": "controls",
        "controls": {
            "doHighlight": Global_access.controls['doHighlight']
        }
    })


def toggle_select_mode(conn: socket.socket):
    Global_access.controls['selectMode'] = Global_access.SELECT_MODE_LINEAGE if Global_access.controls['selectMode'] == Global_access.SELECT_MODE_SINGLE else Global_access.SELECT_MODE_SINGLE
    send_message_full(conn, {
        "type": "controls",
        "controls": {
            "selectMode": Global_access.controls['selectMode']
        }
    })


def click_type(clicked_type):
    """
    When a square is clicked in the environment, the user can
    specify what they want it to be

    :param clicked_type: The type of square the user will fill with
    :param type: String
    """
    Global_access.change_click_type(clicked_type)


def set_mutations(conn: socket.socket, insertion, deletion, substitution):
    if insertion != "":
        Global_access.set_insertion(float(insertion))
    else:
        Global_access.set_insertion(0.0)

    if deletion != "":
        Global_access.set_deletion(float(deletion))
    else:
        Global_access.set_deletion(0.0)

    if substitution != "":
        Global_access.set_substitution(float(substitution))
    else:
        Global_access.set_substitution(0.0)

    send_message(conn, "settings")


def click_tree(conn: socket.socket, event: pygame.event):
    if event.type == pygame.MOUSEBUTTONDOWN and pygame.mouse.get_pressed()[0]:
        x, y = pygame.mouse.get_pos()

        for node in Global_access.tree_nodes:
            if node.rect.collidepoint(x, y):
                if node.active:
                    set_active_node(conn, None)
                else:
                    set_active_node(conn, node.uuid)

                return


# TODO fix this up
def square_clicked(event, conn):
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
    cell_width = Global_access.ENVIRONMENT_BOX.width / Global_access.grid_width
    cell_height = Global_access.ENVIRONMENT_BOX.height / Global_access.grid_height

    # Check that the coordinates are within the bounds of the environment
    if Global_access.ENVIRONMENT_BOX.x < mpos_x < Global_access.WINDOW_WIDTH - Global_access.ENVIRONMENT_BOX.x:
        if mpos_y < Global_access.WINDOW_HEIGHT - Global_access.BOTTOM_BUFFER:
            x_coord = int((mpos_x - Global_access.ENVIRONMENT_BOX.x) / cell_width)
            # Adjust for the grid actually starting in the bottom left, not the top left
            y_coord = abs(int((mpos_y / cell_height)) - Global_access.grid_height) - 1
            org_id = Global_access.ENVIRONMENT_GRID[x_coord][y_coord]["organism"]
            if org_id is not None:
                send_message_full(conn, {
                    "type": "controls",
                    "controls": {
                        "activeNode": org_id
                    }
                })

