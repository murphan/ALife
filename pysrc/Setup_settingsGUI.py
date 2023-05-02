import socket
import tkinter
import tkinter.font as font
from tkinter import *

import Global_access
import Setup_DataProcessingGUI
import EnvironmentControl
from pysrc.Noise import Factor
from send_message import send_message

LIGHT_GREEN = '#5fad75'
ORANGE_YELLOW = '#e8b320'
GREEN = '#0c871b'
BLUE = '#3289a8'

CLICK_TYPES = [
    "Wall",
    "Organism",
    "Food"
]

# These are place holder colors and descriptions
CELL_DESCRIPTIONS = {
    "#f2960c": "Mouth cell",
    "#11f0e8": "Mover cell",
    "#086603": "Photosynthesis cell",
    "#e31045": "Weapon cell",
    "#4e2ba6": "Shield cell",
    "#74c2e3": "Eye cell",
    "#ede6da": "Scaffolding cell"
}


class NoiseControl(object):
    def __init__(
        self,
        use_noise_checkbox: tkinter.Checkbutton,
        center_slider: tkinter.Scale,
        scale_slider: tkinter.Scale,
        depth_slider: tkinter.Scale,
        speed_slider: tkinter.Scale
    ):
        self.use_noise_checkbox = use_noise_checkbox
        self.center_slider = center_slider
        self.scale_slider = scale_slider
        self.depth_slider = depth_slider
        self.speed_slider = speed_slider


