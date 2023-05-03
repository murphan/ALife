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

            fill_cell(x, y, color_0)

            if draw_circle:
                fill_cell(x, y, color_1, circle=True)

            Global_access.ENVIRONMENT_GRID[x][y]["environment"] = cell

            if meta_type == Global_access.TILE_TYPE_ORGANISM:
                Global_access.ENVIRONMENT_GRID[x][y]["organism"] = uuids[organism_index]
            else:
                Global_access.ENVIRONMENT_GRID[x][y]["organism"] = None


def fill_cell(x: int, y: int, color: (int, int, int), circle: bool = False):
    y = int(Global_access.environment_size[1] - y - 1)
    cell_size = float(Global_access.ENVIRONMENT_BOX[2]) / Global_access.environment_size[0]

    if circle:
        pygame.draw.circle(
            Global_access.SCREEN,
            color,
            (
                Global_access.ENVIRONMENT_BOX[0] + x * cell_size + (cell_size / 2),
                Global_access.ENVIRONMENT_BOX[1] + y * cell_size + (cell_size / 2),
            ),
            (cell_size / 2)
        )
    else:
        pygame.draw.rect(
            Global_access.SCREEN,
            color,
            (
                Global_access.ENVIRONMENT_BOX[0] + x * cell_size,
                Global_access.ENVIRONMENT_BOX[1] + y * cell_size,
                cell_size,
                cell_size,
            )
        )
