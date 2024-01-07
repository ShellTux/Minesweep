package Minesweeper.Java.src;

/**
 * Position
 */
public class Position2D {

	private int row;
	private int col;

	public Position2D(final int row, final int col) {
		setRow(row);
		setCol(col);
	}

	public int getRow() {
		return row;
	}

	public Position2D setRow(int row) {
		this.row = row;

		return this;
	}

	public int getCol() {
		return col;
	}

	public Position2D setCol(int col) {
		this.col = col;

		return this;
	}

	public Boolean equals(final int row, final int col) {
		return equals(new Position2D(row, col));
	}

	public Boolean equals(final Position2D other) {
		return getRow() == other.getRow() && getCol() == other.getCol();
	}

	public Position2D add(final int row, final int col) {
		return new Position2D(getRow() + row, getCol() + col);
	}

	public Position2D add(final Position2D other) {
		setRow(getRow() + other.getRow());
		setCol(getCol() + other.getCol());

		return this;
	}

	public int distanceSquare(final int row, final int col) {
		int deltaRow = getRow() - row;
		int deltaCol = getCol() - col;

		return deltaRow * deltaRow + deltaCol * deltaCol;
	}

	public int distanceSquare(final Position2D other) {
		return distanceSquare(other.getRow(), other.getCol());
	}
}
