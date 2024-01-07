package Minesweeper.Java.src;

/**
 * Player
 */
public class Player extends Position2D {

	public Player(int row, int col) {
		super(row, col);
	}

	public Player update(final Position2D position) {
		setRow(position.getRow());
		setCol(position.getCol());

		return this;
	}

	
}
