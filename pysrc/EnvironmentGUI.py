import pygame

import Global_access
import boxing
from Drawing import blackout_message_area
from component.Button import Button
from component.Slider import Slider

ORGANISM_TO_DISPLAY = None

class EnvironmentGUI:
    def __init__(self):
        pygame.init()
        pygame.display.set_caption('ALife Challenge')
        pygame.display.set_icon(pygame.image.load('icon.png'))
        EnvironmentGUI.set_screen_sizes(Global_access.WINDOW_WIDTH, Global_access.WINDOW_HEIGHT)

        Global_access.SCREEN.fill(Global_access.BLACK)

        self.settings_button = Button()
        self.play_pause_button = Button()
        self.fps_slider = Slider(1.0, 21.0)
        self.tree_button = Button()
        self.reset_button = Button()
        self.select_mode_button = Button()
        self.do_highlight_button = Button()

    @staticmethod
    def set_screen_sizes(width: int, height: int):
        Global_access.WINDOW_WIDTH = width
        Global_access.WINDOW_HEIGHT = height

        EnvironmentGUI.set_environment_box()

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
        Global_access.TREE_BOX = pygame.Rect(
            10,
            10,
            Global_access.WINDOW_WIDTH - 20,
            Global_access.WINDOW_HEIGHT - Global_access.BOTTOM_BUFFER - 10,
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
        do_highlight_box = boxing.left_of(ref=tree_box, box=boxing.box(100, 35), x=5)
        select_mode_box = boxing.left_of(ref=do_highlight_box, box=boxing.box(100, 35), x=5)

        reset_box = boxing.from_bottom_left(boxing.box(70, 35), 5, 5)

        self.play_pause_button.render(
            Global_access.SCREEN,
            settings_box,
            "Pause" if Global_access.controls['playing'] else "Play",
            Global_access.ENV_FONT,
            0xe83a54 if Global_access.controls['playing'] else 0x75d943,
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
            value=Global_access.controls['fps'],
        )
        self.tree_button.render(
            Global_access.SCREEN,
            tree_box,
            'Show Tree' if Global_access.controls['displayMode'] == Global_access.DISPLAY_MODE_ENVIRONMENT else 'Show Grid',
            Global_access.ENV_FONT,
            0x4fbdae if Global_access.controls['displayMode'] == Global_access.DISPLAY_MODE_ENVIRONMENT else 0x6acc43,
        )
        self.do_highlight_button.render(
            Global_access.SCREEN,
            do_highlight_box,
            'Highlight' if Global_access.controls['doHighlight'] else 'No Highlight',
            Global_access.ENV_FONT,
            0xebdc9b if Global_access.controls['doHighlight'] else 0x696143,
        )
        self.select_mode_button.render(
            Global_access.SCREEN,
            select_mode_box,
            'Select: Single' if Global_access.controls['selectMode'] == Global_access.SELECT_MODE_SINGLE else 'Select: Lineage',
            Global_access.ENV_FONT,
            0x542ee8 if Global_access.controls['selectMode'] == Global_access.SELECT_MODE_SINGLE else 0x6b18c9,
        )

        self.reset_button.render(
            Global_access.SCREEN,
            reset_box,
            'Reset',
            Global_access.ENV_FONT,
            0xff0000,
        )

        global ORGANISM_TO_DISPLAY
        EnvironmentGUI.display_organism_data(ORGANISM_TO_DISPLAY)

    @staticmethod
    def set_organism_to_display(organism):
        global ORGANISM_TO_DISPLAY
        ORGANISM_TO_DISPLAY = organism
        EnvironmentGUI.display_organism_data(organism)

    @staticmethod
    def display_organism_data(organism):
        """
        This will display the formatted string of organism information at the bottom of the screen

        :param organism: organism structure to pull information from
        :param type: organism type
        """
        if organism is not None:
            blackout_message_area()
            general_string_1 = f"Mutation Modifiers: {organism['mutationModifiers'][0]}    "\
                               f"Num Cells Alive: {organism['numAliveCells']}    "\
                               f"Move Length: {organism['moveLength']}    "\
                               f"Body Energy: {organism['baseBodyEnergy']}  "\
                               f"Reactions: {organism['reactions']}"

            font = Global_access.ENV_FONT
            if len(general_string_1) > 100:
                general_string_2 = general_string_1[100:]
                general_string_1 = general_string_1[:100]
                general_info_1 = font.render(general_string_1, True, (255, 255, 255))
                generalRect_1 = general_info_1.get_rect()
                generalRect_1.center = (Global_access.WINDOW_WIDTH / 2 - 150, Global_access.WINDOW_HEIGHT - 60)
                Global_access.SCREEN.blit(general_info_1, generalRect_1)
                if len(general_string_2) > 100:
                    general_string_3 = general_string_2[100:]
                    general_string_2 = general_string_2[:100]
                    general_info_2 = font.render(general_string_2, True, (255, 255, 255))
                    general_info_3 = font.render(general_string_3, True, (255, 255, 255))
                    generalRect_2 = general_info_2.get_rect()
                    generalRect_2.center = (Global_access.WINDOW_WIDTH / 2 - 150, Global_access.WINDOW_HEIGHT - 38)
                    Global_access.SCREEN.blit(general_info_2, generalRect_2)
                    generalRect_3 = general_info_3.get_rect()
                    generalRect_3.center = (Global_access.WINDOW_WIDTH / 2 - 150, Global_access.WINDOW_HEIGHT - 16)
                    Global_access.SCREEN.blit(general_info_3, generalRect_3)
                else:
                    general_info_2 = font.render(general_string_2, True, (255, 255, 255))
                    generalRect_2 = general_info_2.get_rect()
                    generalRect_2.center = (Global_access.WINDOW_WIDTH / 2 - 150, Global_access.WINDOW_HEIGHT - 38)
                    Global_access.SCREEN.blit(general_info_2, generalRect_2)
            else:
                general_info_1 = font.render(general_string_1, True, (255, 255, 255))
                generalRect_1 = general_info_1.get_rect()
                generalRect_1.center = (Global_access.WINDOW_WIDTH / 2 - 150, Global_access.WINDOW_HEIGHT - 60)
                Global_access.SCREEN.blit(general_info_1, generalRect_1)

