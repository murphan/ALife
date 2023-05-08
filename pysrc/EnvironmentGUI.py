import random

import pygame

import Global_access
from component.Slider import Slider
from component.Button import Button
from typing import Callable
import boxing


class EnvironmentGUI:
    def __init__(self, set_fps: Callable[[float], None]):
        pygame.init()
        pygame.display.set_caption('ALife Challenge')
        pygame.display.set_icon(pygame.image.load('icon.png'))
        EnvironmentGUI.set_screen_sizes(Global_access.WINDOW_WIDTH, Global_access.WINDOW_HEIGHT)

        Global_access.SCREEN.fill(Global_access.BLACK)

        self.settings_button = Button()
        self.play_pause_button = Button()
        self.fps_slider = Slider(1.0, 21.0, lambda: Global_access.fps, lambda fps: set_fps(fps))
        self.tree_button = Button()
        self.reset_button = Button()

    @staticmethod
    def set_screen_sizes(width: int, height: int):
        Global_access.WINDOW_WIDTH = width
        Global_access.WINDOW_HEIGHT = height

        EnvironmentGUI.set_environment_box()

        # Global_access.new_frame = True
        Global_access.SCREEN = pygame.display.set_mode(
            size=(width, height),
            flags=pygame.RESIZABLE,
        )

    @staticmethod
    def set_environment_box():
        Global_access.ENVIRONMENT_BOX = EnvironmentGUI.get_bounds(
            1.0 if Global_access.grid_width == 0 or Global_access.grid_height == 0 else float(Global_access.grid_width) / Global_access.grid_height,
            Global_access.WINDOW_WIDTH,
            Global_access.WINDOW_HEIGHT - Global_access.BOTTOM_BUFFER,
        )

    @staticmethod
    def get_bounds(aspect: float, window_width: int, window_height: int) -> pygame.Rect:
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

            return pygame.Rect(x, y, w, h)

        # window is too tall
        else:
            w = window_width
            h = int((1 / aspect) * window_width)
            x = 0
            y = (window_height - h) / 2

            return pygame.Rect(x, y, w, h)

    def on_window_resize(self, event: pygame.event):
        self.set_screen_sizes(event.w, event.h)
        if Global_access.latest_frame is not None:
            Global_access.latest_frame["should_render"] = True

    def render_ui(self):
        settings_box = boxing.from_bottom_right(boxing.box(70, 35), 5, 5)
        play_box = boxing.left_of(ref=settings_box, box=boxing.box(70, 35), x=5)
        fps_box = boxing.left_of(ref=play_box, box=boxing.box(210, 35), x=5)
        tree_box = boxing.left_of(ref=fps_box, box=boxing.box(100, 35), x=5)

        reset_box = boxing.from_bottom_left(boxing.box(70, 35), 5, 5)

        self.play_pause_button.render(
            Global_access.SCREEN,
            settings_box,
            "Pause" if Global_access.running else "Play",
            Global_access.ENV_FONT,
            0xe83a54 if Global_access.running else 0x75d943,
        )
        self.settings_button.render(
            Global_access.SCREEN,
            play_box,
            "Settings",
            Global_access.ENV_FONT,
            0xe09e24,
        )
        self.fps_slider.render(
            Global_access.SCREEN,
            'fps:',
            fps_box,
            font=Global_access.ENV_FONT,
            text_area_width=80,
            handle_width=10,
            bar_height=5,
            gutter=(10, 2),
        )
        self.tree_button.render(
            Global_access.SCREEN,
            tree_box,
            'Show Tree' if Global_access.tree is None else 'Hide Tree',
            Global_access.ENV_FONT,
            0x4fbdae,
        )
        self.reset_button.render(
            Global_access.SCREEN,
            reset_box,
            'Reset',
            Global_access.ENV_FONT,
            0xff0000,
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
