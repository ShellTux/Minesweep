package Minesweeper.Java.src;

/**
 * Cell
 */
public class Cell {

	public enum State {
		OpenEmpty(' '),
		Close('.'),
		Flag('?'),
		Bomb('*');

		private char value;

		private State(final char value) {
			this.value = value;
		}

		public char getValue() {
			return value;
		}
	}

	private State state;
	private int neighbors;
	private boolean isBomb;

	public boolean isBomb() {
		return isBomb;
	}

	public void setBomb(boolean isBomb) {
		this.isBomb = isBomb;
	}

	public State getState() {
		return state;
	}

	public Cell setState(State state) {
		this.state = state;

		return this;
	}

	public int getNeighbors() {
		return neighbors;
	}

	public Cell setNeighbors(int neighbors) {
		this.neighbors = neighbors;

		return this;
	}

	public void open() {
		setState(State.OpenEmpty);
	}

	public boolean flag() {
		if (getState().equals(State.OpenEmpty)) return false;

		setState(getState().equals(State.Flag) ? State.Close : State.Flag);
		return true;
	}

	public Cell() {
		state = State.Close;
		neighbors = 0;
	}

	@Override
	public String toString() {
		if (getState().equals(State.OpenEmpty)) {
			if (isBomb())
				return "" + State.Bomb.getValue();

			if (getNeighbors() == 0)
				return "" + State.OpenEmpty.getValue();

			return "" + switch (getNeighbors()) {
				case 1 -> '1';
				case 2 -> '2';
				case 3 -> '3';
				case 4 -> '4';
				case 5 -> '5';
				case 6 -> '6';
				case 7 -> '7';
				case 8 -> '8';
				case 9 -> '9';
				default -> throw new IllegalArgumentException("Unexpected value: " + getNeighbors());
			};
		} else if (getState().equals(State.Flag))
			return "" + State.Flag.getValue();

		return "" + State.Close.getValue();
	}
}
