from multiprocessing import Process
import pygame
import sys
import socket

import Control_EnvironmentGUI as Control_Environment
import Setup_settingsGUI

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
WINDOW_HEIGHT = 770
WINDOW_WIDTH = 1500
SCREEN = None
CLOCK = None


class SetupEnvironment:
    """
    This will set up the actual environment for the simulation to be displayed in
    """
    def __init__(self):
        global SCREEN, CLOCK
        pygame.init()
        SCREEN = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
        CLOCK = pygame.time.Clock()
        SCREEN.fill(WHITE)

        self.createButtons()

        self.environment_size = (WINDOW_WIDTH / 10, (WINDOW_HEIGHT - 50) / 10)
        # Environment factors
        self.click_type = "Organism"
        self.speed = 1

        # TODO: Likely need to create instances of the setup_settings and setup_dataprocessing GUI's
        # So that I can access each of them and pass settings along through pages and changes of states

        # Set up the socket connection to the c++ application
        host = socket.gethostname()
        port = self.read_port()
        self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.conn.connect((host, port))

        proc = Process(target=Control_Environment.EnvironmentControl.decode_message, args=(self,))
        proc.start()

        self.start_main_loop()

    def start_main_loop(self):
        while True:
            self.drawGrid()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                if event.type == pygame.MOUSEBUTTONDOWN:
                    if self.settings_button.mouse_click(event):  # Show the settings window
                        self.open_settings()
                    elif self.play_button.mouse_click(event):  # Register that the start button was clicked
                        Control_Environment.EnvironmentControl.start(self)
                    elif self.pause_button.mouse_click(event):  # Register that the pause button was clicked
                        Control_Environment.EnvironmentControl.stop(self)
                    else:  # Display on the environment that a square was clicked
                        Control_Environment.EnvironmentControl.square_clicked(self, event, SCREEN)

            pygame.display.update()

    def drawGrid(self):
        """
        This will draw the grid of the environment and initialize a two-dimensional array
        """
        global SCREEN
        block_size = 10
        for x in range(0, WINDOW_WIDTH, block_size):
            for y in range(0, WINDOW_HEIGHT - 50, block_size):
                rect = pygame.Rect(x, y, block_size, block_size)
                pygame.draw.rect(SCREEN, BLACK, rect, 1)

    def createButtons(self):
        # The three buttons that are on the environment screen
        self.settings_button = Button(SCREEN, (1420, 738), "Settings", (255, 255, 0))
        self.pause_button = Button(SCREEN, (1356, 738), "Pause", (255, 0, 0))
        self.play_button = Button(SCREEN, (1307, 738), "Play", (0, 255, 0))

    def read_port(self):
        f = open("../config.txt", "r")
        return int(f.read())

    def open_settings(self):
        settings = Setup_settingsGUI.SetupSettings()
        settings.start(self)


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


if __name__ == "__main__":
    SetupEnvironment()
