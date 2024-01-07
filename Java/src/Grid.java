package Minesweeper.Java.src;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

import javax.swing.text.Position;

/**
 * Grid
 */
public class Grid {

	private final int rows;
	private final int cols;
	private Cell[][] matrix;
	private Player player;
	private int numberOfBombs;
	private int generateBombs;
	private boolean bombsGenerated;
	private int openedCells;
	private int flaggedCells;
	private static Position2D[] neighborsPos = {
			new Position2D(-1, 0), // Above
			new Position2D(1, 0), // Below
			new Position2D(0, -1), // Left
			new Position2D(0, 1), // Right
						// Corners
			new Position2D(-1, -1), // Above
			new Position2D(-1, 1), // Below
			new Position2D(1, -1), // Left
			new Position2D(1, 1), // Right
	};

	public Grid(final int rows, final int cols, final int generateBombs) {
		this.rows = rows;
		this.cols = cols;
		setMatrix(new Cell[rows][cols]);
		player = new Player(0, 0);
		bombsGenerated = false;
		setNumberOfBombs(0);
		setOpenedCells(0);
		setFlaggedCells(0);
		setGenerateBombs(generateBombs);

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				matrix[i][j] = new Cell();

	}

	public Cell get(final int i, final int j) {
		return getMatrix()[i][j];
	}

	public Cell get(final Position2D position) {
		return get(position.getRow(), position.getCol());
	}

	public Grid set(final int i, final int j, final Cell cell) {
		this.matrix[i][j] = cell;

		return this;
	}

	public Grid set(final int i, final int j, final Cell.State state) {
		this.matrix[i][j].setState(state);

		return this;
	}

	public Cell[][] getMatrix() {
		return matrix;
	}

	public Grid setMatrix(Cell[][] matrix) {
		this.matrix = matrix;

		return this;
	}

	public int getRows() {
		return rows;
	}

	public int getCols() {
		return cols;
	}

	public boolean validPosition(final int i, final int j) {
		return 0 <= i && i < getRows() && 0 <= j && j < getCols();
	}

	public boolean validPosition(final Position2D position) {
		return validPosition(position.getRow(), position.getCol());
	}

	public void movePlayer(final int deltaI, final int deltaJ) {
		final int newI = player.getRow() + deltaI;
		final int newJ = player.getCol() + deltaJ;

		if (!validPosition(newI, newJ))
			return;

		player.add(new Position2D(deltaI, deltaJ));
	}

	public Player getPlayer() {
		return player;
	}

	public Grid setPlayer(Player player) {
		this.player = player;

		return this;
	}

	public int getNumberOfBombs() {
		return numberOfBombs;
	}

	public Grid setNumberOfBombs(int numberOfBombs) {
		this.numberOfBombs = numberOfBombs;

		return this;
	}

	public boolean checkWin() {
		return getOpenedCells() + getFlaggedCells() == getRows() * getCols();
	}

	private boolean update() {
		int numberOfBombs = 0;
		int openedCells = 0;
		int flaggedCells = 0;

		for (int i = 0; i < getRows(); i++) {
			for (int j = 0; j < getCols(); j++) {
				Cell cell = get(i, j);
				Cell.State state = cell.getState();

				if (state.equals(Cell.State.OpenEmpty))
					openedCells++;
				if (state.equals(Cell.State.Flag))
					flaggedCells++;
				if (cell.isBomb())
					numberOfBombs++;
			}
		}

		setOpenedCells(openedCells);
		setFlaggedCells(flaggedCells);
		setNumberOfBombs(numberOfBombs);
		return checkWin();
	}

	public boolean open() {
		if (getGenerateBombs() > 0)
			generateBombs();

		Cell cell = get(player);

		if (cell.getNeighbors() == 0)
			openRegion(player.getRow(), player.getCol());
		else
			cell.open();

		if (update())
			return true;

		if (cell.isBomb()) {
			lost();
			return true;
		}

		return false;
	}

	public void openRegion(final Position2D pos) {
		openRegion(pos.getRow(), pos.getCol());
	}

