SRC_DIR = src
OBJ_DIR = obj
MAIN_CLASS = Minesweeper.Java.src.Minesweeper

java:
	javac -d $(OBJ_DIR) $(SRC_DIR)/*.java

run: java
	tty_settings=$$(stty --save)
	stty cbreak
	stty -icanon
	stty -echo
	java -cp $(OBJ_DIR) $(MAIN_CLASS)
	@stty $$tty_settings >/dev/null

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
