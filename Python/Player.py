class Player:
    def __init__(self, row: int, col: int, table_rows: int, table_cols: int) -> None:
        self.row: int = row
        self.col: int = col
        self.table_rows: int = table_rows
        self.table_cols: int = table_cols

    def move(self, deltaRow: int, deltaCol: int) -> None:
        self.row = max(0, min(self.table_rows - 1, self.row + deltaRow))
        self.col = max(0, min(self.table_cols - 1, self.col + deltaCol))
