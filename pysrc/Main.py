import os
from threading import Thread
from multiprocessing import Process
import sys
import socket
from tkinter import messagebox
# Hides a console message from the pygame module
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'
import pygame

import Control_EnvironmentGUI
import Setup_EnvironmentGUI
import Setup_settingsGUI
import Global_access
import receive_message


class Management:
    def __init__(self, *args, **kwargs):
        self.EnvironmentGui = Setup_EnvironmentGUI.SetupEnvironment()
        self.EnvironmentControl = Control_EnvironmentGUI.EnvironmentControl()

        # Set up the socket connection to the c++ application
        host = socket.gethostname()
        port = self.read_port()
        self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.conn.connect((host, port))
        # self.temp_func()

    def start_receiver(self):
        self.thread = Thread(target=receive_message.decode_message, args=(self, self.conn,))
        self.thread.start()

    def main_loop(self):
        while True:
            self.EnvironmentGui.drawGrid()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.exiting()
                if event.type == pygame.MOUSEBUTTONDOWN:
                    if self.EnvironmentGui.settings_button.mouse_click(event):  # Show the settings window
                        self.open_settings()
                    elif self.EnvironmentGui.play_button.mouse_click(event):  # start button was clicked
                        self.EnvironmentControl.start(self.conn)
                    elif self.EnvironmentGui.pause_button.mouse_click(event):  # pause button was clicked
                        self.EnvironmentControl.stop(self.conn)
                    else:  # Display on the environment that a square was clicked
                        self.EnvironmentControl.square_clicked(event, self.EnvironmentGui, self.conn)

            pygame.display.update()

    def exiting(self):
        confirm = messagebox.askokcancel(title="Quit?", message="Are you sure that you want to Quit?")
        if confirm:
            pygame.quit()
            Global_access.STOPPED = 1
            self.conn.close()
            sys.exit()
        else:
            return

    def temp_func(self):
        self.EnvironmentControl.fill_cell(12, 30, Global_access.org_colors[1])
        self.EnvironmentControl.fill_cell(13, 30, Global_access.org_colors[2])
        self.EnvironmentControl.fill_cell(14, 30, Global_access.org_colors[3])
        self.EnvironmentControl.fill_cell(15, 30, Global_access.org_colors[4])
        self.EnvironmentControl.fill_cell(16, 30, Global_access.org_colors[5])
        self.EnvironmentControl.fill_cell(17, 30, Global_access.org_colors[6])
        self.EnvironmentControl.fill_cell(18, 30, Global_access.org_colors[7])
        self.EnvironmentControl.fill_cell(19, 30, Global_access.org_colors[8])
        self.EnvironmentControl.fill_cell(20, 30, Global_access.org_colors[9])
        self.EnvironmentControl.fill_cell(21, 30, Global_access.org_colors[10])
        self.EnvironmentControl.fill_cell(22, 30, Global_access.org_colors[11])
        self.EnvironmentControl.fill_cell(23, 30, Global_access.org_colors[12])
        self.EnvironmentControl.fill_cell(24, 30, Global_access.org_colors[13])
        self.EnvironmentControl.fill_cell(25, 30, Global_access.org_colors[14])
        self.EnvironmentControl.fill_cell(26, 30, Global_access.org_colors[15])
        self.EnvironmentControl.fill_cell(27, 30, Global_access.org_colors[16])

    def read_port(self):
        f = open("../config.txt", "r")
        return int(f.read())

    def open_settings(self):
        settings = Setup_settingsGUI.SetupSettings()
        settings.start(self)


if __name__ == "__main__":
    starter = Management()
    starter.start_receiver()
    starter.main_loop()
