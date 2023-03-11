import tkinter
from tkinter import *
import tkinter.ttk as ttk
import tkinter.font as font

import Setup_DataProcessingGUI
import Setup_EnvironmentGUI
import Control_EnvironmentGUI as Control_Environment
import Global_access

LIGHT_GREEN = '#5fad75'
ORANGE_YELLOW = '#e8b320'
GREEN = '#0c871b'
BLUE = '#3289a8'

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

        --------------------------------------------------------------
        |                  |                      |                  |
        |        fps       |        size          |       Click      |
        |                  |                      |       Type       |
        |                  |                      |                  |
        --------------------------------------------------------------
        |                  |                      |                  |
        |       Temp       |       Light          |     Oxygen       |
        |     Options      |      Options         |     Options      |
        |                  |                      |                  |
        --------------------------------------------------------------

        """
        middle_top_frame = tkinter.Frame(self.window, bg=LIGHT_GREEN)
        middle_bottom_frame = tkinter.Frame(self.window, bg=LIGHT_GREEN)
        speed_frame = tkinter.Frame(middle_top_frame, bg=LIGHT_GREEN, borderwidth=75)
        size_frame = tkinter.Frame(middle_top_frame, bg=LIGHT_GREEN, borderwidth=75)
        click_type_frame = tkinter.Frame(middle_top_frame, bg=LIGHT_GREEN, borderwidth=75)

        temp_frame = tkinter.Frame(middle_bottom_frame, bg=LIGHT_GREEN, borderwidth=75)
        light_frame = tkinter.Frame(middle_bottom_frame, bg=LIGHT_GREEN, borderwidth=75)
        oxygen_frame = tkinter.Frame(middle_bottom_frame, bg=LIGHT_GREEN, borderwidth=75)

        # Setting up the fps slider frame
        speed_text_frame = tkinter.Frame(speed_frame, bg=LIGHT_GREEN)
        speed_text_frame.pack(side=TOP, expand=True)

        speed_text_label = Label(speed_frame, text="Frames Per Second", background=LIGHT_GREEN)
        speed_text_label.config(font=("Arial", 12))
        speed_text_label.pack()

        self.fps_slider = tkinter.Scale(speed_frame, from_=1, to=21, orient=HORIZONTAL,
                                        background=LIGHT_GREEN, command=lambda event:
                                        Control_Environment.EnvironmentControl.set_fps(self, self.fps_slider.get()))
        self.fps_slider.set(Global_access.fps)
        self.fps_slider.pack(side=TOP)
        speed_frame.pack(side=LEFT)

        # Set up the size configuration options
        size_title_frame = tkinter.Frame(size_frame, bg=LIGHT_GREEN)
        size_title_frame.pack(side=TOP, expand=True)
        size_title_label = Label(size_title_frame, text="Size", background=LIGHT_GREEN)
        size_title_label.config(font=("Arial", 12))
        size_title_label.pack(side=TOP)

        width_frame = tkinter.Frame(size_frame, bg=LIGHT_GREEN)
        width_text_label = Label(width_frame, text="Width(don't use *yet)", background="White")
        width_text_label.config(font=("Arial", 12))
        width_text_label.pack(side=LEFT)
        width_input = ttk.Spinbox(width_frame, from_=50.0, to=77.0,increment=1.0,
                                  state='readonly', wrap=True, command=lambda event:
                                  Control_Environment.EnvironmentControl.set_width(self, width_input.get()))
        width_input.pack(side=LEFT)
        width_frame.pack(side=TOP)

        height_frame = tkinter.Frame(size_frame, bg=LIGHT_GREEN)
        height_text_label = Label(height_frame, text="Height(don't use *yet)", background="White")
        height_text_label.config(font=("Arial", 12))
        height_text_label.pack(side=LEFT)
        height_input = ttk.Spinbox(height_frame, from_=100.0, to=150.0, increment=1.0,
                                   state='readonly', width=19, wrap=True, command=lambda event:
                                   Control_Environment.EnvironmentControl.set_height(self, height_input.get()))
        height_input.pack(side=LEFT)
        height_frame.pack(side=BOTTOM)

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
                        Control_Environment.EnvironmentControl.click_type(self, self.click_dd_value.get()))
        self.click_dd.config(width=15, bg="white")
        self.click_dd.pack(side=TOP)
        click_title_frame.pack(side=TOP)
        self.click_dd_frame.pack(side=LEFT)

        click_type_frame.pack(side=LEFT)
        middle_top_frame.pack(side=TOP, expand=True)

        # set up the environment variable controls
        # Set up Temperature
        temp_control_frame = tkinter.Frame(temp_frame, bg=LIGHT_GREEN)
        temp_title = Label(temp_frame, text="Temperature Options:",
                                background=LIGHT_GREEN)
        temp_title.config(font=("Arial", 12))
        temp_title.pack(side=TOP)
        temp_noise = tkinter.IntVar(value=Global_access.temp_noise)
        temp_noise_check = tkinter.Checkbutton(temp_control_frame, text="Use Noise?", background=LIGHT_GREEN,
                                               variable=temp_noise, command=lambda:
                                               self.set_temp_noise(temp_noise.get()))
        temp_noise_check.pack(side=LEFT)

        temp_value_frame = tkinter.Frame(temp_frame, bg=LIGHT_GREEN)
        temp_value_label = Label(temp_value_frame, text="Temp Value", background=LIGHT_GREEN)
        temp_value_label.config(font=("Arial", 10))
        temp_value_label.pack(side=LEFT)
        self.temp_value = tkinter.Scale(temp_value_frame, from_=-1, to=1, orient=HORIZONTAL,
                                        background=LIGHT_GREEN, resolution=.01,
                                        state=DISABLED if temp_noise.get() else ACTIVE, command=lambda event:
                                        Control_Environment.EnvironmentControl.set_temp_value(self,
                                                                                              self.temp_value.get()))
        self.temp_value.set(Global_access.temperature)
        self.temp_value.pack(side=LEFT)

        temp_scale_frame = tkinter.Frame(temp_frame, bg=LIGHT_GREEN)
        temp_noise_check.config(font=("Arial", 10))
        temp_scale_label = Label(temp_scale_frame, text="Temp Noise Scale", background=LIGHT_GREEN)
        temp_scale_label.config(font=("Arial", 10))
        temp_scale_label.pack(side=LEFT)
        self.temp_scale = tkinter.Scale(temp_scale_frame, from_=1, to=50, orient=HORIZONTAL,
                                        background=LIGHT_GREEN, resolution=.1,
                                        state=ACTIVE if temp_noise.get() else DISABLED, command=lambda event:
                                        Control_Environment.EnvironmentControl.set_temp_noise_levels(self,
                                                                                              "scale",
                                                                                              self.temp_scale.get()))
        self.temp_scale.set(Global_access.temp_scale)
        self.temp_scale.pack(side=LEFT)

        temp_depth_frame = tkinter.Frame(temp_frame, bg=LIGHT_GREEN)
        temp_depth_label = Label(temp_depth_frame, text="Temp Noise Depth", background=LIGHT_GREEN)
        temp_depth_label.config(font=("Arial", 10))
        temp_depth_label.pack(side=LEFT)
        self.temp_depth = tkinter.Scale(temp_depth_frame, from_=0, to=2, orient=HORIZONTAL,
                                        background=LIGHT_GREEN, resolution=.1,
                                        state=ACTIVE if temp_noise.get() else DISABLED, command=lambda event:
                                        Control_Environment.EnvironmentControl.set_temp_noise_levels(self,
                                                                                              "depth",
                                                                                              self.temp_depth.get()))
        self.temp_depth.set(Global_access.temp_depth)
        self.temp_depth.pack(side=LEFT)

        temp_speed_frame = tkinter.Frame(temp_frame, bg=LIGHT_GREEN)
        temp_speed_label = Label(temp_speed_frame, text="Temp Noise Speed", background=LIGHT_GREEN)
        temp_speed_label.config(font=("Arial", 10))
        temp_speed_label.pack(side=LEFT)
        self.temp_speed = tkinter.Scale(temp_speed_frame, from_=0, to=2, orient=HORIZONTAL,
                                        background=LIGHT_GREEN, resolution=.1,
                                        state=ACTIVE if temp_noise.get() else DISABLED, command=lambda event:
                                        Control_Environment.EnvironmentControl.set_temp_noise_levels(self,
                                                                                              "speed",
                                                                                              self.temp_speed.get()))
        self.temp_speed.set(Global_access.temp_speed)
        self.temp_speed.pack(side=LEFT)

        temp_control_frame.pack(side=TOP)
        temp_value_frame.pack(side=TOP)
        temp_scale_frame.pack(side=TOP)
        temp_depth_frame.pack(side=TOP)
        temp_speed_frame.pack(side=TOP)
        temp_frame.pack(side=LEFT)

        # Set up light
        light_control_frame = tkinter.Frame(light_frame, bg=LIGHT_GREEN)
        light_title = Label(light_frame, text="Light Options:",
                           background=LIGHT_GREEN)
        light_title.config(font=("Arial", 12))
        light_title.pack(side=TOP)
        light_noise = tkinter.IntVar(value=Global_access.light_noise)
        light_noise_check = tkinter.Checkbutton(light_control_frame, text="Use Noise?", background=LIGHT_GREEN,
                                                variable=light_noise, command=lambda:
                                                self.set_light_noise(light_noise.get()))
        light_noise_check.pack(side=LEFT)

        light_value_frame = tkinter.Frame(light_frame, bg=LIGHT_GREEN)
        light_value_label = Label(light_value_frame, text="Light Value", background=LIGHT_GREEN)
        light_value_label.config(font=("Arial", 10))
        light_value_label.pack(side=LEFT)
        self.light_value = tkinter.Scale(light_value_frame, from_=-1, to=1, orient=HORIZONTAL,
                                         background=LIGHT_GREEN, resolution=.01,
                                         state=DISABLED if light_noise.get() else ACTIVE, command=lambda event:
                                         Control_Environment.EnvironmentControl.set_light_value(self,
                                                                                                self.light_value.get()))
        self.light_value.set(Global_access.light)
        self.light_value.pack(side=LEFT)

        light_scale_frame = tkinter.Frame(light_frame, bg=LIGHT_GREEN)
        light_noise_check.config(font=("Arial", 10))
        light_scale_label = Label(light_scale_frame, text="Light Noise Scale", background=LIGHT_GREEN)
        light_scale_label.config(font=("Arial", 10))
        light_scale_label.pack(side=LEFT)
        self.light_scale = tkinter.Scale(light_scale_frame, from_=1, to=50, orient=HORIZONTAL,
                                         background=LIGHT_GREEN, resolution=.1,
                                         state=ACTIVE if light_noise.get() else DISABLED, command=lambda event:
                                         Control_Environment.EnvironmentControl.set_light_noise_levels(self,
                                                                                                "scale",
                                                                                                self.light_scale.get()))
        self.light_scale.set(Global_access.light_scale)
        self.light_scale.pack(side=LEFT)

        light_depth_frame = tkinter.Frame(light_frame, bg=LIGHT_GREEN)
        light_depth_label = Label(light_depth_frame, text="Light Noise Depth", background=LIGHT_GREEN)
        light_depth_label.config(font=("Arial", 10))
        light_depth_label.pack(side=LEFT)
        self.light_depth = tkinter.Scale(light_depth_frame, from_=0, to=2, orient=HORIZONTAL,
                                         background=LIGHT_GREEN, resolution=.1,
                                         state=ACTIVE if light_noise.get() else DISABLED, command=lambda event:
                                         Control_Environment.EnvironmentControl.set_light_noise_levels(self,
                                                                                                "depth",
                                                                                                self.light_depth.get()))
        self.light_depth.set(Global_access.light_depth)
        self.light_depth.pack(side=LEFT)

        light_speed_frame = tkinter.Frame(light_frame, bg=LIGHT_GREEN)
        light_speed_label = Label(light_speed_frame, text="Light Noise Speed", background=LIGHT_GREEN)
        light_speed_label.config(font=("Arial", 10))
        light_speed_label.pack(side=LEFT)
        self.light_speed = tkinter.Scale(light_speed_frame, from_=0, to=2, orient=HORIZONTAL,
                                         background=LIGHT_GREEN, resolution=.1,
                                         state=ACTIVE if light_noise.get() else DISABLED, command=lambda event:
                                         Control_Environment.EnvironmentControl.set_light_noise_levels(self,
                                                                                                "speed",
                                                                                                self.light_speed.get()))
        self.light_speed.set(Global_access.light_speed)
        self.light_speed.pack(side=LEFT)

        light_control_frame.pack(side=TOP)
        light_value_frame.pack(side=TOP)
        light_scale_frame.pack(side=TOP)
        light_depth_frame.pack(side=TOP)
        light_speed_frame.pack(side=TOP)
        light_frame.pack(side=LEFT)

        # Set up Oxygen
        oxygen_control_frame = tkinter.Frame(oxygen_frame, bg=LIGHT_GREEN)
        oxygen_title = Label(oxygen_frame, text="Oxygen Options:",
                            background=LIGHT_GREEN)
        oxygen_title.config(font=("Arial", 12))
        oxygen_title.pack(side=TOP)
        oxygen_noise = tkinter.IntVar(value=Global_access.oxygen_noise)
        oxygen_noise_check = tkinter.Checkbutton(oxygen_control_frame, text="Use Noise?", background=LIGHT_GREEN,
                                                 variable=oxygen_noise, command=lambda:
                                                 self.set_oxygen_noise(oxygen_noise.get()))
        oxygen_noise_check.pack(side=LEFT)

        oxygen_value_frame = tkinter.Frame(oxygen_frame, bg=LIGHT_GREEN)
        oxygen_value_label = Label(oxygen_value_frame, text="Oxygen Value", background=LIGHT_GREEN)
        oxygen_value_label.config(font=("Arial", 10))
        oxygen_value_label.pack(side=LEFT)
        self.oxygen_value = tkinter.Scale(oxygen_value_frame, from_=-1, to=1, orient=HORIZONTAL,
                                          background=LIGHT_GREEN, resolution=.01,
                                          state=DISABLED if oxygen_noise.get() else ACTIVE, command=lambda event:
                                          Control_Environment.EnvironmentControl.set_oxygen_value(self,
                                                                                                  self.oxygen_value.get()))
        self.oxygen_value.set(Global_access.oxygen)
        self.oxygen_value.pack(side=LEFT)

        oxygen_scale_frame = tkinter.Frame(oxygen_frame, bg=LIGHT_GREEN)
        oxygen_noise_check.config(font=("Arial", 10))
        oxygen_scale_label = Label(oxygen_scale_frame, text="Oxygen Noise Scale", background=LIGHT_GREEN)
        oxygen_scale_label.config(font=("Arial", 10))
        oxygen_scale_label.pack(side=LEFT)
        self.oxygen_scale = tkinter.Scale(oxygen_scale_frame, from_=1, to=50, orient=HORIZONTAL,
                                          background=LIGHT_GREEN, resolution=.1,
                                          state=ACTIVE if oxygen_noise.get() else DISABLED, command=lambda event:
                                          Control_Environment.EnvironmentControl.set_oxygen_noise_levels(self,
                                                                                                  "scale",
                                                                                                  self.oxygen_scale.get()))
        self.oxygen_scale.set(Global_access.oxygen_scale)
        self.oxygen_scale.pack(side=LEFT)

        oxygen_depth_frame = tkinter.Frame(oxygen_frame, bg=LIGHT_GREEN)
        oxygen_depth_label = Label(oxygen_depth_frame, text="Oxygen Noise Depth", background=LIGHT_GREEN)
        oxygen_depth_label.config(font=("Arial", 10))
        oxygen_depth_label.pack(side=LEFT)
        self.oxygen_depth = tkinter.Scale(oxygen_depth_frame, from_=0, to=2, orient=HORIZONTAL,
                                          background=LIGHT_GREEN, resolution=.1,
                                          state=ACTIVE if oxygen_noise.get() else DISABLED, command=lambda event:
                                          Control_Environment.EnvironmentControl.set_oxygen_noise_levels(self,
                                                                                                  "depth",
                                                                                                  self.oxygen_depth.get()))
        self.oxygen_depth.set(Global_access.oxygen_depth)
        self.oxygen_depth.pack(side=LEFT)

        oxygen_speed_frame = tkinter.Frame(oxygen_frame, bg=LIGHT_GREEN)
        oxygen_speed_label = Label(oxygen_speed_frame, text="Osygen Noise Speed", background=LIGHT_GREEN)
        oxygen_speed_label.config(font=("Arial", 10))
        oxygen_speed_label.pack(side=LEFT)
        self.oxygen_speed = tkinter.Scale(oxygen_speed_frame, from_=0, to=2, orient=HORIZONTAL,
                                          background=LIGHT_GREEN, resolution=.1,
                                          state=ACTIVE if oxygen_noise.get() else DISABLED, command=lambda event:
                                          Control_Environment.EnvironmentControl.set_oxygen_noise_levels(self,
                                                                                                  "speed",
                                                                                                  self.oxygen_speed.get()))
        self.oxygen_speed.set(Global_access.oxygen_speed)
        self.oxygen_speed.pack(side=LEFT)

        oxygen_control_frame.pack(side=TOP)
        oxygen_value_frame.pack(side=TOP)
        oxygen_scale_frame.pack(side=TOP)
        oxygen_depth_frame.pack(side=TOP)
        oxygen_speed_frame.pack(side=TOP)
        oxygen_frame.pack(side=LEFT)

        middle_bottom_frame.pack(side=TOP, expand=True)

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

    def set_oxygen_noise(self, oxygen_noise):
        """
        This is a setter for the state of the sliders based on if the checkbox is clicked
        """
        Control_Environment.EnvironmentControl.set_oxygen_noise(self, oxygen_noise)
        if Global_access.oxygen_noise:
            self.oxygen_speed.config(state=ACTIVE)
            self.oxygen_depth.config(state=ACTIVE)
            self.oxygen_scale.config(state=ACTIVE)
            self.oxygen_value.config(state=DISABLED, takefocus=0)
            self.oxygen_speed.set(Global_access.oxygen_speed)
            self.oxygen_depth.set(Global_access.oxygen_depth)
            self.oxygen_scale.set(Global_access.oxygen_scale)
        else:
            self.oxygen_speed.config(state=DISABLED, takefocus=0)
            self.oxygen_depth.config(state=DISABLED, takefocus=0)
            self.oxygen_scale.config(state=DISABLED, takefocus=0)
            self.oxygen_value.config(state=ACTIVE)
            self.oxygen_value.set(Global_access.oxygen)

    def set_light_noise(self, light_noise):
        """
        This is a setter for the state of the sliders based on if the checkbox is clicked
        """
        Control_Environment.EnvironmentControl.set_light_noise(self, light_noise)
        if Global_access.light_noise:
            self.light_speed.config(state=ACTIVE)
            self.light_depth.config(state=ACTIVE)
            self.light_scale.config(state=ACTIVE)
            self.light_value.config(state=DISABLED, takefocus=0)
            self.light_speed.set(Global_access.light_speed)
            self.light_depth.set(Global_access.light_depth)
            self.light_scale.set(Global_access.light_scale)
        else:
            self.light_speed.config(state=DISABLED, takefocus=0)
            self.light_depth.config(state=DISABLED, takefocus=0)
            self.light_scale.config(state=DISABLED, takefocus=0)
            self.light_value.config(state=ACTIVE)
            self.light_value.set(Global_access.light)

    def set_temp_noise(self, temp_noise):
        """
        This is a setter for the state of the sliders based on if the checkbox is clicked
        """
        Control_Environment.EnvironmentControl.set_temp_noise(self, temp_noise)
        if Global_access.temp_noise:
            self.temp_speed.config(state=ACTIVE)
            self.temp_depth.config(state=ACTIVE)
            self.temp_scale.config(state=ACTIVE)
            self.temp_value.config(state=DISABLED, takefocus=0)
            self.temp_speed.set(Global_access.temp_speed)
            self.temp_depth.set(Global_access.temp_depth)
            self.temp_scale.set(Global_access.temp_scale)
        else:
            self.temp_speed.config(state=DISABLED, takefocus=0)
            self.temp_depth.config(state=DISABLED, takefocus=0)
            self.temp_scale.config(state=DISABLED, takefocus=0)
            self.temp_value.config(state=ACTIVE)
            self.temp_value.set(Global_access.temperature)
