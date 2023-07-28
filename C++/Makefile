CC = g++
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -Wno-error=unused-parameter
CFLAGS += -Wno-error=unused-variable
CFLAGS += -Wno-error=unused-but-set-variable
LINKS = -lncurses

SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(SRCDIR)/*.hpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

TARGET = minesweepper
TARDIR = build

.PHONY: all clean
all: run

debug-flags: CFLAGS += -g -Og
debug-flags: clean $(TARGET)

debug: CFLAGS += -DDEBUG=1
debug: run

$(TARGET): $(OBJECTS) | $(TARDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LINKS)

$(SRCDIR) $(OBJDIR) $(TARDIR) $(TESTDIR):
	mkdir --parents --verbose $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	@printf "\n"
	@printf "\033[31m"
	@printf "########################\n"
	@printf "Building %s\n" $@
	@printf "########################\n"
	@printf "\033[0m"
	@printf "\n"
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@printf "Cleaning\n"
	rm -f $(OBJECTS) $(TARGET) $(TEST)

run: $(TARGET)
	@printf "\n"
	@printf "\033[32m"
	@printf "###############\n"
	@printf "#   Running   #\n"
	@printf "###############\n"
	@printf "\033[0m"
	@printf "\n"
	./$(TARGET)
