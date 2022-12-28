import pygame
import sys

import Control_EnvironmentGUI as Control_Environment

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
WINDOW_HEIGHT = 770
WINDOW_WIDTH = 1500
SCREEN = None
CLOCK = None

class SetupEnvironment:
    def __init__(self):
        global SCREEN, CLOCK
        pygame.init()
        SCREEN = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
        CLOCK = pygame.time.Clock()
        SCREEN.fill(WHITE)

        self.settings_button = Button(SCREEN, (1424, 738), "Settings", (255, 255, 0))
        self.pause_button = Button(SCREEN, (1364, 738), "Pause", (255, 0, 0))
        self.play_button = Button(SCREEN, (1320, 738), "Play", (0, 255, 0))

        while True:
            self.drawGrid()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                if self.settings_button.mouse_click(event):
                    Control_Environment.EnvironmentControl.settings_window(self)
                elif self.play_button.mouse_click(event):
                    Control_Environment.EnvironmentControl.start(self)
                elif self.pause_button.mouse_click(event):
                    Control_Environment.EnvironmentControl.stop(self)

            pygame.display.update()

    def drawGrid(self):
        global SCREEN
        blockSize = 10
        for x in range(0, WINDOW_WIDTH, blockSize):
            for y in range(0, WINDOW_HEIGHT, blockSize):
                if x >= WINDOW_WIDTH - 180 and y >= WINDOW_HEIGHT - 35:
                    continue
                rect = pygame.Rect(x, y, blockSize, blockSize)
                pygame.draw.rect(SCREEN, BLACK, rect, 1)


class Button:
    def __init__(self, screen, position, text, color):
        font = pygame.font.SysFont("Arial", 25)
        text_options = font.render(text, True, (0, 0, 0))
        x, y, w, h = text_options.get_rect()
        x, y = position
        self.rect = pygame.draw.rect(screen, color, (x, y, w, h))
        screen.blit(text_options, self.rect)

    def clicked(self):
        """ Returns True if you clicked on the specified button """
        x, y = pygame.mouse.get_pos()
        return True if self.rect.collidepoint(x, y) else False

    def mouse_click(self, event):
        """
        checks if you click the mouse button and then if it's on the button
        """
        if event.type == pygame.MOUSEBUTTONDOWN:
            if pygame.mouse.get_pressed()[0]:
                return True if self.clicked() else False


if __name__ == "__main__":
    SetupEnvironment()
