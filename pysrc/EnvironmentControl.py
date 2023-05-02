import socket

import Environment_cell
import Global_access
import Organism_cell
from send_message import send_message


def set_fps(conn, fps):
    if Global_access.fps != fps:
        Global_access.fps = fps
        send_message(conn, "control")


def toggle_start_stop(conn):
    Global_access.running = not Global_access.running
    send_message(conn, "control")


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


# TODO fix this up
def square_clicked(event, envVars, conn):
    """
    This will fill in a square when it is clicked in the environment

    :param event: the clicked event used for finding the clicked position
    :param type: event type

    :param envVars: Variables from the environment window
    :param type: setupenvironment instance

    :param conn: connection to send message with
    :param type: socket
    """
    return # this function is broken right now
    mpos_x, mpos_y = event.pos
    # Grid coords are different in python than they are in c++ for y coordinates
    coord = mpos_x // 10, int(Global_access.environment_size[1] - (mpos_y // 10) - 1)

    # Check that the coordinates are within the bounds of the environment (only check height)
    if coord[1] < 0:
        return

    # This block checks if an organism is in the cell and then requests data if it is
    if Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["organism"] is not None:
        org_id = Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["organism"].organism_id
        send_message(conn, "request", org_id)
    # If the cell wasn't filled with an organism we check if it is empty (not food or a wall cell but empty)
    elif Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"] is not None and \
        Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"].tile_type == Global_access.TILE_TYPE_EMPTY:

        # we're probably going to have to send a new frame message instead of trying to prerender what we think it's gonna look like
        # when we place the thing

        if Global_access.CLICK_TYPE == 'Organism':
            self.fill_cell(coord[0], coord[1], Global_access.org_colors[1])
            new_cell = Organism_cell.OrganismCell(0, 0, 0, 0, 0, 0, 0, 0, 0, coord[0], coord[1])
            Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["organism"] = new_cell
            send_message(conn, "new_filled", (coord[0], coord[1], "organism"))
        elif Global_access.CLICK_TYPE == 'Food':
            self.fill_cell(coord[0], coord[1], Global_access.YELLOW)
            new_cell = Environment_cell.EnvironmentCell(0, 0, 0, 0, 0, 0, 0, 0)
            Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"] = new_cell
            send_message(conn, "new_filled", (coord[0], coord[1], "food"))
        else:  # Wall
            self.fill_cell(coord[0], coord[1], Global_access.WHITE)
            new_cell = Environment_cell.EnvironmentCell(0, 0, 0, 0, -1, 0, 0, 0)
            Global_access.ENVIRONMENT_GRID[coord[0]][coord[1]]["environment"] = new_cell
            send_message(conn, "new_filled", (coord[0], coord[1], "wall"))

