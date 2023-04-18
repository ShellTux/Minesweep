#include "colors.h"
#include "cursor.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

typedef struct {
	int neighbors;
	bool flagged;
	bool bomb;
	bool open;
} Cell;

typedef struct {
	int row, col;
} Cursor;

int ROWS            = 10;
int COLS            = 10;
bool firstPlay      = 1;
int NUMBER_OF_BOMBS = 5;
// Save terminal attributes
struct termios terminalOriginalAttributes;

void restoreTerminal(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &terminalOriginalAttributes);
	SHOW_CURSOR;
}

bool validPosition(int i, int j)
{
	return i >= 0 && i < ROWS && j >= 0 && j < COLS;
}

void moveCursorUp(int lines)
{
	printf("\x1b[%d%c", lines, lines >= 0 ? 'A' : 'D');
}

void createTable(Cell table[ROWS][COLS], int bombsNumber, Cursor player)
{
	// TODO: Technically it possible that this while loop is infinite
	while (bombsNumber > 0) {
		int i = rand() % ROWS;
		int j = rand() % COLS;

		if (i == player.row && j == player.col) continue;

		// Skip if there is a bomb already there
		if (table[i][j].bomb) continue;

		table[i][j].bomb = 1;
		for (int deltaRow = -1; deltaRow <= 1; ++deltaRow)
			for (int deltaCol = -1; deltaCol <= 1; ++deltaCol) {
				if (deltaRow == 0 && deltaCol == 0) continue;
				int newI = i + deltaRow, newJ = j + deltaCol;
				if (!validPosition(newI, newJ)) continue;

				++table[newI][newJ].neighbors;
			}

		--bombsNumber;
	}
}

void initializeTable(Cell table[ROWS][COLS])
{
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j) {
			Cell cell;
			cell.bomb      = 0;
			cell.flagged   = 0;
			cell.neighbors = 0;
			cell.open      = 0;

			table[i][j] = cell;
		}
}

char getCharacterFromCell(Cell cell)
{
	char c = '.';

	if (cell.open) {
		if (cell.bomb) return '*';

		return cell.neighbors ? '0' + cell.neighbors : ' ';
	} else if (cell.flagged) return '?';


	return c;
}

int countOpen(Cell table[ROWS][COLS])
{
	// TODO: Improve instead of go through the whole table every time
	// Have a global variable for the opened cells
	int openSum = 0;
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j) openSum += table[i][j].open;

	return openSum;
}

int countFlags(Cell table[ROWS][COLS])
{
	// TODO: Improve instead of go through the whole table every time
	// Have a global variable for the flags cells
	int flagSum = 0;
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j) flagSum += table[i][j].flagged;

	return flagSum;
}

void printTable(Cell table[ROWS][COLS], Cursor player)
{
	printf("\033[2J");
	printf("\033[1;1H");

	int openSum = countOpen(table);
	int flagSum = countFlags(table);

	printf("%sNumber of Bombs: %d%s",
	       COLOR_CYAN,
	       NUMBER_OF_BOMBS,
	       COLOR_RESET);

	printf(" | ");

	printf("%sOpened cells: %d%s", COLOR_MAGENTA, openSum, COLOR_RESET);

	printf(" | ");

	printf("%sFlagged Cells: %d%s\n", COLOR_BLUE, flagSum, COLOR_RESET);

	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			Cell cell         = table[i][j];
			bool isPlayer     = player.row == i && player.col == j;
			char c            = getCharacterFromCell(cell);
			char *stringColor = COLOR_RESET;

			if (cell.bomb && cell.open) stringColor = COLOR_RED;
			else if (cell.flagged) stringColor = COLOR_YELLOW;

			if (isPlayer)
				printf("%s[%s%c%s]%s",
				       COLOR_GREEN,
				       stringColor,
				       c,
				       COLOR_GREEN,
				       COLOR_RESET);
			else printf(" %s%c%s ", stringColor, c, COLOR_RESET);
		}

		printf("\n");
	}

	// draw Borders
	for (int i = 0; i < ROWS; ++i)
		printf("\033[%d;%dH|", i + 2, 3 * COLS + 1);
	printf("\n");

	for (int i = 0; i <= 3 * COLS; ++i) printf("-");
	printf("\n");

	if (openSum + flagSum == ROWS * COLS && flagSum == NUMBER_OF_BOMBS) {
		printf("%sYou won%s\n", COLOR_GREEN, COLOR_RESET);
		restoreTerminal();
		exit(0);
	}
}

void movePlayer(Cursor *player, int deltaRow, int deltaCol)
{
	if (!validPosition(player->row + deltaRow, player->col + deltaCol))
		return;

	player->row += deltaRow;
	player->col += deltaCol;
}

