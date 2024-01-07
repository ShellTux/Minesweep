package Minesweeper.Java.src;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;

public class Minesweeper {
	private Grid grid;

	public Minesweeper() {
		this(10, 10, 10);
	}

	public Minesweeper(final int rows, final int cols, final int genereateBombs) {
		grid = new Grid(rows, cols, genereateBombs);
	}

	private void print() {
		System.out.println(grid);
	}

	public void run() {
		boolean quit = false;

		System.out.print("\033[H\033[2J");
		print();
		while (!quit) {
			quit = listenKeyboard();
			System.out.print("\033[H\033[2J");
			print();
		}

		System.out.println(
				grid.checkWin() ? "Won!"
						: "Lost!");
	}

	public boolean listenKeyboard() {
		try {

			final char input = (char) System.in.read();
			System.out.println(input);
			while (System.in.available() > 0) {
				System.in.read();
			}
			return switch (input) {
				case 'w' -> {
					grid.movePlayer(-1, 0);
					yield false;
				}
				case 'a' -> {
					grid.movePlayer(0, -1);
					yield false;
				}
				case 's' -> {
					grid.movePlayer(1, 0);
					yield false;
				}
				case 'd' -> {
					grid.movePlayer(0, 1);
					yield false;
				}
				case ' ' -> {
					yield grid.open();
				}
				case 'f' -> {
					yield grid.flag();
				}
				case 'q' -> true;
				default -> false;
			};
		} catch (IOException e) {
			e.printStackTrace();
		}

		return true;
	}

	public static void main(String[] args) {
		Minesweeper minesweeper = new Minesweeper();
		minesweeper.run();
	}
}
