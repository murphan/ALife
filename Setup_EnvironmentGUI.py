import pygame
import sys

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

        # The three buttons that are on the environment screen
        self.settings_button = Button(SCREEN, (1424, 738), "Settings", (255, 255, 0))
        self.pause_button = Button(SCREEN, (1364, 738), "Pause", (255, 0, 0))
        self.play_button = Button(SCREEN, (1320, 738), "Play", (0, 255, 0))

        self.environment_grid = []

        while True:
            self.drawGrid()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                if self.settings_button.mouse_click(event):
                    Setup_settingsGUI.SetupSettings()
                elif self.play_button.mouse_click(event):
                    Control_Environment.EnvironmentControl.start(self)
                elif self.pause_button.mouse_click(event):
                    Control_Environment.EnvironmentControl.stop(self)
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    Control_Environment.EnvironmentControl.square_clicked(self, event, SCREEN)
                # TODO: Likely will have to have another call here to update data and position of organisms

            pygame.display.update()

    def drawGrid(self):
        """
        This will draw the grid of the environment and initialize a two-dimensional array
        """
        global SCREEN
        blockSize = 10
        for x in range(0, WINDOW_WIDTH, blockSize):
            for y in range(0, WINDOW_HEIGHT, blockSize):
                if x >= WINDOW_WIDTH - 180 and y >= WINDOW_HEIGHT - 35:
                    continue
                rect = pygame.Rect(x, y, blockSize, blockSize)
                pygame.draw.rect(SCREEN, BLACK, rect, 1)
            self.environment_grid.append([[0]* int(WINDOW_HEIGHT / blockSize)])


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