void openEmptyRegionAtPos(Cell table[ROWS][COLS], int row, int col)
{
	Cell *cell = &table[row][col];

	if (cell->open || cell->bomb || cell->flagged) return;
	if (cell->neighbors > 0) {
		cell->open = 1;
		return;
	}

	cell->open = 1;


	int deltaPos[4][2] = {
	    {-1,  0}, // Above
	    { 1,  0}, // Below
	    { 0, -1}, // Left
	    { 0,  1}  // Right
	};

	for (unsigned long int i = 0;
	     i < sizeof(deltaPos) / sizeof(deltaPos[0]);
	     ++i) {
		int newI = row + deltaPos[i][0];
		int newJ = col + deltaPos[i][1];

		if (!validPosition(newI, newJ)) continue;

		openEmptyRegionAtPos(table, newI, newJ);
	}

	for (int deltaRow = -1; deltaRow <= 1; ++deltaRow)
		for (int deltaCol = -1; deltaCol <= 1; ++deltaCol) {
			// Skip corners
			if (deltaRow * deltaRow + deltaCol * deltaCol > 1)
				continue;

			int i = row + deltaRow, j = col + deltaCol;
			if (!validPosition(i, j)) continue;
			openEmptyRegionAtPos(table, i, j);
		}
}

void lost(Cell table[ROWS][COLS], Cursor player)
{
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j) {
			Cell *cell = &table[i][j];

			if (!cell->bomb) continue;

			cell->open = 1;
			printTable(table, player);
		}

	printf("%sYou Lost%s\n", COLOR_RED, COLOR_RESET);
	restoreTerminal();
	exit(0);
}

void openTableAtCursor(Cell table[ROWS][COLS], Cursor player)
{
	Cell *cell = &table[player.row][player.col];

	if (cell->bomb) lost(table, player);

	if (cell->neighbors == 0)
		openEmptyRegionAtPos(table, player.row, player.col);

	cell->open    = 1;
	cell->flagged = 0;
}

void flagTableAtCursor(Cell table[ROWS][COLS], Cursor player)
{
	table[player.row][player.col].flagged ^= 1;
}

int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], "--help") == 0) {
			char *helpPage
			    = "Use: minesweeper-c [OPTION] [VALUE]\n"
			      "  --help         Print this help page\n"
			      "  --bombs        Number of bombs (integer)\n"
			      "  --rows         Number of table rows "
			      "(integer)\n"
			      "  --cols         Number of table cols (integer)";

			printf("%s\n", helpPage);

			return 0;
		} else if (strcmp(argv[i], "--bombs") == 0) {
			if (!sscanf(argv[i + 1], "%d", &NUMBER_OF_BOMBS)) {
				fprintf(
				    stderr,
				    "The number of bombs is not an integer\n"
				    "Example:\n"
				    "./minesweeper-c --bombs 10\n");
				return 1;
			}
		} else if (strcmp(argv[i], "--rows") == 0) {
			if (!sscanf(argv[i + 1], "%d", &ROWS)) {
				fprintf(
				    stderr,
				    "The number of rows is not an integer\n"
				    "Example:\n"
				    "./minesweeper-c --rows 10\n");
				return 1;
			}
		} else if (strcmp(argv[i], "--cols") == 0) {
			if (!sscanf(argv[i + 1], "%d", &COLS)) {
				fprintf(
				    stderr,
				    "The number of columns is not an integer\n"
				    "Example:\n"
				    "./minesweeper-c --cols 10\n");
				return 1;
			}
		}
	}

	Cell table[ROWS][COLS];
	Cursor player;
	player.row = ROWS / 2;
	player.col = COLS / 2;

	srand(time(NULL));

	initializeTable(table);


	tcgetattr(STDIN_FILENO, &terminalOriginalAttributes);

	// Set terminal to non-canonical mode
	struct termios tattr = terminalOriginalAttributes;
	tattr.c_lflag        &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &tattr);

	// Loop to read arrow key presses
	char keyPress;
	HIDE_CURSOR;
	while (keyPress != 'e') {
		printTable(table, player);

		// Read a single character from the input stream
		read(STDIN_FILENO, &keyPress, 1);

		switch (keyPress) {
		case 04: // ^d
		case 27: // Escape
			keyPress = 'e';
			break;
		case 'w': // Up
			movePlayer(&player, -1, 0);
			break;
		case 's': // Down
			movePlayer(&player, 1, 0);
			break;
		case 'a': // Left
			movePlayer(&player, 0, -1);
			break;
		case 'd': // Right
			movePlayer(&player, 0, 1);
			break;
		case ' ': // Open
			if (firstPlay)
				createTable(table, NUMBER_OF_BOMBS, player);
			firstPlay = 0;
			openTableAtCursor(table, player);
			break;
		case 'f':
			flagTableAtCursor(table, player);
			break;
		}
	}

	restoreTerminal();

	return 0;
}
