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
        Global_access.set_screen(pygame.display.set_mode((Global_access.WINDOW_WIDTH, Global_access.WINDOW_HEIGHT)))
        CLOCK = pygame.time.Clock()
        Global_access.SCREEN.fill(Global_access.WHITE)

        self.createButtons()
        Global_access.set_environment_size(Global_access.WINDOW_WIDTH / 10, (Global_access.WINDOW_HEIGHT - 50) / 10)
        Global_access.define_grid(Global_access.environment_size[0], Global_access.environment_size[1])

    def drawGrid(self):
        """
        This will draw the grid of the environment
        """
        self.clear_screen()
        block_width = Global_access.block_width
        block_height = Global_access.block_height

        # Ensure that there is an actual size that we have been given before rendering anything on the screen
        if not block_height or not block_width:
            return

        for x in range(0, Global_access.WINDOW_WIDTH, block_width):
            for y in range(0, Global_access.WINDOW_HEIGHT - 50, block_height):
                rect = pygame.Rect(x, y, block_width, block_height)
                pygame.draw.rect(Global_access.SCREEN, Global_access.BLACK, rect, 1)

    def clear_screen(self):
        """
        This will just clear the window for when the environment changes sizes and redraws the grid
        """
        if Global_access.size_changed:
            Global_access.SCREEN.fill(Global_access.WHITE)
            self.createButtons()
            Global_access.size_changed = False

    def createButtons(self):
        # The three buttons that are on the environment screen
        self.settings_button = Button(Global_access.SCREEN, (1420, 730), "Settings", (255, 255, 0))
        self.pause_button = Button(Global_access.SCREEN, (1356, 730), "Pause", (255, 0, 0))
        self.play_button = Button(Global_access.SCREEN, (1307, 730), "Play", (0, 255, 0))

    def add_organism_display(self, organism):
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
    def __init__(self, screen, position, text, color):
        font = pygame.font.SysFont("Arial", 25)
        text_options = font.render(text, True, (0, 0, 0))
        x, y, w, h = text_options.get_rect()
        x, y = position
        self.button_rect = pygame.draw.rect(screen, color, (x, y, w, h))
        screen.blit(text_options, self.button_rect)

    def clicked(self):
        """
        Returns True if you clicked on the specified button being checked
        """
        x, y = pygame.mouse.get_pos()
        return True if self.button_rect.collidepoint(x, y) else False

    def mouse_click(self, event):
        """
        Checks if you click the mouse button and then if it's on the button
        """
        if event.type == pygame.MOUSEBUTTONDOWN:
            if pygame.mouse.get_pressed()[0]:
                return True if self.clicked() else False
