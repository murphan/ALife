import pygame


class Button(object):
    """
    The button class which defines the buttons in the environment
    """
    def __init__(self):
        self.bounding_box = pygame.Rect(0, 0, 0, 0)

    def render(
        self,
        screen,
        bounding_box: pygame.rect,
        text: str,
        font: pygame.font,
        color: int,
    ):
        rendered_text = font.render(text, True, (0, 0, 0))
        text_rect = rendered_text.get_rect()

        self.bounding_box = bounding_box

        pygame.draw.rect(screen, color, bounding_box)
        screen.blit(
            source=rendered_text,
            dest=(
                bounding_box.x + (bounding_box.width - text_rect.width) / 2,
                bounding_box.y + (bounding_box.height - text_rect.height) / 2
            )
        )

    def update(self, event: pygame.event):
        """
        Checks if you click the mouse button and then if it's on the button
        """
        if event.type == pygame.MOUSEBUTTONDOWN:
            if pygame.mouse.get_pressed()[0]:
                x, y = pygame.mouse.get_pos()
                if self.bounding_box.collidepoint(x, y):
                    return True

        return False
