import random

import pygame

import Global_access

CLOCK = None


class SetupEnvironment:
    """
    This will set up the actual environment for the simulation to be displayed in
    """
    def __init__(self):
        global CLOCK
        pygame.init()

        SetupEnvironment.set_screen_sizes(Global_access.WINDOW_WIDTH, Global_access.WINDOW_HEIGHT)

        CLOCK = pygame.time.Clock()
        Global_access.SCREEN.fill(Global_access.BLACK)

        self.settings_button = None
        self.play_pause_button = None

        self.create_buttons()

    @staticmethod
    def set_screen_sizes(width: int, height: int):
        Global_access.WINDOW_WIDTH = width
        Global_access.WINDOW_HEIGHT = height

        SetupEnvironment.set_environment_box(width, height)

        # Global_access.new_frame = True
        Global_access.SCREEN = pygame.display.set_mode(
            size=(width, height),
            flags=pygame.RESIZABLE,
        )

    @staticmethod
    def set_environment_box(width: int, height: int):
        grid_width, grid_height = Global_access.environment_size
        Global_access.ENVIRONMENT_BOX = SetupEnvironment.get_bounds(
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
