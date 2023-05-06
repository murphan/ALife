import socket
from typing import Any

import pygame
from collections import deque
import Global_access
from send_message import send_message


def toggle_show_tree(conn: socket.socket):
    if Global_access.tree is None:
        send_message(conn, "tree")
    else:
        Global_access.tree = None


def circle_in_box(box: pygame.Rect, index: int, num_circles: int) -> (float, float):
    along = (index + 1.0) / (num_circles + 1.0)
    x = box.x + box.width * along
    y = box.y + box.height / 2
    return x, y


def this_box(level_totals: list[int], levels_already_inserted: list[int], value: int, level: int, level_height: float, width: float) -> pygame.Rect:
    left_along = levels_already_inserted[level] / level_totals[level]
    width_along = value / level_totals[level]

    return pygame.Rect(left_along * width, (len(level_totals) - level - 1) * level_height, width_along * width, level_height)


def draw(screen: pygame.Surface, bounds: pygame.Rect, tree_json: Any):
    if tree_json is None:
        return

    pygame.draw.rect(screen, 0x000000, bounds)

    level_totals: list[int] = tree_json["levelTotals"]

    levels_already_inserted = [0 for _ in level_totals]

    level_height = bounds.height / len(level_totals)

    traverse_queue = deque()
    traverse_queue.appendleft(tree_json["root"])

    while len(traverse_queue) > 0:
        node: list = traverse_queue.pop()

        this_value = node[0]
        this_level = node[1]
        this_alive = node[2]
        this_color = node[3]
        this_children = node[4]
        this_parent = node[5] if len(node) == 6 else None

        box = this_box(level_totals, levels_already_inserted, this_value, this_level, level_height, bounds.width)

        x = bounds.x + box.x + box.width / 2
        y = bounds.y + box.y + box.height / 2

        if this_parent is not None:
            pygame.draw.line(screen, this_color, (x, y), (this_parent[0], this_parent[1]), width=2)

        pygame.draw.circle(screen, this_color, (x, y), 8)

        levels_already_inserted[this_level] += this_value

        for child_node in this_children:
            if len(child_node) == 5:
                child_node.append((x, y))
            traverse_queue.appendleft(child_node)
