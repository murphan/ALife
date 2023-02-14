"""
This file has Global variables and information that can be acessed by the entire application.
It requires access to the mutex in order to update as there are threads in the application
"""

from threading import Thread, Lock

import pygame.display

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
speed = 1
temperature = 0
oxygen = 0
light = 0
running = False

CLICK_TYPE = "Organism"
ENVIRONMENT_GRID = []

SCREEN = None
WINDOW_HEIGHT = 770
WINDOW_WIDTH = 1500
environment_size = 150, 72  # Set these to 0 when ready to make environment dynamically sized
block_height = 10  # TODO: set these to 0 when ready to make environment dynamic sizeable
block_width = 10  # TODO: set these to 0 when ready to make environment dynamic sizeable

size_changed = False
new_frame = False

mutex = Lock()

TILE_TYPE = {-2: "Empty",
             -1: "Wall",
             0: "Food 0",
             1: "Food 1",
             2: "Food 2",
             3: "Food 3"}

# Organism Colors
org_colors = {1: '#5573a3',
              2: '#23aac2',
              3: '#700232',
              4: '#9e9403',
              5: '#7ee093',
              6: '#0c871b',
              7: '#03f4fc',
              8: '#b1fc03',
              9: '#fc95be',
              10: '#8a05f7',
              11: '#757171',
              12: '#a1641f',
              13: '#836521',
              14: '#97b09d',
              15: '#61223f',
              16: '#6852f7'
              }


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


def change_speed(new_speed):
    mutex.acquire()
    global speed
    speed = new_speed
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
