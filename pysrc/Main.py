import os
import socket
import sys
from threading import Thread
from tkinter import messagebox

# Hides a console message from the pygame module
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'
import pygame

import Drawing
import EnvironmentControl
import Global_access
import Setup_settingsGUI
import receive_message
from EnvironmentGUI import EnvironmentGUI
from send_message import send_message


class Management:
    def __init__(self):
        self.conn = None
        self.thread = None
        self.create_connection()

        self.environment_gui = EnvironmentGUI()

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

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    Thread(target=Management.exiting).start()

                if event.type == pygame.VIDEORESIZE:
                    self.environment_gui.on_window_resize(event)
                    Global_access.set_should_render(True)

                if self.environment_gui.settings_button.update(event):
                    Thread(target=self.open_settings).start()

                if self.environment_gui.play_pause_button.update(event):
                    EnvironmentControl.toggle_start_stop(self.conn)

                fps_value = self.environment_gui.fps_slider.update(event)
                if fps_value is not None:
                    EnvironmentControl.set_fps(self.conn, int(fps_value))

                if self.environment_gui.tree_button.update(event):
                    EnvironmentControl.toggle_display_mode(self.conn)

                if self.environment_gui.reset_button.update(event):
                    send_message(self.conn, 'reset')

                if self.environment_gui.do_highlight_button.update(event):
                    EnvironmentControl.toggle_do_highlight(self.conn)

                if self.environment_gui.select_mode_button.update(event):
                    EnvironmentControl.toggle_select_mode(self.conn)

                if event.type == pygame.MOUSEBUTTONDOWN:
                    EnvironmentControl.square_clicked(event, self.conn)

                EnvironmentControl.click_tree(self.conn, event)

            should_render_environment = Global_access.latest_frame is not None and\
                Global_access.latest_frame["should_render"]

            if should_render_environment:
                Drawing.blackout_top()
                Drawing.render(Global_access.latest_frame)

                Drawing.blackout_bottom()
                self.environment_gui.render_ui()

                Global_access.set_should_render(False)

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