class SetupSettings:
    """
    This will set up the settings window for manipulating different options in Environment
    """
    def __init__(self, conn: socket.socket):
        self.conn = conn
        self.window = self.setup_window()
        self.noise_controls: list[NoiseControl] = []
        self.setup_banner()
        self.setup_configurations()
        self.setup_buttons()

    @staticmethod
    def setup_window() -> Tk:
        """
        This sets up the main window
        """
        window = Tk()
        window.title("Settings")
        window.configure(bg=LIGHT_GREEN)
        window.geometry("1500x900")
        window.minsize(1500, 900)

        return window

    def start(self):
        """
        This will actually display the settings window
        """
        self.window.mainloop()

    def setup_banner(self):
        """
        This simply sets up the top banner at the top of the page
        """
        top_frame = tkinter.Frame(self.window, bg=LIGHT_GREEN)
        top_frame.pack(side=TOP, expand=True)

        title_text_label = Label(top_frame, text="A-LIFE CHALLENGE", background=LIGHT_GREEN)
        title_text_label.config(font=("Helvetica", 35))

        title_text_label.pack()

    def setup_configurations(self):
        """
        This sets up the different configuration options which are split into four sections

        --------------------------------------------------------------
        |                  |                      |                  |
        |  reproduction    |       colors         |      Click       |
        |      rates       |       guide          |      Type        |
        |                  |                      |                  |
        --------------------------------------------------------------
        |                  |                      |                  |
        |       Temp       |       Light          |     Oxygen       |
        |     Options      |      Options         |     Options      |
        |                  |                      |                  |
        --------------------------------------------------------------

        """
        top_frame = tkinter.Frame(self.window, bg=LIGHT_GREEN)
        bottom_frame = tkinter.Frame(self.window, bg=LIGHT_GREEN)

        repro_frame = tkinter.Frame(top_frame, bg=LIGHT_GREEN, borderwidth=25)
        size_frame = tkinter.Frame(top_frame, bg=LIGHT_GREEN, borderwidth=25)
        click_type_frame = tkinter.Frame(top_frame, bg=LIGHT_GREEN, borderwidth=25)

        temp_frame = tkinter.Frame(bottom_frame, bg=LIGHT_GREEN, borderwidth=75)
        light_frame = tkinter.Frame(bottom_frame, bg=LIGHT_GREEN, borderwidth=75)
        oxygen_frame = tkinter.Frame(bottom_frame, bg=LIGHT_GREEN, borderwidth=75)

        # Dealing with the reproduction inputs
        reproduction_text_label = Label(repro_frame, text="Reproduction Mutation Rates", background=LIGHT_GREEN)
        reproduction_text_label.config(font=("Arial", 12))
        reproduction_text_label.pack(side=TOP)

        insertion_frame = tkinter.Frame(repro_frame, bg=LIGHT_GREEN)
        insertion_label = Label(insertion_frame, text="Insertion", background=LIGHT_GREEN)
        insertion_label.config(font=("Arial", 10))
        insertion_label.pack(side=LEFT)
        self.insertion_value = tkinter.Entry(insertion_frame, width=12)
        self.insertion_value.insert(0, Global_access.repro_insertion)
        self.insertion_value.pack(side=LEFT)
        insertion_frame.pack(side=TOP)

        deletion_frame = tkinter.Frame(repro_frame, bg=LIGHT_GREEN)
        deletion_label = Label(deletion_frame, text="Deletion", background=LIGHT_GREEN)
        deletion_label.config(font=("Arial", 10))
        deletion_label.pack(side=LEFT)
        self.deletion_value = tkinter.Entry(deletion_frame, width=12)
        self.deletion_value.insert(0, Global_access.repro_deletion)
        self.deletion_value.pack(side=LEFT)
        deletion_frame.pack(side=TOP)

        substitution_frame = tkinter.Frame(repro_frame, bg=LIGHT_GREEN)
        substitution_label = Label(substitution_frame, text="Substitution", background=LIGHT_GREEN)
        substitution_label.config(font=("Arial", 10))
        substitution_label.pack(side=LEFT)
        self.substitution_value = tkinter.Entry(substitution_frame, width=12)
        self.substitution_value.insert(0, Global_access.repro_substitution)
        self.substitution_value.pack(side=LEFT)
        substitution_frame.pack(side=TOP)

        completed_button_frame = tkinter.Frame(repro_frame, bg=LIGHT_GREEN)
        completed_button = tkinter.Button(completed_button_frame, bg="light blue", text="Complete!",
                                          command=lambda: EnvironmentControl.set_mutations(
                                                                                        self.conn,
                                                                                        self.insertion_value.get(),
                                                                                        self.deletion_value.get(),
                                                                                        self.substitution_value.get()))
        completed_button.pack(side=TOP)
        completed_button_frame.pack(side=TOP)
        repro_frame.pack(side=LEFT)

        # Set up the size configuration options
        color_title_frame = tkinter.Frame(size_frame, bg=LIGHT_GREEN)
        color_title_frame.pack(side=TOP, expand=True)
        color_title_label = Label(color_title_frame, text="Color Guide", background=LIGHT_GREEN)
        color_title_label.config(font=("Arial", 12))
        color_title_label.pack(side=TOP)

        color_guide_frame = tkinter.Frame(size_frame, bg=LIGHT_GREEN)

        block_size = 25
        color_tile_frame = tkinter.Frame(color_guide_frame, bg="blue")
        canvas = Canvas(color_tile_frame, height=block_size * len(CELL_DESCRIPTIONS), width=block_size, bg=LIGHT_GREEN)
        i = 0
        for key, value in CELL_DESCRIPTIONS.items():
            canvas.create_rectangle(0, 0 + (block_size * i),
                                    block_size, block_size + (block_size * i), outline="black", fill=key, width=3)
            i+=1
        canvas.pack(side=TOP)

        color_tile_frame.pack(side=LEFT)

        color_label_frame = tkinter.Frame(color_guide_frame, bg=LIGHT_GREEN)
        for key, value in CELL_DESCRIPTIONS.items():
            tkinter.Label(color_label_frame, text=value, bg=LIGHT_GREEN).pack(side=TOP)

        color_label_frame.pack(side=LEFT)

        color_guide_frame.pack(side=TOP)
        size_frame.pack(side=LEFT)

        # Setup Click type section
        click_title_frame = tkinter.Frame(click_type_frame, bg=LIGHT_GREEN)
        click_title_frame.pack(side=TOP, expand=True)
        click_title_label = Label(click_title_frame, text="Click Type", background=LIGHT_GREEN)
        click_title_label.config(font=("Arial", 12))
        click_title_label.pack(side=TOP)
        click_title_frame.pack(side=TOP)

        self.click_dd_frame = tkinter.Frame(click_type_frame, bg=LIGHT_GREEN)
        self.click_dd_value = tkinter.StringVar(click_type_frame)
        self.click_dd_value.set(Global_access.CLICK_TYPE)
        self.click_dd = tkinter.OptionMenu(click_type_frame, self.click_dd_value, *CLICK_TYPES, command=lambda event:
                        EnvironmentControl.click_type(self.click_dd_value.get()))

        self.click_dd.config(width=15, bg="white")
        self.click_dd.pack(side=TOP)
        click_title_frame.pack(side=TOP)
        self.click_dd_frame.pack(side=LEFT)

        click_type_frame.pack(side=LEFT)
        top_frame.pack(side=TOP, expand=True)

        self.noise_controls.append(
            self.create_noise_control('Temperature Control', temp_frame, Factor.TEMPERATURE)
        )
        self.noise_controls.append(
            self.create_noise_control('Light Control', light_frame, Factor.LIGHT)
        )
        self.noise_controls.append(
            self.create_noise_control('Oxygen Control', oxygen_frame, Factor.OXYGEN)
        )

        bottom_frame.pack(side=TOP, expand=True)

    def create_slider(
        self,
        low: float,
        high: float,
        frame: tkinter.Frame,
        label_text: str,
        factor: Factor,
        field: str
    ) -> tkinter.Scale:
        slider_frame = tkinter.Frame(frame, bg=LIGHT_GREEN)

        label = Label(slider_frame, text=label_text, background=LIGHT_GREEN)
        label.config(font=("Arial", 10))
        label.pack(side=LEFT)

        variable = tkinter.DoubleVar(value=getattr(Global_access.noises[factor.value], field))
        slider = tkinter.Scale(
            slider_frame,
            from_=low,
            to=high,
            orient=HORIZONTAL,
            background=LIGHT_GREEN, resolution=.01,
            state=ACTIVE,
            variable=variable,
            command=lambda _: self.set_factor_field(factor, field, variable.get())
        )
        slider.pack(side=LEFT)

        slider_frame.pack(side=TOP)

        return slider

    def create_noise_control(self, label_text: str, frame: tkinter.Frame, factor: Factor) -> NoiseControl:
        title = Label(frame, text=label_text, background=LIGHT_GREEN)
        title.config(font=("Arial", 12))
        title.pack()

        control_frame = tkinter.Frame(frame, bg=LIGHT_GREEN)
        control_frame.pack(side=TOP)

        # use_noise checkbox
        use_noise_var = tkinter.BooleanVar(value=Global_access.noises[factor.value].use_noise)
        use_noise_checkbox = tkinter.Checkbutton(
            control_frame,
            text="Use Noise?",
            background=LIGHT_GREEN,
            variable=use_noise_var,
            command=lambda: self.set_factor_use_noise(factor, use_noise_var.get())
        )
        use_noise_checkbox.pack(side=TOP)

        center_slider = self.create_slider(-1.0, 1.0, control_frame, "Center", factor, "center")
        scale_slider = self.create_slider(1.0, 100.0, control_frame, "Scale", factor, "scale")
        depth_slider = self.create_slider(0.0, 2.0, control_frame, "Depth", factor, "depth")
        speed_slider = self.create_slider(0.0, 0.1, control_frame, "Speed", factor, "speed")

        frame.pack(side=LEFT)

        return NoiseControl(use_noise_checkbox, center_slider, scale_slider, depth_slider, speed_slider)

    def setup_buttons(self):
        """
        This will create the buttons in the settings window for statistics and back
        """
        bottom_frame = tkinter.Frame(self.window, bg=LIGHT_GREEN)

        statistics_button = tkinter.Button(bottom_frame, text="Statistics",
                                           bg=ORANGE_YELLOW, padx=20, bd=3, font=font.Font(size=16),
                                           command=lambda: self.request_data())
        statistics_button.pack(side=LEFT, padx=300)

        back_button = tkinter.Button(bottom_frame, text="Back", bg=GREEN, padx=20, bd=3, font=font.Font(size=16),
                                     command=lambda: self.window.destroy())
        back_button.pack(side=RIGHT, padx=300)

        bottom_frame.pack(side=TOP, expand=True)

    def request_data(self):
        """
        This will request data from the c++ application for the data processing window
        """
        send_message(self.conn, "request_all")
        # TODO: This call should actually be moved to the decoding of the messages
        # We need to ensure that we have all of the data before we display the window
        # Keeping this here for now in order to display the window and demonstrate functionality
        Setup_DataProcessingGUI.SetupDataDisplay()

    def set_factor_field(self, factor: Factor, field: str, value):
        setattr(Global_access.noises[factor.value], field, value)
        send_message(self.conn, "settings")

    def set_factor_use_noise(self, factor: Factor, use_noise: True):
        """
        This is a setter for the state of the sliders based on if the checkbox is clicked
        """
        noise = Global_access.noises[factor.value]
        noise_control = self.noise_controls[factor.value]

        if use_noise:
            noise_control.speed_slider.config(state=ACTIVE)
            noise_control.depth_slider.config(state=ACTIVE)
            noise_control.scale_slider.config(state=ACTIVE)
        else:
            noise_control.speed_slider.config(state=DISABLED)
            noise_control.depth_slider.config(state=DISABLED)
            noise_control.scale_slider.config(state=DISABLED)

        noise_control.center_slider.set(noise.center)
        noise_control.speed_slider.set(noise.speed)
        noise_control.depth_slider.set(noise.depth)
        noise_control.scale_slider.set(noise.scale)

        noise.use_noise = use_noise
        send_message(self.conn, "settings")

