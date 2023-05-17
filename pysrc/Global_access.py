"""
This file has Global variables and information that can be acessed by the entire application.
It requires access to the mutex in order to update as there are threads in the application
"""

import pygame

import Drawing
from EnvironmentGUI import EnvironmentGUI
from Noise import Noise

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
noises: list[Noise] = [
    Noise(False, 0.0, 0.0, 0.0, 0.0),
]

repro_insertion = 0.0
repro_deletion = 0.0
repro_substitution = 0.0

DISPLAY_MODE_ENVIRONMENT = 0
DISPLAY_MODE_TREE = 1

SELECT_MODE_SINGLE = 0
SELECT_MODE_LINEAGE = 1

ControlsType = {
    "playing": bool,
    'fps': int,
    'updateDisplay': bool,
    'displayMode': int,
    'smartTree': bool,
    'activeNode': int,
    'doHighlight': bool,
    'selectMode': int,
}

controls: ControlsType = {
    "playing": False,
    "fps": 0,
    "updateDisplay": True,
    "displayMode": DISPLAY_MODE_ENVIRONMENT,
    "smartTree": False,
    'activeNode': None,
    'doHighlight': False,
    'selectMode': SELECT_MODE_SINGLE,
}


class TreeNode(object):
    def __init__(self, rect: pygame.Rect, uuid: int, active: bool):
        self.rect = rect
        self.uuid = uuid
        self.active = active


tree_nodes: list[TreeNode]

# Used for environment sizing
WINDOW_HEIGHT = 770
WINDOW_WIDTH = 1500
ENVIRONMENT_BOX = pygame.Rect(0, 0, 0, 0)
TREE_BOX = pygame.Rect(0, 0, 0, 0)
grid_width = 0
grid_height = 0

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

ENV_FONT: pygame.font.Font

# how much we leave for ui at the bottom of the screen
BOTTOM_BUFFER = 75


def define_grid(width: int, height: int):
    """
    This wipes the environment grid, that's ok because we whenever we do this, c++
    will then be able to fill it for us

    :param width: width of the environment
    :param type: int

    :param height: height of the environment
    :param type: int
    """
    global grid_width, grid_height, ENVIRONMENT_GRID

    # no need to reallocate memory
    if grid_width == width and grid_height == height:
        return

    ENVIRONMENT_GRID = [
        [
            {"environment": None, "organism": None}
            for _ in range(height)
        ]
        for _ in range(width)
    ]

    grid_width = width
    grid_height = height
    EnvironmentGUI.set_environment_box()


def set_should_render(should_render: bool):
    if latest_frame is not None:
        latest_frame["should_render"] = should_render


def update_grid(x, y):
    global ENVIRONMENT_GRID
    ENVIRONMENT_GRID[x][y] = 1


def change_click_type(new_click_type):
    global CLICK_TYPE
    CLICK_TYPE = new_click_type


def set_insertion(rate):
    global repro_insertion
    repro_insertion = rate


def set_deletion(rate):
    global repro_deletion
    repro_deletion = rate


def set_substitution(rate):
    global repro_substitution
    repro_substitution = rate
