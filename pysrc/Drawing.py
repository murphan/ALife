import pygame

import Environment_cell
import Global_access
from collections import deque
import copy

Node = [int, int, int, bool, bool, int, list, (int, int)]

Tree = {"levelTotals": list[int], "root": Node}

RenderInfo = {"size": (int, int), 'grid': bytes, "ids": list[int], "tree": Tree}

BYTES_PER_TILE = 9


def render(render_info: RenderInfo):
    render_copy = copy.deepcopy(render_info)

    width, height = render_copy["size"]

    Global_access.define_grid(width, height)
    Global_access.tree_nodes = []

    if 'grid' in render_copy:
        render_grid(Global_access.SCREEN, Global_access.ENVIRONMENT_BOX, width, height, render_copy["grid"], render_copy["ids"])
    elif 'tree' in render_copy:
        render_tree(Global_access.SCREEN, Global_access.TREE_BOX, render_copy['tree'])


def render_grid(screen: pygame.Surface, bounds: pygame.Rect, width: int, height: int, grid: bytes, ids: list[int]):
    # precompute bounds
    lefts: list[int] = []
    ups: list[int] = []

    runner = 0.0
    for x in range(width):
        lefts.append(int(runner))
        runner += bounds.width / width
    lefts.append(bounds.width)

    runner = 0.0
    for y in range(height):
        ups.append(int(runner))
        runner += bounds.height / height
    ups.append(bounds.height)

    for y in range(height):
        for x in range(width):
            baseIndex = (y * width + x) * BYTES_PER_TILE

            meta_byte      = int.from_bytes(grid[baseIndex + 0:baseIndex + 1], "big", signed=False)
            organism_index = int.from_bytes(grid[baseIndex + 1:baseIndex + 3], "big", signed=False)
            color_0        = int.from_bytes(grid[baseIndex + 3:baseIndex + 6], "big", signed=False)
            color_1        = int.from_bytes(grid[baseIndex + 6:baseIndex + 9], "big", signed=False)

            draw_circle = meta_byte >> 7 == 1
            meta_type = meta_byte & 0x7f

            cell = Environment_cell.EnvironmentCell(0, 0, 0, meta_type, 0, 0, 0)

            fill_cell(screen, bounds, width, height, x, y, lefts, ups, color_0, False)

            if draw_circle:
                fill_cell(screen, bounds, width, height, x, y, lefts, ups, color_1, True)

            Global_access.ENVIRONMENT_GRID[x][y]["environment"] = cell

            if meta_type == Global_access.TILE_TYPE_ORGANISM:
                Global_access.ENVIRONMENT_GRID[x][y]["organism"] = ids[organism_index]
            else:
                Global_access.ENVIRONMENT_GRID[x][y]["organism"] = None


def fill_cell(
    screen: pygame.Surface,
    bounds: pygame.Rect,
    width: int,
    height: int,
    grid_x: int,
    grid_y: int,
    lefts: list[int],
    ups: list[int],
    color: int,
    circle: bool,
):
    y = height - grid_y - 1
    x = grid_x

    left = bounds.x + lefts[x]
    right = bounds.x + lefts[x + 1]

    up = bounds.y + ups[y]
    down = bounds.y + ups[y + 1]

    if circle:
        pygame.draw.circle(
            surface=screen,
            color=color,
            center=(
                (left + right) / 2.0,
                (down + up) / 2.0,
            ),
            radius=(float(right - left) / 2.0)
        )
    else:
        pygame.draw.rect(
            surface=screen,
            color=color,
            rect=(
                left,
                up,
                right - left,
                down - up,
            )
        )


def circle_in_box(box: pygame.Rect, index: int, num_circles: int) -> (float, float):
    along = (index + 1.0) / (num_circles + 1.0)
    x = box.x + box.width * along
    y = box.y + box.height / 2
    return x, y


def this_box(level_totals: list[int], levels_already_inserted: list[int], value: int, level: int, level_height: float, width: float) -> pygame.Rect:
    left_along = levels_already_inserted[level] / level_totals[level]
    width_along = value / level_totals[level]

    return pygame.Rect(left_along * width, (len(level_totals) - level - 1) * level_height, width_along * width, level_height)


def render_tree(screen: pygame.Surface, bounds: pygame.Rect, tree_json: Tree):
    pygame.draw.rect(screen, 0x000000, bounds)

    level_totals: list[int] = tree_json["levelTotals"]

    levels_already_inserted = [0 for _ in level_totals]

    level_height = bounds.height / len(level_totals)

    traverse_queue = deque()
    traverse_queue.appendleft(tree_json["root"])

    while len(traverse_queue) > 0:
        node: Node = traverse_queue.pop()

        uuid = node[0]
        value = node[1]
        level = node[2]
        alive = node[3]
        active = node[4]
        color = node[5]
        children = node[6]
        parent_position = node[7] if len(node) == 8 else None

        box = this_box(level_totals, levels_already_inserted, value, level, level_height, bounds.width)

        x = bounds.x + box.x + box.width / 2
        y = bounds.y + box.y + box.height / 2

        if parent_position is not None:
            pygame.draw.line(screen, color, (x, y), parent_position, width=2)

        pygame.draw.circle(screen, color, (x, y), 8)

        Global_access.tree_nodes.append(Global_access.TreeNode(
            pygame.Rect(x - 8, y - 8, 16, 16),
            uuid,
            True if level == 0 else active
        ))

        levels_already_inserted[level] += value

        for child_node in children:
            if len(child_node) == 8:
                child_node[7] = (x, y)
            else:
                child_node.append((x, y))

            traverse_queue.appendleft(child_node)


def blackout_top():
    pygame.draw.rect(
        Global_access.SCREEN,
        Global_access.BLACK,
        pygame.Rect(0, 0, Global_access.WINDOW_WIDTH, Global_access.WINDOW_HEIGHT - Global_access.BOTTOM_BUFFER)
    )


def blackout_bottom():
    pygame.draw.rect(
        Global_access.SCREEN,
        Global_access.BLACK,
        pygame.Rect(0, Global_access.WINDOW_HEIGHT - Global_access.BOTTOM_BUFFER, Global_access.WINDOW_WIDTH, Global_access.BOTTOM_BUFFER)
    )


def blackout_message_area():
    pygame.draw.rect(
        Global_access.SCREEN,
        Global_access.BLACK,
        pygame.Rect(120, Global_access.WINDOW_HEIGHT - Global_access.BOTTOM_BUFFER, Global_access.WINDOW_WIDTH - 650,
                    Global_access.BOTTOM_BUFFER)
    )
