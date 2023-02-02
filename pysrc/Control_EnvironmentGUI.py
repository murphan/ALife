from tkinter import *
import pygame

GREEN = '#0c871b'
YELLOW = '#fafa16'
BLACK = '#000000'


class EnvironmentControl:
    def __init__(self, *args, **kwargs):
        """
        This is the initialization of the environment window controls that
        will need to be kept track of throughout the run
        """
        # TODO: Make these correspond to the correct values in setup_environmentGUI
        self.height = 1
        self.width = 1

    def set_speed(self, speed):
        """
        This is where the speed of the environment is set from the settings window

        :param speed: The speed of the environment. Ex. "2x" or ".5x"
        :param type: string
        """
        settings = self.env_settings
        if settings.speed == float(speed[:-1]):
            return
        else:
            settings.speed = float(speed[:-1])
            EnvironmentControl.send_message(self, settings.conn, "Speed", settings.speed)

    def set_size(self, width, height):
        """
        This is where the size is changed together. This may be called at a
        different time from the individual setter functions

        :param width: Width of the window
        :param type: int

        :param height: Height of the window
        :param type: int
        """
        self.height = height
        self.width = width

    def start(self):
        """
        This is where the environment is started up again when it is selected in the environment window
        """
        if self.running:
            return
        else:
            self.running = True
            EnvironmentControl.send_message(self, self.conn, "Control", self.running)

    def stop(self):
        """
        This is the call to stop whenever it is clicked in the environment window
        """
        if not self.running:
            return
        else:
            self.running = False
            EnvironmentControl.send_message(self, self.conn, "Control", self.running)

    def set_width(self, width):
        """
        This is where the width is set and adjusted from the settings window

        :param width: The width of the window
        :Param type: int
        """
        # print(f"width: {width}")
        self.width = width

    def set_height(self, height):
        """
        This is where the height is set and adjusted from the settings window

        :param height: The height of the window
        :param type: int
        """
        # print(f"height: {height}")
        self.height = height

    def set_env_vars(self, temperature, light, oxygen):
        """
        This will set each of the three environment variables

        :param temperature: The temperature of the environment in degrees Celsius
        :param type: int

        :param light: The brightness of the environment in percentage
        :param type: int

        :param oxygen: The oxygen level of the environment in percentage
        :param type: int
        """
        settings = self.env_settings
        if settings.temperature == temperature and settings.light == light and settings.oxygen == oxygen:
            return
        else:
            settings.temperature = temperature if temperature != "" else 0
            settings.light = light if light != "" else 0
            settings.oxygen = oxygen if oxygen != "" else 0
            data = settings.temperature, settings.light, settings.oxygen
            EnvironmentControl.send_message(self, settings.conn, "Environment Variables", data)

    def square_clicked(self, event, SCREEN):
        """
        This will fill in a square when it is clicked in the environment

        :param event: the clicked event used for finding the clicked position
        :param type: event type

        :param SCREEN: the SCREEN parameters from the environment window
        :param type: pygame.display
        """
        mpos_x, mpos_y = event.pos
        coord = mpos_x // 10, mpos_y // 10
        rect = pygame.Rect(coord[0] * 10, coord[1] * 10,
                           10, 10)
        # Check that the coordinates are within the bounds of the environment (only check height)
        if coord[1] > self.environment_size[1]:
            return
        # TODO: Also check if the coordinates are already filled. If so,
        #  request data about that organism if not, send data that this square is now filled
        if self.click_type == 'Organism':
            pygame.draw.rect(SCREEN, GREEN, rect)
        elif self.click_type == 'Food':
            pygame.draw.rect(SCREEN, YELLOW, rect)
        else:
            pygame.draw.rect(SCREEN, BLACK, rect)

    def click_type(self, clicked_type):
        """
        When a square is clicked in the environment, the user can
        specify what they want it to be

        :param clicked_type: The type of square the user will fill with
        :param type: String
        """
        self.env_settings.click_type = clicked_type

    def decode_message(self):
        """
        This will decode the message that is sent from the c++ application. This will be
        started in setup Environment and will be in its own process. It will loop
        infinitely looking for messages

        NOTE: Self was passed from setup_environment and so this will be a setup_environment instance of self
        """
        while True:
            message_buf = ''  # The buffer to append message information to

            message = self.conn.recv(1024).decode()
            if message:
                length = int(message[:4])
                message = message[4:]
                while length:
                    if length < 1022:
                        message_buf += message
                        length = 0
                        break
                    else:
                        message_buf += message
                        length -= 1022
                    message = self.conn.recv(1024).decode()

                print("message received")

            # TODO: Process the commands here

    def send_message(self, conn, message_type, data=None):
        """
        This is the spot where a message can be sent to the c++ application

        NOTE: Self was passed from setup_environment and so this will be a setup_environment instance of self

        :param conn: connection to send the message to
        :param type: socket

        :param message_type: The type of message that will be sent (ex. Control, Request, Environment Variables)
        :param type: string

        :param data: Optional argument for passing data that may be needed in c++
        :param type: Any
        """
        # TODO: Check the message type and create a message that is different based on the type that is given
        message = EnvironmentControl.create_message(self, message_type, data)
        length = len(message)
        if len(str(length)) < 4:
            length = str(length).zfill(4)
            message = str(length).encode("utf-8") + message.encode("utf-8")
        else:
            message = str(length).encode("utf-8") + message.encode("utf-8")

        conn.send(message)

    def create_message(self, message_type, data):
        """
        This function will generate a new message based on the message type and the data that is with it
        :param message_type: The type of message that will be sent (ex. Control, Request, Environment Variables)
        :param type: string

        :param data: Optional argument for passing data that may be needed in c++
        :param type: Any
        """
        if message_type == "Control" or message_type == "Request" \
           or message_type == "Speed" or message_type == "Request All":
            return "{type:" + message_type + ", data:" + str(data) + "}"
        elif message_type == "Environment Variables":
            formatted_data = "{temperature: " + str(data[0]) + ", light: " \
                             + str(data[1]) + ", oxygen: " + str(data[2]) + "}"
            return "{type:" + message_type + ", data:" + formatted_data + "}"
        elif message_type == "New Filled":
            formatted_data = "{x: " + str(data[0]) + "y: " + str(data[1]) + "type" + str(data[2]) + "}"
            return "{type:" + message_type + ", data:" + formatted_data + "}"
