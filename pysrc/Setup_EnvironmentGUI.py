import os
from multiprocessing import Process
import sys
import socket
import pygame
import random

import Control_EnvironmentGUI
import Setup_settingsGUI
import Global_access

CLOCK = None


class SetupEnvironment:
    """
    This will set up the actual environment for the simulation to be displayed in
    """
    def __init__(self):
        global CLOCK
        pygame.init()
        Global_access.set_screen(
            pygame.display.set_mode(
                size=(Global_access.WINDOW_WIDTH, Global_access.WINDOW_HEIGHT),
            )
        )
        CLOCK = pygame.time.Clock()
        Global_access.SCREEN.fill(Global_access.BLACK)

        self.settings_button = None
        self.play_pause_button = None

        self.create_buttons()

        # Global_access.set_environment_size(Global_access.WINDOW_WIDTH / 10, (Global_access.WINDOW_HEIGHT - 50) / 10)
        # Global_access.define_grid(Global_access.environment_size[0], Global_access.environment_size[1])

    def create_buttons(self):
        self.play_pause_button = Button(
            Global_access.SCREEN,
            (Global_access.WINDOW_WIDTH - 150, Global_access.WINDOW_HEIGHT - 40, 70, 35),
            "Pause" if Global_access.running else "Play",
            20,
            (255, 0, 0) if Global_access.running else (0, 255, 0),
        )
        self.settings_button = Button(
            Global_access.SCREEN,
            (Global_access.WINDOW_WIDTH - 75, Global_access.WINDOW_HEIGHT - 40, 70, 35),
            "Settings",
            20,
            (255, 255, 0),
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


class Button:
    """
    The button class which defines the buttons in the environment
    """
    def __init__(self, screen, position: (int, int, int, int), text: str, textSize: int, color: (int, int, int)):
        font = pygame.font.SysFont("Arial", textSize)

        rendered_text = font.render(text, True, (0, 0, 0))
        text_rect = rendered_text.get_rect()

        self.check_rect = pygame.Rect(*position)

        x, y, w, h = position

        pygame.draw.rect(screen, color, position)
        screen.blit(source=rendered_text, dest=(x + (w - text_rect[2]) / 2, y + (h - text_rect[3]) / 2))

    def clicked(self):
        """
        Returns True if you clicked on the specified button being checked
        """
        x, y = pygame.mouse.get_pos()
        return True if self.check_rect.collidepoint(x, y) else False

    def mouse_click(self, event):
        """
        Checks if you click the mouse button and then if it's on the button
        """
        if event.type == pygame.MOUSEBUTTONDOWN:
            if pygame.mouse.get_pressed()[0]:
                return True if self.clicked() else False
