#include "game.hpp"

#include <curses.h>
#include <ncurses.h>

#include <cstdint>
#include <iostream>
#include <vector>

char cell_char(const Cell cell) {
    if (cell.state == State::Open) {
        if (cell.is_bomb) return Game::CharState::Bomb;

        if (cell.neighbors > 0) return '0' + cell.neighbors;

        return Game::CharState::OpenEmpty;
    } else if (cell.state == State::Flag)
        return Game::CharState::Flag;

    return Game::CharState::Close;
};

Game::Game(const size_t rows, const size_t cols, const size_t n_bombs) {
    this->rows = rows;
    this->cols = cols;

    const Cell default_cell = {
        .state = State::Close,
        .is_bomb = false,
        .neighbors = 0,
    };

    field = vector(rows, vector(cols, default_cell));

    number_bombs = n_bombs;

    initscr();
    cbreak();
    noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    curs_set(0);
    if (has_colors())
        start_color();
    else
        cout << "Your terminal does not support color" << endl;

    init_pair(Cyan, COLOR_CYAN, 0);
    init_pair(Magenta, COLOR_MAGENTA, 0);
    init_pair(Blue, COLOR_BLUE, 0);
    init_pair(Red, COLOR_RED, 0);
    init_pair(Green, COLOR_GREEN, 0);
};

Game::~Game(void) {
    if (win) {
        attron(COLOR_PAIR(Green));
        printw("You won!\n");
        attroff(COLOR_PAIR(Green));
    } else {
        attron(COLOR_PAIR(Red));
        printw("You lost!\n");
        attroff(COLOR_PAIR(Red));
    }

    attron(COLOR_PAIR(Cyan));
    printw("Press Enter to exit.\n");
    attroff(COLOR_PAIR(Cyan));

    getch();
    echo();
    curs_set(1);
    endwin();
};

void Game::update(void) {
    clear();
    mvwin(stdscr, 0, 0);

    printw("Size: %zux%zu", rows, cols);
    printw(" | ");

    attron(COLOR_PAIR(Cyan));
    printw("Number of bombs: %zu", number_bombs);
    attroff(COLOR_PAIR(Cyan));

    printw(" | ");

    attron(COLOR_PAIR(Magenta));
    printw("Opened cells: %zu", opened_cells);
    attroff(COLOR_PAIR(Magenta));

    printw(" | ");

    attron(COLOR_PAIR(Blue));
    printw("Flagged cells: %zu", flagged_cells);
    attroff(COLOR_PAIR(Blue));

    printw("\n");

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            const Vector current = Vector(i, j);
            char state = cell_char(field[i][j]);
            bool is_player = current == player;

            if (is_player)
                printw("[");
            else
                printw(" ");

            int color = win ? Green : Red;
            if (state == CharState::Bomb) attron(COLOR_PAIR(color));
            printw("%c", state);
            if (state == CharState::Bomb) attroff(COLOR_PAIR(color));

            if (is_player)
                printw("]");
            else
                printw(" ");
        }
        printw("\n");
    }
}

void Game::get_key_press(void) {
    int key_press = getch();
    switch (key_press) {
        case 'w':
        case KEY_UP:
            player += Vector(-1, 0);
            break;
        case 's':
        case KEY_DOWN:
            player += Vector(1, 0);
            break;
        case 'a':
        case KEY_LEFT:
            player += Vector(0, -1);
            break;
        case 'd':
        case KEY_RIGHT:
            player += Vector(0, 1);
            break;
        case 'q':
            is_running = false;
            break;
        case 'f':
            flag(player);
            break;
        case '\n':
        case ' ':
            if (first) {
                generate_field();
                first = false;
            }
            open(player);
            break;
    }

    player.constrain(0, rows - 1, 0, cols - 1);

    if (opened_cells + flagged_cells == rows * cols) {
        win = true;
        is_running = false;
        update();
    };
};

void Game::open(Vector position) {
    const auto i = position.get_i();
    const auto j = position.get_j();
    Cell *cell = &field[i][j];

    if (cell->state == State::Open) return;

    if (cell->state == State::Flag)
        --flagged_cells;
    else if (cell->is_bomb) {
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j) {
                Cell *cell = &field[i][j];

                if (!cell->is_bomb) continue;

                cell->state = State::Open;
            }

        is_running = false;
        update();
        return;
    }
    ++opened_cells;
    cell->state = State::Open;

    if (cell->neighbors > 0) return;

    Vector delta_pos[] = {
        Vector(-1, 0),
        Vector(0, -1),
        Vector(0, 1),
        Vector(1, 0),
        // Corners
        Vector(-1, -1),
        Vector(-1, 1),
        Vector(1, -1),
        Vector(1, 1),
    };

    for (Vector delta_position : delta_pos) {
        if (delta_position == Vector(0, 0)) continue;

        const Vector new_position = position + delta_position;

        if (!valid_position(new_position)) continue;

        open(new_position);
    }
}

bool Game::valid_position(Vector pos) {
    return valid_position(pos.get_i(), pos.get_j());
}

bool Game::valid_position(const int64_t i, const int64_t j) {
    return 0 <= i && i < (int64_t)rows && 0 <= j && j < (int64_t)cols;
}

void Game::flag(Vector position) {
    const auto i = position.get_i();
    const auto j = position.get_j();
    Cell *cell = &field[i][j];

    if (cell->state == State::Open) return;

    if (cell->state == State::Close) {
        cell->state = State::Flag;
        ++flagged_cells;
    } else if (cell->state == State::Flag) {
        cell->state = State::Close;
        --flagged_cells;
    }
}

void Game::generate_field(void) {
    Vector neighbors[] = {
        Vector(-1, 0),
        Vector(0, -1),
        Vector(0, 1),
        Vector(1, 0),
        // Corners
        Vector(-1, -1),
        Vector(-1, 1),
        Vector(1, -1),
        Vector(1, 1),
    };

    size_t bombs_planted = 0;
    // TODO: Possible infinite loop, I am lazy. UwU
    while (bombs_planted < number_bombs) {
        const auto i = rand() % rows;
        const auto j = rand() % cols;
        Vector current(i, j);

        if (field[i][j].is_bomb) continue;

        field[i][j].is_bomb = true;
        ++bombs_planted;

        for (Vector neighboring : neighbors) {
            const Vector new_position = current + neighboring;
            const auto new_i = new_position.get_i();
            const auto new_j = new_position.get_j();
            if (!valid_position(new_position)) continue;

            ++field[new_i][new_j].neighbors;
        }
    }
}
