import os
import socket
import sys
from threading import Thread
from tkinter import messagebox

# Hides a console message from the pygame module
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'
import pygame

import EnvironmentControl
from Setup_EnvironmentGUI import EnvironmentGUI
import Setup_settingsGUI
import Global_access
import receive_message
import Drawing
from send_message import send_message


class Management:
    def __init__(self):
        self.conn = None
        self.thread = None
        self.create_connection()

        self.environment_gui = EnvironmentGUI(lambda fps: EnvironmentControl.set_fps(self.conn, fps))

        Global_access.ENV_FONT = pygame.font.SysFont('Arial', 20)

        # TODO call whenever new connection is established
        self.send_init_message()

    def create_connection(self):
        # Set up the socket connection to the c++ application
        connected = False
        while not connected:
            try:
                host = socket.gethostname()
                port = Management.read_port()
                self.conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.conn.connect((host, port))
                connected = True
            except socket.error:
                continue

    def start_receiver(self):
        self.thread = Thread(target=receive_message.decode_message, args=(self.conn,))
        self.thread.start()

    def send_init_message(self):
        send_message(self.conn, "init")

    def main_loop(self):
        while True:
            if Global_access.EXIT == 1:
                self.conn.close()
                pygame.quit()
                sys.exit()

            should_render_ui = False

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    Thread(target=Management.exiting).start()

                if event.type == pygame.VIDEORESIZE:
                    self.environment_gui.on_window_resize(event)

                if self.environment_gui.settings_button.update(event):
                    Thread(target=self.open_settings).start()

                if self.environment_gui.play_pause_button.update(event):
                    EnvironmentControl.toggle_start_stop(self.conn)
                    should_render_ui = True

                if self.environment_gui.fps_slider.update(event):
                    should_render_ui = True

            should_render_environment = Global_access.latest_frame is not None and\
                Global_access.latest_frame["should_render"]

            if should_render_environment:
                Global_access.SCREEN.fill(Global_access.BLACK)
                Drawing.render_grid(Global_access.latest_frame)
                Global_access.latest_frame["should_render"] = False

            if should_render_environment or should_render_ui:
                self.environment_gui.render_ui()

            pygame.display.update()


    @staticmethod
    def exiting():
        confirm = messagebox.askokcancel(title="Quit?", message="Are you sure that you want to Quit?")
        if confirm:
            Global_access.EXIT = 1
        else:
            return

    @staticmethod
    def read_port():
        f = open("../config.txt", "r")
        return int(f.read())

    def open_settings(self):
        settings = Setup_settingsGUI.SetupSettings(self.conn)
        settings.start()


if __name__ == "__main__":
    starter = Management()
    starter.start_receiver()
    starter.main_loop()
