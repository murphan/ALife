from tkinter import *
import numpy as np

# This is not yet a comprehensive list and is only for temporary fill in purposes at the moment
# Global list of available attributes to be able to display to the user
ATTRIBUTES = ["Average Age",
              "Average Temperature",
              "Death Rate",
              "Reproduction Rate",
              "Total Deaths",
              "Total Food"
              ]


class DataDisplayControl:
    def __init__(self):
        self.setup_graph_one()
        self.setup_graph_two()

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

    def set_graph_array(self, data, graph, array):
        """
        This will set the graph array data given the graph and array number
        :param data: The option selected on the option menu to change the graph to
        :param graph: The graph number (either 1 or 2)
        :param array: The array number (1-5)
        :return:
        """
        pass
