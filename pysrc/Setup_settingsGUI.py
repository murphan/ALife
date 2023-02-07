import tkinter
from tkinter import *
import tkinter.ttk as ttk
import tkinter.font as font

import Setup_DataProcessingGUI
import Setup_EnvironmentGUI
import Control_EnvironmentGUI as Control_Environment

LIGHT_GREEN = '#5fad75'
ORANGE_YELLOW = '#e8b320'
GREEN = '#0c871b'
BLUE = '#3289a8'

SPEEDS = [".5x",
          "1x",
          "2x",
          "4x"]

CLICK_TYPES = ["Wall",
               "Organism",
               "Food"]


class SetupSettings:
    """
    This will set up the settings window for manipulating different options in Environment
    """
    def __init__(self):
        self.setup_window()
        self.setup_banner()
        self.setup_configurations()
        self.setup_buttons()

    def start(self, settings):
        """
        This will actually display the settings window

        :param settings: An instance of the main "management" class
        :param type: Management class instance
        """
        self.env_settings = settings
        self.window.mainloop()

    def setup_window(self):
        """
        This sets up the main window
        """
        window = Tk()
        window.title("Settings")
        window.state('zoomed')
        window.configure(bg=LIGHT_GREEN)
        window.geometry("1000x500")
        window.minsize(1500, 785)

        self.window = window

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

        -----------------------------------------------------------------------------------
        |                  |                      |                  |                    |
        |                  |                      |                  |                    |
        |       speed      |        size          |    Environment   |       Click        |
        |                  |                      |   Manipulations  |       Type         |
        |                  |                      |                  |                    |
        -----------------------------------------------------------------------------------
        """
        middle_frame = tkinter.Frame(self.window, bg=LIGHT_GREEN, borderwidth=150)
        speed_frame = tkinter.Frame(middle_frame, bg=LIGHT_GREEN, borderwidth=25)
        size_frame = tkinter.Frame(middle_frame, bg=LIGHT_GREEN, borderwidth=25)
        env_manip_frame = tkinter.Frame(middle_frame, bg=LIGHT_GREEN, borderwidth=25)
        click_type_frame = tkinter.Frame(middle_frame, bg=LIGHT_GREEN, borderwidth=25)

        # Setting up the speed dropdown frame
        speed_text_frame = tkinter.Frame(speed_frame, bg=LIGHT_GREEN)
        speed_text_frame.pack(side=TOP, expand=True)

        speed_text_label = Label(speed_frame, text="Set Speed", background=LIGHT_GREEN)
        speed_text_label.config(font=("Arial", 12))
        speed_text_label.pack()

        self.speed_dd_value = tkinter.StringVar(speed_frame)
        self.speed_dd_value.set("1x")
        self.speed_dd = tkinter.OptionMenu(speed_frame, self.speed_dd_value, *SPEEDS, command=lambda event:
                                    Control_Environment.EnvironmentControl.set_speed(self, self.speed_dd_value.get()))
        self.speed_dd.config(width=15, bg="white")
        self.speed_dd.pack(side=TOP)
        speed_frame.pack(side=LEFT)

        # Set up the size configuration options
        size_title_frame = tkinter.Frame(size_frame, bg=LIGHT_GREEN)
        size_title_frame.pack(side=TOP, expand=True)
        size_title_label = Label(size_title_frame, text="Set Size", background=LIGHT_GREEN)
        size_title_label.config(font=("Arial", 12))
        size_title_label.pack(side=TOP)

        width_frame = tkinter.Frame(size_frame, bg=LIGHT_GREEN)
        width_text_label = Label(width_frame, text="Width(don't use *yet)", background="White")
        width_text_label.config(font=("Arial", 12))
        width_text_label.pack(side=LEFT)
        width_input = ttk.Spinbox(width_frame, from_=50.0, to=77.0,increment=1.0,
                                  state='readonly', wrap=True, command=lambda:
                                  Control_Environment.EnvironmentControl.set_width(self, width_input.get()))
        width_input.pack(side=LEFT)
        width_frame.pack(side=TOP)

        height_frame = tkinter.Frame(size_frame, bg=LIGHT_GREEN)
        height_text_label = Label(height_frame, text="Height(don't use *yet)", background="White")
        height_text_label.config(font=("Arial", 12))
        height_text_label.pack(side=LEFT)
        height_input = ttk.Spinbox(height_frame, from_=100.0, to=150.0, increment=1.0,
                                   state='readonly', width=19, wrap=True, command=lambda:
                                   Control_Environment.EnvironmentControl.set_height(self, height_input.get()))
        height_input.pack(side=LEFT)
        height_frame.pack(side=BOTTOM)

        size_frame.pack(side=LEFT)

        # set up the environment variable controls
        env_title_frame = tkinter.Frame(env_manip_frame, bg=LIGHT_GREEN)
        env_title_frame.pack(side=TOP, expand=True)
        env_title_label = Label(env_title_frame, text="Change Environment Variables Noise Levels", background=LIGHT_GREEN)
        env_title_label.config(font=("Arial", 12))
        env_title_label.pack(side=TOP)

        temp_control_frame = tkinter.Frame(env_manip_frame, bg=LIGHT_GREEN)
        temp_text_label = Label(temp_control_frame, text=f"Temperature", background="White")
        temp_text_label.config(font=("Arial", 12))
        temp_text_label.pack(side=LEFT)
        temp_input = ttk.Spinbox(temp_control_frame, from_=-128.0, to=127.0, increment=1.0,
                                 width=12, wrap=True)
        temp_input.pack(side=LEFT)
        temp_control_frame.pack(side=TOP)

        light_control_frame = tkinter.Frame(env_manip_frame, bg=LIGHT_GREEN)
        light_text_label = Label(light_control_frame, text="Light", background="White")
        light_text_label.config(font=("Arial", 12))
        light_text_label.pack(side=LEFT)
        light_input = ttk.Spinbox(light_control_frame, from_=-128.0, to=127.0, increment=1.0,
                                  width=22, wrap=True)
        light_input.pack(side=LEFT)
        light_control_frame.pack(side=TOP)

        oxygen_control_frame = tkinter.Frame(env_manip_frame, bg=LIGHT_GREEN)
        oxygen_text_label = Label(oxygen_control_frame, text="Oxygen", background="White")
        oxygen_text_label.config(font=("Arial", 12))
        oxygen_text_label.pack(side=LEFT)
        oxygen_input = ttk.Spinbox(oxygen_control_frame, from_=-128.0, to=127.0, increment=1.0,
                                   width=19, wrap=True)
        oxygen_input.pack(side=LEFT)
        oxygen_control_frame.pack(side=TOP)

        # Button for telling program user is done changing numbers
        done_button_frame = tkinter.Frame(env_manip_frame, bg=LIGHT_GREEN)

        done_button = tkinter.Button(done_button_frame, text="Numbers are good!",
                                     bg=BLUE, padx=20, bd=3, font=font.Font(size=10), command=lambda:
                                     Control_Environment.EnvironmentControl.set_env_vars(self, temp_input.get(),
                                                                                         light_input.get(),
                                                                                         oxygen_input.get()))
        done_button.pack(side=LEFT)
        done_button_frame.pack(side=TOP)

        env_manip_frame.pack(side=LEFT)

        # Setup Click type section
        click_title_frame = tkinter.Frame(click_type_frame, bg=LIGHT_GREEN)
        click_title_frame.pack(side=TOP, expand=True)
        click_title_label = Label(click_title_frame, text="Change Click Type", background=LIGHT_GREEN)
        click_title_label.config(font=("Arial", 12))
        click_title_label.pack(side=TOP)
        click_title_frame.pack(side=TOP)

        self.click_dd_value = tkinter.StringVar(click_type_frame)
        self.click_dd_value.set("Organism")
        self.speed_dd = tkinter.OptionMenu(click_type_frame, self.click_dd_value, *CLICK_TYPES, command=lambda event:
        Control_Environment.EnvironmentControl.click_type(self, self.click_dd_value.get()))
        self.speed_dd.config(width=15, bg="white")
        self.speed_dd.pack(side=TOP)
        speed_frame.pack(side=LEFT)
        click_type_frame.pack(side=LEFT)

        middle_frame.pack(side=TOP, expand=True)

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
        Control_Environment.EnvironmentControl.send_message(self, self.env_settings.conn, "request_all")
        # TODO: This call should actually be moved to the decoding of the messages
        # We need to ensure that we have all of the data before we display the window
        # Keeping this here for now in order to display the window and demonstrate functionality
        Setup_DataProcessingGUI.SetupDataDisplay()