package Minesweeper.Java.src;

/**
 * Colors
 */
public class Colors {

	public static final String ANSI_RESET = "\u001B[0m";
	public static final String ANSI_BLACK = "\u001B[30m";
	public static final String ANSI_RED = "\u001B[31m";
	public static final String ANSI_GREEN = "\u001B[32m";
	public static final String ANSI_YELLOW = "\u001B[33m";
	public static final String ANSI_BLUE = "\u001B[34m";
	public static final String ANSI_MAGENTA = "\u001B[35m";
	public static final String ANSI_CYAN = "\u001B[36m";
	public static final String ANSI_WHITE = "\u001B[37m";

	public static void main(String[] args) {

		System.out.println(ANSI_RED + "This text is in red color!" + ANSI_RESET);
		System.out.println(ANSI_GREEN + "This text is in green color!" + ANSI_RESET);
		System.out.println(ANSI_BLUE + "This text is in blue color!" + ANSI_RESET);
	}
}
