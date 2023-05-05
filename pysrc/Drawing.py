import pygame

import Environment_cell
import Global_access

RenderInfo = {"size": (int, int), "buffer": bytes, "uuids": list[str]}


def render_grid(render_info: RenderInfo):
    BYTES_PER_TILE = 9

    width, height = render_info["size"]
    buffer = render_info["buffer"]
    uuids = render_info["uuids"]

    Global_access.define_grid(width, height)

    # precompute bounds
    lefts: list[int] = []
    ups: list[int] = []

    runner = 0.0
    for x in range(Global_access.grid_width):
        lefts.append(int(runner))
        runner += float(Global_access.ENVIRONMENT_BOX.width) / float(Global_access.grid_width)
    lefts.append(Global_access.ENVIRONMENT_BOX.width)

    runner = 0.0
    for y in range(Global_access.grid_height):
        ups.append(int(runner))
        runner += float(Global_access.ENVIRONMENT_BOX.height) / float(Global_access.grid_height)
    ups.append(Global_access.ENVIRONMENT_BOX.height)

    for y in range(height):
        for x in range(width):
            baseIndex = (y * width + x) * BYTES_PER_TILE

            meta_byte      = int.from_bytes(buffer[baseIndex + 0:baseIndex + 1], "big", signed=False)
            organism_index = int.from_bytes(buffer[baseIndex + 1:baseIndex + 3], "big", signed=False)
            color_0        = int.from_bytes(buffer[baseIndex + 3:baseIndex + 6], "big", signed=False)
            color_1        = int.from_bytes(buffer[baseIndex + 6:baseIndex + 9], "big", signed=False)

            draw_circle = meta_byte >> 7 == 1
            meta_type = meta_byte & 0x7f

            cell = Environment_cell.EnvironmentCell(0, 0, 0, meta_type, 0, 0, 0)

            fill_cell(x, y, lefts, ups, color_0)

            if draw_circle:
                fill_cell(x, y, lefts, ups, color_1, circle=True)

            Global_access.ENVIRONMENT_GRID[x][y]["environment"] = cell

            if meta_type == Global_access.TILE_TYPE_ORGANISM:
                Global_access.ENVIRONMENT_GRID[x][y]["organism"] = uuids[organism_index]
            else:
                Global_access.ENVIRONMENT_GRID[x][y]["organism"] = None


def fill_cell(grid_x: int, grid_y: int, lefts: list[int], ups: list[int], color: int, circle: bool = False):
    y = Global_access.grid_height - grid_y - 1
    x = grid_x

    left = Global_access.ENVIRONMENT_BOX.x + lefts[x]
    right = Global_access.ENVIRONMENT_BOX.x + lefts[x + 1]

    up = Global_access.ENVIRONMENT_BOX.y + ups[y]
    down = Global_access.ENVIRONMENT_BOX.y + ups[y + 1]

    if circle:
        pygame.draw.circle(
            surface=Global_access.SCREEN,
            color=color,
            center=(
                (left + right) / 2.0,
                (down + up) / 2.0,
            ),
            radius=(float(right - left) / 2.0)
        )
    else:
        pygame.draw.rect(
            surface=Global_access.SCREEN,
            color=color,
            rect=(
                left,
                up,
                right - left,
                down - up,
            )
        )
