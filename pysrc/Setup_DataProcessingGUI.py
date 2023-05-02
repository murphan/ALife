import os
import sys
import tkinter
from tkinter import *
from tkinter.ttk import *
import matplotlib
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np

import Control_DataProcessingGUI

matplotlib.use("TkAgg")

# This is not yet a comprehensive list and is only for temporary fill in purposes at the moment
# Global list of available attributes to be able to display to the user
ATTRIBUTES = ["Average Age",
              "Average Temperature",
              "Death Rate",
              "Reproduction Rate",
              "Total Deaths",
              "Total Food"
              ]


class SetupDataDisplay:
    """
    Setting up of the display window. No data is processed here at all besides any potentially necessary for the
    displaying in the window.
    """
    def __init__(self):
        self.control = Control_DataProcessingGUI.DataDisplayControl()
        self.setup_window()
        self.setup_frames()

        self.window.mainloop()

    def setup_window(self):
        """
        Establish the three frames for the two graphs in the top and bottom right and the attributes display
        on the left side

        The frame is split up as such:
        |-----------------------------------------------------------------------------------------------------|
        |                                                  |                      |                           |
        |                                                  |                      |                           |
        |                                                  |     top_dropdowns    |        top_graph          |
        |                                                  |                      |                           |
        |                                                  |                      |                           |
        |                  genetics_frame                  |----------------------|---------------------------|
        |                                                  |                      |                           |
        |                                                  |                      |                           |
        |                                                  |   bottom_dropdowns   |        bottom_graph       |
        |                                                  |                      |                           |
        |                                                  |                      |                           |
        |-----------------------------------------------------------------------------------------------------|

        :return: None
        """
        # Setting up and giving attributes to the main window
        window = Tk()
        window.title("Data Processing")
        window.state('zoomed')
        window.configure(bg="white")
        window.geometry("1000x500")
        window.minsize(1500, 785)

        self.window = window

        # Add the left and right frames
        left_frame = tkinter.Frame(window, bg="white")
        right_frame = tkinter.Frame(window, bg="white")
        left_frame.pack(side=LEFT, expand=True)
        right_frame.pack(side=RIGHT, expand=True)

        # Right frame split into a top and bottom
        right_top_frame = tkinter.Frame(window, bg="white")
        right_bottom_frame = tkinter.Frame(window, bg="white")
        right_top_frame.pack(side=TOP, expand=True)
        right_bottom_frame.pack(side=BOTTOM, expand=True)

        # Finish setup of the right frames, there are four frames total there
        self.top_dropdowns = tkinter.Frame(right_top_frame, bg="white")
        self.top_dropdowns.pack(side=LEFT, expand=True)
        self.setup_top_dropdowns(self.top_dropdowns)
        self.top_graph = Frame(right_top_frame)
        self.top_graph.pack(side=RIGHT, expand=True)

        self.bottom_dropdowns = tkinter.Frame(right_bottom_frame, bg="white")
        self.bottom_dropdowns.pack(side=LEFT, expand=True)
        self.setup_bottom_dropdowns(self.bottom_dropdowns)
        self.bottom_graph = Frame(right_bottom_frame)
        self.bottom_graph.pack(side=RIGHT, expand=True)

        # Set up the left side
        self.genetics_frame = Frame(left_frame)
        self.genetics_frame.pack(expand=True)

    def setup_top_dropdowns(self, frame):
        """
        This will add the five dropdowns to the top frame

        :param frame: the frame to add the dropdowns to
        :return: None
        """
        self.top_dd_1_value = tkinter.StringVar(self.window)
        self.top_dd_1_value.set("Attribute/Variable")
        self.top_dd_1 = tkinter.OptionMenu(frame, self.top_dd_1_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.top_dd_1_value.get(), 1, 1))
        self.top_dd_1.pack(side=TOP)
        self.top_dd_1.config(fg="red", bg="white", width=20)

        self.top_dd_2_value = tkinter.StringVar(self.window)
        self.top_dd_2_value.set("Attribute/Variable")
        self.top_dd_2 = tkinter.OptionMenu(frame, self.top_dd_2_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.top_dd_2_value.get(), 1, 2))
        self.top_dd_2.pack(side=TOP)
        self.top_dd_2.config(fg="blue", bg="white", width=20)

        self.top_dd_3_value = tkinter.StringVar(self.window)
        self.top_dd_3_value.set("Attribute/Variable")
        self.top_dd_3 = tkinter.OptionMenu(frame, self.top_dd_3_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.top_dd_3_value.get(), 1, 3))
        self.top_dd_3.pack(side=TOP)
        self.top_dd_3.config(fg="orange", bg="white", width=20)

        self.top_dd_4_value = tkinter.StringVar(self.window)
        self.top_dd_4_value.set("Attribute/Variable")
        self.top_dd_4 = tkinter.OptionMenu(frame, self.top_dd_4_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.top_dd_4_value.get(), 1, 4))
        self.top_dd_4.pack(side=TOP)
        self.top_dd_4.config(fg="green", bg="white", width=20)

        self.top_dd_5_value = tkinter.StringVar(self.window)
        self.top_dd_5_value.set("Attribute/Variable")
        self.top_dd_5 = tkinter.OptionMenu(frame, self.top_dd_5_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.top_dd_5_value.get(), 1, 5))
        self.top_dd_5.pack(side=TOP)
        self.top_dd_5.config(fg="purple", bg="white", width=20)

    def setup_bottom_dropdowns(self, frame):
        """
        This will add the five dropdowns to the bottom frame

        :param frame: the frame to add the dropdowns to
        :return: None
        """
        # To track each of the five selections individually five different variables have to be made
        self.btm_dd_1_value = tkinter.StringVar(self.window)
        self.btm_dd_1_value.set("Attribute/Variable")
        self.btm_dd_1 = tkinter.OptionMenu(frame, self.btm_dd_1_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.btm_dd_1_value.get(), 2, 1))
        self.btm_dd_1.pack(side=TOP)
        self.btm_dd_1.config(fg="red", bg="white", width=20)

        self.btm_dd_2_value = tkinter.StringVar(self.window)
        self.btm_dd_2_value.set("Attribute/Variable")
        self.btm_dd_2 = tkinter.OptionMenu(frame, self.btm_dd_2_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.btm_dd_2_value.get(), 2, 2))
        self.btm_dd_2.pack(side=TOP)
        self.btm_dd_2.config(fg="blue", bg="white", width=20)

        self.btm_dd_3_value = tkinter.StringVar(self.window)
        self.btm_dd_3_value.set("Attribute/Variable")
        self.btm_dd_3 = tkinter.OptionMenu(frame, self.btm_dd_3_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.btm_dd_3_value.get(), 2, 3))
        self.btm_dd_3.pack(side=TOP)
        self.btm_dd_3.config(fg="orange", bg="white", width=20)

        self.btm_dd_4_value = tkinter.StringVar(self.window)
        self.btm_dd_4_value.set("Attribute/Variable")
        self.btm_dd_4 = tkinter.OptionMenu(frame, self.btm_dd_4_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.btm_dd_4_value.get(), 2, 4))
        self.btm_dd_4.pack(side=TOP)
        self.btm_dd_4.config(fg="green", bg="white", width=20)

        self.btm_dd_5_value = tkinter.StringVar(self.window)
        self.btm_dd_5_value.set("Attribute/Variable")
        self.btm_dd_5 = tkinter.OptionMenu(frame, self.btm_dd_5_value, *ATTRIBUTES,
                                           command=lambda event:
                                           self.control.set_graph_array(self.btm_dd_5_value.get(), 2, 5))
        self.btm_dd_5.pack(side=TOP)
        self.btm_dd_5.config(fg="purple", bg="white", width=20)

    def setup_frames(self):
        """
        This will make very basic graphs for data to eventually be displayed in
        :return: None
        """
        self.top_figure = Figure(figsize=(7, 4), dpi=100)
        plot = self.top_figure.add_subplot(1, 1, 1)

        plot.plot()

        canvas = FigureCanvasTkAgg(self.top_figure, self.top_graph)
        canvas.get_tk_widget().grid(row=0, column=0)

        self.bottom_figure = Figure(figsize=(7, 4), dpi=100)
        plot = self.bottom_figure.add_subplot(1, 1, 1)

        plot.plot()

        canvas = FigureCanvasTkAgg(self.bottom_figure, self.bottom_graph)
        canvas.get_tk_widget().grid(row=0, column=0)
