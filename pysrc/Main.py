import os
import socket
import sys
import time
from threading import Thread
from tkinter import messagebox

# Hides a console message from the pygame module
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'
import pygame

from Control_EnvironmentGUI import EnvironmentControl
from Setup_EnvironmentGUI import EnvironmentGUI
import Setup_settingsGUI
import Global_access
import receive_message
import Drawing


class Management:
    def __init__(self):
        self.conn = None
        self.thread = None
        self.create_connection()

        self.environment_control = EnvironmentControl()
        self.environment_gui = EnvironmentGUI(lambda fps: self.environment_control.set_fps(self.conn, fps))

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
        self.environment_control.send_message(self.conn, "init")

    def main_loop(self):
        while True:
            if Global_access.EXIT == 1:
                self.conn.close()
                pygame.quit()
                sys.exit()

            should_render_ui = False

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    thread = Thread(target=self.exiting)
                    thread.start()
                if event.type == pygame.MOUSEBUTTONDOWN:
                    if self.environment_gui.settings_button.mouse_click(event):  # Show the settings window
                        thread = Thread(target=self.open_settings)
                        thread.start()
                    elif self.environment_gui.play_pause_button.mouse_click(event):  # start button was clicked
                        self.environment_control.toggle_start_stop(self.conn)
                        should_render_ui = True
                    else:  # Display on the environment that a square was clicked
                        self.environment_control.square_clicked(event, self.environment_gui, self.conn)
                if event.type == pygame.VIDEORESIZE:
                    self.environment_gui.on_window_resize(event)
                if self.environment_gui.fps_slider.update(event):
                    should_render_ui = True

            if (Global_access.latest_frame is not None and Global_access.latest_frame["should_render"])\
                    or should_render_ui:
                Global_access.SCREEN.fill(Global_access.BLACK)
                Drawing.render_grid(Global_access.latest_frame)
                self.environment_gui.render_buttons()
                self.environment_gui.render_fps_slider()
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
