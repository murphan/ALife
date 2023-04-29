import pygame.event
from typing import Callable


class Slider(object):
    def __init__(self, low: float, high: float, get_value: Callable[[], int], set_value: Callable[[int], None]):
        self.handle_width = 0
        self.low = low
        self.high = high
        self.dragging = False
        self.get_value = get_value
        self.set_value = set_value

        self.slider_box = pygame.Rect(0, 0, 0, 0)

    def render(
        self,
        screen: pygame.Surface,
        label: str,
        bounding_box: pygame.Rect,
        font_size: int,
        text_area_width: int,
        handle_width: int,
        bar_height: int,
        gutter: (int, int),
    ):
        self.handle_width = handle_width

        self.slider_box = pygame.Rect(
            bounding_box.x + text_area_width,
            bounding_box.y + gutter[1],
            bounding_box.width - text_area_width - gutter[0],
            bounding_box.height - (gutter[1] * 2)
        )

        # draw bounding box
        pygame.draw.rect(screen, 0x887894, bounding_box)

        # draw text
        font = pygame.font.SysFont('Arial', font_size)
        rendered_text = font.render(f'{label} {self.get_value()}', True, (0, 0, 0))
        text_rect = rendered_text.get_rect()
        screen.blit(source=rendered_text, dest=(
            bounding_box.x + (text_area_width / 2) - (text_rect.width / 2),
            bounding_box.y + (bounding_box.height / 2) - (text_rect.height / 2),
            text_rect.width,
            text_rect.height,
        ))

        # draw bar
        pygame.draw.rect(screen, 0x000000, (
            self.slider_box.x,
            self.slider_box.y + (self.slider_box.height / 2) - (bar_height / 2),
            self.slider_box.width,
            bar_height,
        ))

        # draw handle
        pygame.draw.rect(screen, 0x842cc7, (
            self.slider_x(),
            self.slider_box.y,
            self.handle_width,
            self.slider_box.height,
        ))

    def update(self, event: pygame.event.Event) -> bool:
        """
        :return: did it update
        """
        if event.type == pygame.MOUSEBUTTONDOWN:
            mouse_x, mouse_y = pygame.mouse.get_pos()
            if self.slider_box.collidepoint(mouse_x, mouse_y):
                self.dragging = True
                self.update_position(mouse_x)
                return True

        elif event.type == pygame.MOUSEBUTTONUP:
            self.dragging = False

        elif event.type == pygame.MOUSEMOTION:
            if self.dragging:
                mouse_x, _ = pygame.mouse.get_pos()
                self.update_position(mouse_x)
                return True

        return False

    def update_position(self, mouse_x: int):
        self.set_value(int(Slider.interp(
            self.low, self.high, Slider.clamp(
                self.inverse_interp(self.slider_left(), self.slider_right(), mouse_x)
            )
        )))

    def slider_left(self) -> float:
        return self.slider_box.x + (self.handle_width / 2)

    def slider_right(self) -> float:
        return self.slider_box.x + (self.slider_box.width - self.handle_width / 2)

    def slider_x(self) -> float:
        along = Slider.clamp(Slider.inverse_interp(self.low, self.high, self.get_value()))
        return Slider.interp(self.slider_left(), self.slider_right(), along) - (self.handle_width / 2)

    # utility

    @staticmethod
    def interp(low: float, high: float, along: float): return along * (high - low) + low

    @staticmethod
    def inverse_interp(low: float, high: float, value: float): return (value - low) / (high - low)

    @staticmethod
    def clamp(n: float): return max(0.0, min(n, 1.0))
