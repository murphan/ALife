from enum import Enum


class Noise(object):
    def __init__(self, use_noise: bool, center: float, scale: float, depth: float, speed: float):
        self.use_noise = use_noise
        self.center = center
        self.scale = scale
        self.depth = depth,
        self.speed = speed


class Factor(Enum):
    LIGHT = 0