	public void openRegion(final int i, final int j) {
		Cell cell = get(i, j);

		Cell.State state = cell.getState();
		if (state.equals(Cell.State.OpenEmpty) || state.equals(Cell.State.Flag) || cell.isBomb())
			return;

		cell.open();

		if (cell.getNeighbors() > 0)
			return;

		for (final Position2D deltaPosition : neighborsPos) {
			final Position2D newPosition = deltaPosition.add(i, j);
			if (!validPosition(newPosition))
				continue;

			openRegion(newPosition);
		}

	}

	public boolean flag() {
		get(player).flag();
		update();

		return checkWin();
	}

	private void lost() {
		for (int i = 0; i < getRows(); i++) {
			for (int j = 0; j < getCols(); j++) {
				Cell cell = get(i, j);

				if (cell.isBomb())
					cell.setState(Cell.State.OpenEmpty);
			}
		}
	}

	private void generateBombs() {
		ArrayList<Cell> cells = new ArrayList<>();

		for (int i = 0; i < getRows(); i++) {
			for (int j = 0; j < getCols(); j++) {
				if (player.equals(i, j))
					continue;

				if (player.distanceSquare(i, j) <= 2)
					continue;

				Cell cell = get(i, j);
				cells.add(cell);
			}
		}

		Collections.shuffle(cells);

		for (int i = 0; i < getGenerateBombs(); i++) {
			cells.get(i).setBomb(true);
			numberOfBombs++;
		}

		setGenerateBombs(0);

		for (int i = 0; i < getRows(); i++) {
			for (int j = 0; j < getCols(); j++) {
				Cell cell = get(i, j);

				if (!cell.isBomb())
					continue;

				for (int deltaI = -1; deltaI <= 1; deltaI++) {
					for (int deltaJ = -1; deltaJ <= 1; deltaJ++) {
						Position2D newPosition = new Position2D(i + deltaI, j + deltaJ);

						if (!validPosition(newPosition))
							continue;
						if (player.equals(newPosition))
							continue;

						Cell neighbor = get(newPosition);
						neighbor.setNeighbors(neighbor.getNeighbors() + 1);
					}
				}
			}
		}
	}

	private int countCellsByState(final Cell.State state) {
		int sum = 0;

		for (Cell[] cells : matrix)
			for (Cell cell : cells) {
				if (!cell.getState().equals(state))
					continue;

				sum++;
			}

		return sum;
	}

	public int countClosedCells() {
		return countCellsByState(Cell.State.Close);
	}

	@Override
	public String toString() {
		String string = "";

		if (matrix == null)
			return string;

		string += String.format(
				"Size: %dx%d" + " | "
				+ Colors.ANSI_CYAN + "NumberOfBombs: %d" + Colors.ANSI_RESET + " | "
				+ Colors.ANSI_MAGENTA + "Opened cells: %d" + Colors.ANSI_RESET + " | "
				+ Colors.ANSI_BLUE + "Flagged cells: %d\n" + Colors.ANSI_RESET,
				getRows(), getCols(),
				getNumberOfBombs() + getGenerateBombs(),
				getOpenedCells(),
				getFlaggedCells());

		for (int i = 0; i < getRows(); i++) {
			for (int j = 0; j < getCols(); j++) {
				final Cell cell = matrix[i][j];

				string += String.format(
						player.equals(i, j)
								? "[%s]"
								: " %s ",
						cell);
			}

			if (i >= getRows() - 1)
				continue;
			string += "\n";
		}

		return string;
	}

	public boolean isBombsGenerated() {
		return bombsGenerated;
	}

	public Grid setBombsGenerated(boolean bombsGenerated) {
		this.bombsGenerated = bombsGenerated;

		return this;
	}

	public int getOpenedCells() {
		return openedCells;
	}

	public Grid setOpenedCells(int openedCells) {
		this.openedCells = openedCells;

		return this;
	}

	public int getFlaggedCells() {
		return flaggedCells;
	}

	public Grid setFlaggedCells(int flaggedCells) {
		this.flaggedCells = flaggedCells;

		return this;
	}

	public static Position2D[] getNeighborsPos() {
		return neighborsPos;
	}

	public static void setNeighborsPos(Position2D[] neighborsPos) {
		Grid.neighborsPos = neighborsPos;
	}

	public int getGenerateBombs() {
		return generateBombs;
	}

	public Grid setGenerateBombs(int generateBombs) {
		this.generateBombs = generateBombs;

		return this;
	}
}
