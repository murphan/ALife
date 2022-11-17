import os
import sys
import tkinter
from tkinter import *
from tkinter.ttk import *

from Control_EnvironmentGUI import EnvironmentControl


class EnvironmentGui:
    """
    This will do the set up of the EnvironmentGUI for the A-life simulation
    """
    def __init__(self):
        self.create_window()
        self.create_grid()
        self.create_menu()

        self.window.mainloop()

    def create_window(self):
        """
        This will do the set up of the main window
        """
        self.window = Tk()
        self.window.title("A-Life Challenge")
        self.window.state('zoomed')
        self.window.configure(bg="white")
        self.window.geometry("1000x500")
        self.window.minsize(1500, 785)

    def create_grid(self):
        """
        This will do the setting up of the grid for the simulation to be displayed in
        """
        self.screen_width = self.window.winfo_screenwidth()
        self.screen_height = self.window.winfo_screenheight()

        self.grid = Canvas(self.window, width=self.screen_width, height=self.screen_height - 200, bg="grey")

        self.grid_width = 0
        self.grid_height = 0
        width = 10
        while width < self.screen_width - 6:
            height = 10
            self.grid_width += 1
            self.grid_height = 0
            while height < self.screen_height - 200:
                self.grid.create_rectangle(width, height, 10, 10, outline='black')
                self.grid_height += 1
                height += 10
            width += 10

        self.grid.pack()

    def create_menu(self):
        """
        This will set up the menu for selecting and changing options for the simulation.
        The frame will be positioned at the bottom of the screen
        """
        Speed_options = ["1x", "2x", "4x"]
        self.speed_dropdown_value = tkinter.StringVar(self.window)
        self.speed_dropdown_value.set("1x")
        self.speed_dropdown = tkinter.OptionMenu(self.window, self.speed_dropdown_value, *Speed_options,
                                           command=lambda event:
                                           EnvironmentControl.set_speed(self, self.speed_dropdown_value.get()))
        self.speed_dropdown.pack(side=BOTTOM)

        self.speed_text = tkinter.Label(self.window, text="Speed", bg = "white")
        self.speed_text.pack(side=BOTTOM)

        # TODO: Create a size option for rows and columns
        # TODO: Create an option for pause and play buttons


if __name__ == "__main__":
    EnvironmentGui()
