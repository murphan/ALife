"""
This file has Global variables and information that can be acessed by the entire application.
It requires access to the mutex in order to update as there are threads in the application
"""

from threading import Lock
import Drawing
import pygame

from pysrc.Setup_EnvironmentGUI import SetupEnvironment

# This is only set to be 1 when the program is exiting. Do not use unless ending execution
EXIT = 0

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = '#0c871b'
YELLOW = '#fafa16'
ORANGE = '#fa6c07'
RED = '#d10a0a'
PINK = '#f00ce1'

# Environment factors
fps = 1
temperature = 0  # If noise isn't used, this will be set
temp_noise = 0  # value for if temp noise is used or not
temp_scale = 0.0
temp_depth = 0.0
temp_speed = 0.0

oxygen = 0  # If noise isn't used, this will be set
oxygen_noise = 0  # value for if oxygen noise is used or not
oxygen_scale = 0.0
oxygen_depth = 0.0
oxygen_speed = 0.0

light = 0  # If noise isn't used, this will be set
light_noise = 0  # value for if light noise is used or not
light_scale = 0.0
light_depth = 0.0
light_speed = 0.0

repro_insertion = 0.0
repro_deletion = 0.0
repro_substitution = 0.0

running = False
updateDisplay = True

# Used for environment sizing
WINDOW_HEIGHT = 770
WINDOW_WIDTH = 1500
ENVIRONMENT_BOX: (int, int, int, int) = None
environment_size = 0, 0

# variables used frequently in the environment
SCREEN = None
CLICK_TYPE = "Organism"
ENVIRONMENT_GRID = []
latest_frame: Drawing.RenderInfo = None

TILE_TYPE_EMPTY = 0
TILE_TYPE_ORGANISM = 1
TILE_TYPE_FOOD = 2
TILE_TYPE_WALL = 3

BUFFER_SIZE = 4096

# The Mutex needing to be acquired in order to update information
mutex = Lock()


def define_grid(width: int, height: int):
    """
    This wipes the environment grid, that's ok because we whenever we do this, c++
    will then be able to fill it for us

    :param width: width of the environment
    :param type: int

    :param height: height of the environment
    :param type: int
    """
    global environment_size, ENVIRONMENT_GRID, WINDOW_WIDTH, WINDOW_HEIGHT, SCREEN

    # no need to reallocate memory
    if environment_size[0] == width and environment_size[1] == height:
        return

    ENVIRONMENT_GRID = [
        [
            {"environment": None, "organism": None}
            for _ in range(int(height))
        ]
        for _ in range(int(width))
    ]

    environment_size = width, height
    SetupEnvironment.set_environment_box(WINDOW_WIDTH, WINDOW_HEIGHT)


def update_grid(x, y):
    mutex.acquire()
    global ENVIRONMENT_GRID
    ENVIRONMENT_GRID[x][y] = 1
    mutex.release()


def change_fps(new_fps):
    global fps
    fps = new_fps


def change_temperature(new_temp):
    mutex.acquire()
    global temperature
    temperature = new_temp
    mutex.release()


def change_light(new_light):
    mutex.acquire()
    global light
    light = new_light
    mutex.release()


def change_oxygen(new_oxygen):
    mutex.acquire()
    global oxygen
    oxygen = new_oxygen
    mutex.release()


def change_click_type(new_click_type):
    mutex.acquire()
    global CLICK_TYPE
    CLICK_TYPE = new_click_type
    mutex.release()


def set_environment_size(width, height):
    mutex.acquire()
    global environment_size
    environment_size = width, height
    mutex.release()


def set_insertion(rate):
    mutex.acquire()
    global repro_insertion
    repro_insertion = rate
    mutex.release()


def set_deletion(rate):
    mutex.acquire()
    global repro_deletion
    repro_deletion = rate
    mutex.release()


def set_substitution(rate):
    mutex.acquire()
    global repro_substitution
    repro_substitution = rate
    mutex.release()
