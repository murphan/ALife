import os
import sys
import tkinter
from tkinter import *
from tkinter.ttk import *
import matplotlib
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np

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


class DataDisplayGUI:
    """
    Setting up of the display window. No data is processed here at all besides any potentially necessary for the
    displaying in the window.
    """
    def __init__(self):
        self.setup_graph_one()
        self.setup_graph_two()
        self.setup_window()
        self.setup_frames()

        self.window.mainloop()

    def setup_graph_one(self):
        """
        Set up the first graph which is displayed in the top right corner of the window. The names of the arrays are
        shown and displayed as w1a1, w1a2, etc. which stands for window1array1, and so on.
        :return: None
        """
        self.w1a1 = np.array
        self.w1a2 = np.array
        self.w1a3 = np.array
        self.w1a4 = np.array
        self.w1a5 = np.array

    def setup_graph_two(self):
        """
        Set up the first graph which is displayed in the top right corner of the window.The names of the arrays are
        shown and displayed as w2a1, w2a2, etc. which stands for window2array1, and so on.
        :return: None
        """
        self.w2a1 = np.array
        self.w2a2 = np.array
        self.w2a3 = np.array
        self.w2a4 = np.array
        self.w2a5 = np.array

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
        dropdown1 = tkinter.OptionMenu(frame, self.top_dd_1_value, *ATTRIBUTES)
        dropdown1.pack(side=TOP)
        dropdown1.config(fg="red", bg="white", width=20)

        self.top_dd_2_value = tkinter.StringVar(self.window)
        self.top_dd_2_value.set("Attribute/Variable")
        dropdown2 = tkinter.OptionMenu(frame, self.top_dd_2_value, *ATTRIBUTES)
        dropdown2.pack(side=TOP)
        dropdown2.config(fg="blue", bg="white", width=20)

        self.top_dd_3_value = tkinter.StringVar(self.window)
        self.top_dd_3_value.set("Attribute/Variable")
        dropdown3 = tkinter.OptionMenu(frame, self.top_dd_3_value, *ATTRIBUTES)
        dropdown3.pack(side=TOP)
        dropdown3.config(fg="orange", bg="white", width=20)

        self.top_dd_4_value = tkinter.StringVar(self.window)
        self.top_dd_4_value.set("Attribute/Variable")
        dropdown4 = tkinter.OptionMenu(frame, self.top_dd_4_value, *ATTRIBUTES)
        dropdown4.pack(side=TOP)
        dropdown4.config(fg="green", bg="white", width=20)

        self.top_dd_5_value = tkinter.StringVar(self.window)
        self.top_dd_5_value.set("Attribute/Variable")
        dropdown5 = tkinter.OptionMenu(frame, self.top_dd_5_value, *ATTRIBUTES)
        dropdown5.pack(side=TOP)
        dropdown5.config(fg="purple", bg="white", width=20)

    def setup_bottom_dropdowns(self, frame):
        """
        This will add the five dropdowns to the bottom frame

        :param frame: the frame to add the dropdowns to
        :return: None
        """
        # To track each of the five selections individually five different variables have to be made
        self.btm_dd_1_value = tkinter.StringVar(self.window)
        self.btm_dd_1_value.set("Attribute/Variable")
        dropdown1 = tkinter.OptionMenu(frame, self.btm_dd_1_value, *ATTRIBUTES)
        dropdown1.pack(side=TOP)
        dropdown1.config(fg="red", bg="white", width=20)

        self.btm_dd_2_value = tkinter.StringVar(self.window)
        self.btm_dd_2_value.set("Attribute/Variable")
        dropdown2 = tkinter.OptionMenu(frame, self.btm_dd_2_value, *ATTRIBUTES)
        dropdown2.pack(side=TOP)
        dropdown2.config(fg="blue", bg="white", width=20)

        self.btm_dd_3_value = tkinter.StringVar(self.window)
        self.btm_dd_3_value.set("Attribute/Variable")
        dropdown3 = tkinter.OptionMenu(frame, self.btm_dd_3_value, *ATTRIBUTES)
        dropdown3.pack(side=TOP)
        dropdown3.config(fg="orange", bg="white", width=20)

        self.btm_dd_4_value = tkinter.StringVar(self.window)
        self.btm_dd_4_value.set("Attribute/Variable")
        dropdown4 = tkinter.OptionMenu(frame, self.btm_dd_4_value, *ATTRIBUTES)
        dropdown4.pack(side=TOP)
        dropdown4.config(fg="green", bg="white", width=20)

        self.btm_dd_5_value = tkinter.StringVar(self.window)
        self.btm_dd_5_value.set("Attribute/Variable")
        dropdown5 = tkinter.OptionMenu(frame, self.btm_dd_5_value, *ATTRIBUTES)
        dropdown5.pack(side=TOP)
        dropdown5.config(fg="purple", bg="white", width=20)

    def setup_frames(self):
        """
        This will make very basic graphs for data to eventually be displayed in
        :return: None
        """
        figure = Figure(figsize=(7, 4), dpi=100)
        plot = figure.add_subplot(1, 1, 1)

        plot.plot()

        canvas = FigureCanvasTkAgg(figure, self.top_graph)
        canvas.get_tk_widget().grid(row=0, column=0)

        figure = Figure(figsize=(7, 4), dpi=100)
        plot = figure.add_subplot(1, 1, 1)

        plot.plot()

        canvas = FigureCanvasTkAgg(figure, self.bottom_graph)
        canvas.get_tk_widget().grid(row=0, column=0)

# This can be enabled at the moment for temporary purposes of testing whether or not the GUI is working or not
# if __name__ == "__main__":
#     DataDisplayGUI()
