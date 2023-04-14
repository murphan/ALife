"""
This file has Global variables and information that can be acessed by the entire application.
It requires access to the mutex in order to update as there are threads in the application
"""

from threading import Thread, Lock
import pygame.display
import tkinter
from tkinter import *

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

running = False
updateDisplay = True

# Used for environment sizing
WINDOW_HEIGHT = 770
WINDOW_WIDTH = 1500
environment_size = 150, 72  # Set these to 0 when ready to make environment dynamically sized
block_height = 10  # TODO: set these to 0 when ready to make environment dynamic sizeable
block_width = 10  # TODO: set these to 0 when ready to make environment dynamic sizeable
size_changed = False

# variables used frequently in the environment
SCREEN = None
CLICK_TYPE = "Organism"
ENVIRONMENT_GRID = []
new_frame = False
second_surface = pygame.Surface([1500, 730])

TILE_TYPE_EMPTY = 0
TILE_TYPE_ORGANISM = 1
TILE_TYPE_FOOD = 2
TILE_TYPE_WALL = 3

# The Mutex needing to be acquired in order to update information
mutex = Lock()

def check_empty():
    global ENVIRONMENT_GRID
    for row in ENVIRONMENT_GRID:
        if any(row):
            return True
    return False


def define_grid(width, height):
    """
    This will set the correct size of the environment window reference grid

    :param width: width of the environment
    :param type: int, float

    :param height: height of the environment
    :param type: int, float
    """
    # TODO: Delete these first three lines when ready to make environment dynamically sized
    global ENVIRONMENT_GRID
    ENVIRONMENT_GRID = [[{"environment": None, "organism": None} for x in range(int(height))] for y in range(int(width))]
    return
    mutex.acquire()
    # global ENVIRONMENT_GRID, WINDOW_WIDTH, WINDOW_HEIGHT, size_changed
    if check_empty():
        temp_environment_grid = ENVIRONMENT_GRID
        ENVIRONMENT_GRID = [[0 for x in range(int(height))] for y in range(int(width))]
        for x in range(len(temp_environment_grid)):
            for y in range(len(temp_environment_grid[0])):
                ENVIRONMENT_GRID[x][y] = temp_environment_grid[x][y]
        mutex.release()
        size_changed = True
        return
    else:
        ENVIRONMENT_GRID = [[0 for x in range(int(height))] for y in range(int(width))]
        mutex.release()
        set_environment_size(width, height)
        set_block_size()


def set_block_size():
    mutex.acquire()
    global block_width, WINDOW_WIDTH, block_height, WINDOW_HEIGHT, environment_size
    block_width = int(WINDOW_WIDTH / environment_size[0])
    block_height = int((WINDOW_HEIGHT - 50) / environment_size[1])
    mutex.release()


def update_grid(x, y):
    mutex.acquire()
    global ENVIRONMENT_GRID
    ENVIRONMENT_GRID[x][y] = 1
    mutex.release()


def change_fps(new_fps):
    mutex.acquire()
    global fps
    fps = new_fps
    mutex.release()


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


def set_screen(screen):
    mutex.acquire()
    global SCREEN
    SCREEN = screen
    mutex.release()


def set_environment_size(width, height):
    mutex.acquire()
    global environment_size
    environment_size = width, height
    mutex.release()
