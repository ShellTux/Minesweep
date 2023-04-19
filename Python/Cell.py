class Cell:
    def __init__(self, *, open: bool = False, flag: bool = False, bomb: bool = False, neighbors: int = 0) -> None:
        self.open: bool     = open
        self.flag: bool     = flag
        self.bomb: bool     = bomb
        self.neighbors: int = neighbors
