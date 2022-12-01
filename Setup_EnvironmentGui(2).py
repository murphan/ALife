import tkinter
from tkinter import *
import arcade
import arcade.gui

# Set how many rows and columns we will have
ROW_COUNT = 44
COLUMN_COUNT = 89

# This sets the WIDTH and HEIGHT of each grid location
WIDTH = 12
HEIGHT = 12

# This sets the margin between each cell
# and on the edges of the screen.
MARGIN = 5

# Do the math to figure out our screen dimensions
SCREEN_WIDTH = (WIDTH + MARGIN) * COLUMN_COUNT + MARGIN
SCREEN_HEIGHT = (HEIGHT + MARGIN) * ROW_COUNT + MARGIN
SCREEN_TITLE = "A-Life Challenge"


class MyGame(arcade.Window):
    """
    Main application class.
    """

    def __init__(self, width, height, title):
        """
        Set up the application.
        """
        super().__init__(width, height, title)

        self.manager = arcade.gui.UIManager()
        self.manager.enable()

        # Create a 2 dimensional array.
        self.grid = []
        for row in range(ROW_COUNT):
            self.grid.append([])
            for column in range(COLUMN_COUNT):
                self.grid[row].append(0)

        arcade.set_background_color(arcade.color.GRAY)

        # # Create a vertical BoxGroup to align buttons
        # self.v_box = arcade.gui.UIBoxLayout()
        #
        # # Create the buttons
        # start_button = arcade.gui.UIFlatButton(text="Start Game", width=200)
        # self.v_box.add(start_button.with_space_around(bottom=20))
        #
        # self.manager.add(
        #     arcade.gui.UIAnchorWidget(
        #         anchor_x="center_x",
        #         anchor_y="center_y",
        #         child=self.v_box)
        # )

        column = int(684 // (WIDTH + MARGIN))
        row = int(654 // (HEIGHT + MARGIN))

        # self.grid[row][column] = 1
        # self.organism = [row, column]

    def create_organism(self):
        column = int(684 // (WIDTH + MARGIN))
        row = int(654 // (HEIGHT + MARGIN))

        self.grid[row][column] = 1
        self.organism = [row, column]

        # arcade.draw_rectangle_filled(column, row, WIDTH, HEIGHT, color)

    def on_draw(self):
        """
        Render the screen.
        """
        # This command has to happen before we start drawing
        self.clear()

        # Draw the grid
        for row in range(ROW_COUNT):
            for column in range(COLUMN_COUNT):
                if self.grid[row][column] == 1:
                    color = arcade.color.RED
                else:
                    color = arcade.color.WHITE

                # Do the math to figure out where the box is
                x = (MARGIN + WIDTH) * column + MARGIN + WIDTH // 2
                y = (MARGIN + HEIGHT) * row + MARGIN + HEIGHT // 2

                # Draw the box
                arcade.draw_rectangle_filled(x, y, WIDTH, HEIGHT, color)

        self.manager.draw()
        # self.create_organism()

    def on_mouse_press(self, x, y, button, modifiers):
        """
        Called when the user presses a mouse button.
        """

        self.grid[self.organism[0]][self.organism[1]] = 0
        self.organism[0] -= 1
        self.grid[self.organism[0]][self.organism[1]] = 1
        # # Change the x/y screen coordinates to grid coordinates
        # column = int(x // (WIDTH + MARGIN))
        # row = int(y // (HEIGHT + MARGIN))
        #
        # # Make sure we are on-grid. It is possible to click in the upper right
        # # corner in the margin and go to a grid location that doesn't exist
        # if row < ROW_COUNT and column < COLUMN_COUNT:
        #
        #     # Flip the location between 1 and 0.
        #     if self.grid[row][column] == 0:
        #         self.grid[row][column] = 1
        #     else:
        #         self.grid[row][column] = 0



def main():
    window = Tk()
    window.title("A-Life Challenge")
    window.state('zoomed')
    # window.mainloop()

    MyGame(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE)
    arcade.run()


if __name__ == "__main__":
    main()
