import random

import pygame

import Global_access
from component.Slider import Slider
from component.Button import Button
from typing import Callable


class EnvironmentGUI:
    def __init__(self, set_fps: Callable[[float], None]):
        pygame.init()
        pygame.display.set_caption('ALife Challenge')

        EnvironmentGUI.set_screen_sizes(Global_access.WINDOW_WIDTH, Global_access.WINDOW_HEIGHT)

        Global_access.SCREEN.fill(Global_access.BLACK)

        self.settings_button = Button()
        self.play_pause_button = Button()
        self.fps_slider = Slider(1.0, 21.0, lambda: Global_access.fps, lambda fps: set_fps(fps))

    @staticmethod
    def set_screen_sizes(width: int, height: int):
        Global_access.WINDOW_WIDTH = width
        Global_access.WINDOW_HEIGHT = height

        EnvironmentGUI.set_environment_box(width, height)

        # Global_access.new_frame = True
        Global_access.SCREEN = pygame.display.set_mode(
            size=(width, height),
            flags=pygame.RESIZABLE,
        )

    @staticmethod
    def set_environment_box(width: int, height: int):
        grid_width, grid_height = Global_access.environment_size
        Global_access.ENVIRONMENT_BOX = EnvironmentGUI.get_bounds(
            1.0 if grid_width == 0 or grid_height == 0 else float(grid_width) / grid_height,
            width,
            height,
        )

    @staticmethod
    def get_bounds(aspect: float, window_width: int, window_height: int) -> (int, int, int, int):
        """
        produces the largest, centered, box of aspect ratio :param aspect (width / height)
        that fits within :param window_width * :param window_height
        """
        window_aspect = float(window_width) / window_height

        # window is too wide
        if window_aspect > aspect:
            h = window_height
            w = int(aspect * window_height)
            y = 0
            x = (window_width - w) / 2

            return x, y, w, h

        # window is too tall
        else:
            w = window_width
            h = int((1 / aspect) * window_width)
            x = 0
            y = (window_height - h) / 2

            return x, y, w, h

    def on_window_resize(self, event: pygame.event):
        self.set_screen_sizes(event.w, event.h)
        if Global_access.latest_frame is not None:
            Global_access.latest_frame["should_render"] = True

    def render_ui(self):
        self.play_pause_button.render(
            Global_access.SCREEN,
            pygame.Rect(Global_access.WINDOW_WIDTH - 150, Global_access.WINDOW_HEIGHT - 40, 70, 35),
            "Pause" if Global_access.running else "Play",
            Global_access.ENV_FONT,
            0xe83a54 if Global_access.running else 0x75d943,
        )
        self.settings_button.render(
            Global_access.SCREEN,
            pygame.Rect(Global_access.WINDOW_WIDTH - 75, Global_access.WINDOW_HEIGHT - 40, 70, 35),
            "Settings",
            Global_access.ENV_FONT,
            0xe09e24,
        )
        self.fps_slider.render(
            Global_access.SCREEN,
            'fps:',
            pygame.Rect(Global_access.WINDOW_WIDTH - 150 - 5 - 210, Global_access.WINDOW_HEIGHT - 40, 210, 35),
            font=Global_access.ENV_FONT,
            text_area_width=80,
            handle_width=10,
            bar_height=5,
            gutter=(10, 2),
        )

    @staticmethod
    def add_organism_display(organism):
        """
        This will display the formatted string of organism information at the bottom of the screen

        :param organism: organism structure to pull information from
        :param type: organism type
        """

        test_string = f"ID: {random.randint(111111111, 999999999)}  \
          Body = U2FsdGVkX1/DX9ztNOyA+E0ztNTyMtLx/bKrIzZlN/E=  \
          width = 5  \
          height = 5  \
          energy = 100  \
          age = 122  \
          x = 54  \
          y = 81"

        font = pygame.font.SysFont("freesansbold.ttf", 22)
        text_options = font.render(test_string, True, (0, 0, 0))
        textRect = text_options.get_rect()
        textRect.center = (670, 745)
        Global_access.SCREEN.blit(text_options, textRect)
