
import pygame
import Global_access

g = Global_access


def box(width: int, height: int) -> pygame.Rect:
    return pygame.Rect(0, 0, width, height)


def from_bottom_right(box: pygame.Rect, x: int, y: int) -> pygame.Rect:
    return pygame.Rect(g.WINDOW_WIDTH - box.width - x, g.WINDOW_HEIGHT - box.height - y, box.width, box.height)


def left_of(ref: pygame.Rect, box: pygame.Rect, x: int) -> pygame.Rect:
    return pygame.Rect(ref.x - box.width - x, ref.y, box.width, box.height)
