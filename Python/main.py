#!/usr/bin/env python3

from Player import Player
from Cell import Cell
from terminal import *
import sys
import tty
import termios
from random import choice

ROWS: int            = 10
COLS: int            = 10
NUMBER_OF_BOMBS: int = 10
first_player: bool   = True

class KeyboardListener:
    def __enter__(self):
        self.fd = sys.stdin.fileno()
        self.old_settings = termios.tcgetattr(self.fd)
        tty.setcbreak(sys.stdin.fileno())
        return self
    
    def __exit__(self, type, value, traceback):
        termios.tcsetattr(self.fd, termios.TCSADRAIN, self.old_settings)

    def get_key(self):
        return sys.stdin.read(1)

def validPosition(i: int, j: int) -> bool:
    return (0 <= i < ROWS) and (0 <= j < COLS)

def createTable() -> list[list[Cell]]:
    table = [ [ 
               Cell( open = False,
                    flag = False,
                    bomb = False,
                    neighbors = 0,
                    ) for j in range(COLS)] for i in range(ROWS)
             ]
    for i in range(ROWS):
        row = []
        for j in range(COLS):
            row.append(Cell(
                open = False,
                flag = False,
                bomb = False,
                neighbors = 0,
                ))
        table.append(row)
    return table

def initializeTable(table: list[list[Cell]], player: Player) -> None:
    all_positions: list[tuple[int, int]] = [
            (i, j) for i in range(ROWS) for j in range(COLS)
            if (player.row != i or player.col != j) and (abs(i - player.row) + abs(j - player.row) > 2)
            ]
    n_bombs: int = NUMBER_OF_BOMBS

    while n_bombs > 0:
        i, j = choice(all_positions)

        table[i][j].bomb = True

        for deltaRow in (-1, 0, 1):
            for deltaCol in (-1, 0, 1):
                if not validPosition(i + deltaRow, j + deltaCol):
                    continue

                table[i + deltaRow][j + deltaCol].neighbors += 1
        n_bombs -= 1
        pass
    return

def getCharacterForCell(cell: Cell) -> str:
    c: str = '.'

    if cell.open:
        if cell.bomb:
            return f'{COLOR_RED}*{COLOR_RESET}'

        return ' ' if cell.neighbors == 0 else str(cell.neighbors)
    elif cell.flag:
        return f'{COLOR_YELLOW}?{COLOR_RESET}'

    return c

def printTable(table: list[list[Cell]], player: Player) -> None:
    openSum: int = countOpen(table)
    flagSum: int = countFlag(table)

    print(CLEAR, GOTO_1_1, end = '', sep = '')
    print(f'{COLOR_CYAN}Number of Bombs: {NUMBER_OF_BOMBS}',
          f'{COLOR_MAGENTA}Openned cells: {openSum}',
          f'{COLOR_YELLOW}Flagged cells: {flagSum}',
          sep = f'{COLOR_RESET} | ',
          end = f'{COLOR_RESET}\n'
          )

    for i in range(ROWS):
        for j in range(COLS):
            c = getCharacterForCell(table[i][j])
            if (player.row == i and player.col == j):
                print(f'[{c}]', end = '')
            else:
                print(f' {c} ', end = '')
        print()

    # TODO: Counting is not working
    if (openSum + flagSum == ROWS * COLS and flagSum == NUMBER_OF_BOMBS):
        print(f'{COLOR_GREEN}You won!{COLOR_RESET}')
        exit(0)

def openTableAtCursor(table: list[list[Cell]], player: Player) -> None:
    cell: Cell = table[player.row][player.col]

    if cell.open:
        return

    if cell.bomb:
        lost(table, player)

    if cell.flag:
        cell.flag = False

    if cell.neighbors == 0:
        openEmptyRegion(table, player.row, player.col)

    cell.open = True

def openEmptyRegion(table: list[list[Cell]], i: int, j: int) -> None:
    current: Cell = table[i][j]

    if (current.open or current.bomb or current.flag):
        return

    current.open = True

    if current.neighbors > 0:
        return


    neighbors_to_check: list[tuple[int, int]] = [
            (-1, 0), # Above
            ( 1, 0), # Bellow
            ( 0,-1), # Left
            ( 0, 1), # Right
            # Corners
            (-1, -1), # Above
            (-1,  1), # Below
            ( 1, -1), # Left
            ( 1,  1), # Right
            ]

    for neighbor in neighbors_to_check:
        deltaRow, deltaCol = neighbor

        newI: int = i + deltaRow
        newJ: int = j + deltaCol

        if not validPosition(newI, newJ):
            continue

        openEmptyRegion(table, newI, newJ)

def flagTableAtCursor(table: list[list[Cell]], player: Player) -> None:
    cell: Cell = table[player.row][player.col]

    if cell.open:
        return

    table[player.row][player.col].flag = not table[player.row][player.col].flag

def countOpen(table: list[list[Cell]]) -> int:
    openSum: int = 0

    for i in range(ROWS):
        for j in range(COLS):
            openSum += int(table[i][j].open)

    return openSum

def countFlag(table: list[list[Cell]]) -> int:
    flagSum: int = 0

    for i in range(ROWS):
        for j in range(COLS):
            flagSum += int(table[i][j].flag)

    return flagSum

def lost(table: list[list[Cell]], player: Player) -> None:
    for i in range(ROWS):
        for j in range(COLS):
            cell: Cell = table[i][j]

            if cell.bomb:
                table[i][j].open = True

    printTable(table, player)
    print(f'{COLOR_RED}You Lost{COLOR_RESET}')
    exit(1)

if __name__ == '__main__':
    for index, argument in enumerate(sys.argv):
        if argument == '--help':
            print('''Use minesweeper-c [OPTION] [VALUE]
  --help    Print this help page
  --bombs   Number of bombs (integer)
  --rows    Number of table rows (integer)
  --cols    Number of table cols (integer)''')
            exit(0)
        elif argument == '--rows':
            try:
                ROWS = int(sys.argv[index + 1])
            except ValueError:
                print(f'{sys.argv[index + 1]} is not an integer', file = sys.stderr)
                exit(1)
            except IndexError:
                print('Number of Rows not provided', file = sys.stderr)
                exit(1)
        elif argument == '--cols':
            try:
                COLS = int(sys.argv[index + 1])
            except ValueError:
                print(f'{sys.argv[index + 1]} is not an integer', file = sys.stderr)
                exit(1)
            except IndexError:
                print('Number of Cols not provided', file = sys.stderr)
                exit(1)
        elif argument == '--bombs':
            try:
                NUMBER_OF_BOMBS = int(sys.argv[index + 1])
            except ValueError:
                print(f'{sys.argv[index + 1]} is not an integer', file = sys.stderr)
                exit(1)
            except IndexError:
                print('Number of Bombs not provided', file = sys.stderr)
                exit(1)

    table = createTable()
    player = Player(ROWS // 2, COLS // 2, ROWS, COLS)
    with KeyboardListener() as listener:
        while True:
            printTable(table, player)
            key = listener.get_key()
            # print(f"Key pressed: {key}")
            if key == 'q':
                break
            elif key == 'w':
                player.move(-1, 0)
            elif key == 's':
                player.move(1, 0)
            elif key == 'a':
                player.move(0, -1)
            elif key == 'd':
                player.move(0, 1)
            elif key == ' ':
                if (first_player):
                    initializeTable(table, player)
                    first_player = False
                openTableAtCursor(table, player)
            elif key == 'f':
                flagTableAtCursor(table, player)


