CC = g++
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -Wno-error=unused-parameter
CFLAGS += -Wno-error=unused-variable
CFLAGS += -Wno-error=unused-but-set-variable
CFLAGS += -I"$$PWD/include/"
LINKS = -lncurses


SRCDIR = src
OBJDIR = obj
INCLUDEDIR = include

# C Source Files
SOURCES  = $(shell find $(SRCDIR) \
		-name "*.cpp" -o \
		-name "*.s" -o \
		-name "*.asm" | tr '\n' ' ')
# Remove SRCDIR
SOURCES := $(SOURCES:$(SRCDIR)/%=%)

# C Object Files
OBJECTS := $(SOURCES:%.cpp=%.o)
OBJECTS := $(OBJECTS:%.s=%.o)
OBJECTS := $(OBJECTS:%.asm=%.o)

# Header files
HEADERS  = $(wildcard $(INCLUDEDIR)/*.hpp)

TARGET = minesweeper
TARDIR = build

.PHONY: all
all: $(TARGET)

debug-flags: CFLAGS += -g
debug-flags: clean $(TARGET)

debug: CFLAGS += -DDEBUG=1

.PHONY: .clangd
.clangd:
	rm --force $@

	@echo CompileFlags: | tee --append $@
	@echo '  Add:' | tee --append $@

	@for flag in $(CFLAGS) ; do echo "    - $$flag" | tee --append $@ ; done

$(SRCDIR) $(OBJDIR) $(TARDIR) $(TESTDIR):
	mkdir --parents --verbose $@

.PHONY: clean
clean:
	@printf "Cleaning\n"
	rm -f $(TARDIR)/$(TARGET) $(TARGET) $(TEST)
	rm -rf $(OBJDIR)

$(TARGET): $(OBJECTS:%=$(OBJDIR)/%) | $(TARDIR)
	$(CC) $(CFLAGS) -o $(TARDIR)/$@ $^ $(LINKS)
	cp $(TARDIR)/$@ .

# Object rule for C++ files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJDIR)
	@printf "\n\033[31m"
	@printf "########################\n"
	@printf "Building %s\n" $@
	@printf "########################\n"
	@printf "\033[0m\n"
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c -o $@ $<

# Object rule for Assembly files
$(OBJDIR)/%.o: $(SRCDIR)/%.s $(HEADERS) | $(OBJDIR)
	@printf "\n\033[31m"
	@printf "########################\n"
	@printf "Building %s\n" $@
	@printf "########################\n"
	@printf "\033[0m\n"
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.asm $(HEADERS) | $(OBJDIR)
	@printf "\n\033[31m"
	@printf "########################\n"
	@printf "Building %s\n" $@
	@printf "########################\n"
	@printf "\033[0m\n"
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c -o $@ $<

