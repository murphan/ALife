import os
import socket
import sys
import time
from threading import Thread
from tkinter import messagebox

# Hides a console message from the pygame module
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'
import pygame

import Control_EnvironmentGUI
import Setup_EnvironmentGUI
import Setup_settingsGUI
import Global_access
import receive_message
import Drawing

class Management:
    def __init__(self, *args, **kwargs):
        self.EnvironmentGui = Setup_EnvironmentGUI.SetupEnvironment()
        self.EnvironmentControl = Control_EnvironmentGUI.EnvironmentControl()

        self.create_connection()

        # TODO call whenever new connection is established
        self.send_init_message()

    def create_connection(self):
        # Set up the socket connection to the c++ application
        connected = False
        while not connected:
            try:
                host = socket.gethostname()
                port = self.read_port()
                self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.conn.connect((host, port))
                connected = True
            except socket.error:
                continue

    def start_receiver(self):
        self.thread = Thread(target=receive_message.decode_message, args=(self.conn,))
        self.thread.start()

    def send_init_message(self):
        self.EnvironmentControl.send_message(self.conn, "init")

    def main_loop(self):
        while True:
            if Global_access.EXIT == 1:
                self.conn.close()
                pygame.quit()
                sys.exit()

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    thread = Thread(target=self.exiting)
                    thread.start()
                if event.type == pygame.MOUSEBUTTONDOWN:
                    if self.EnvironmentGui.settings_button.mouse_click(event):  # Show the settings window
                        thread = Thread(target=self.open_settings)
                        thread.start()
                    elif self.EnvironmentGui.play_pause_button.mouse_click(event):  # start button was clicked
                        self.EnvironmentControl.toggle_start_stop(self.conn)
                        self.EnvironmentGui.create_buttons()
                    else:  # Display on the environment that a square was clicked
                        self.EnvironmentControl.square_clicked(event, self.EnvironmentGui, self.conn)
                if event.type == pygame.VIDEORESIZE:
                    self.EnvironmentGui.on_window_resize(event)

            if Global_access.latest_frame is not None and Global_access.latest_frame["should_render"]:
                Global_access.SCREEN.fill(Global_access.BLACK)
                Drawing.render_grid(Global_access.latest_frame)
                self.EnvironmentGui.create_buttons()
                Global_access.latest_frame["should_render"] = False

            pygame.display.update()

            time.sleep(0.01)

    def exiting(self):
        confirm = messagebox.askokcancel(title="Quit?", message="Are you sure that you want to Quit?")
        if confirm:
            Global_access.EXIT = 1
        else:
            return

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
