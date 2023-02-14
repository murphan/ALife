import os
from threading import Thread
from multiprocessing import Process
import sys
import socket
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

    def start_receiver(self):
        self.thread = Thread(target=receive_message.decode_message, args=(self, self.conn,))
        self.thread.start()

    def main_loop(self):
        while True:
            self.EnvironmentGui.drawGrid()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    Global_access.STOPPED = 1
                    self.conn.close()
                    sys.exit()
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
