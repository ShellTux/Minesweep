#include <map>
#include <vector>

#include "vector.hpp"

using namespace std;

enum class State { Close = 0, Open, Flag };
typedef struct {
    State state;
    bool is_bomb;
    size_t neighbors;
} Cell;

char cell_char(const Cell cell);

class Game {
   public:
    Game(const size_t rows, const size_t cols, const size_t n_bombs);
    ~Game(void);
    void update(void);
    void get_key_press(void);
    bool is_running = true;
    enum CharState { OpenEmpty = ' ', Close = '.', Flag = '?', Bomb = '*' };

   private:
    void open(Vector position);
    void flag(Vector position);
    bool valid_position(Vector pos);
    bool valid_position(const int64_t i, const int64_t j);
    void generate_field(void);
    bool first{true};
    bool win{false};
    size_t rows, cols;
    size_t number_bombs;
    size_t opened_cells{};
    size_t flagged_cells{};
    enum NcursesColor { Reset = 0, Cyan = 1, Magenta, Blue, Red, Green };
    vector<vector<Cell>> field;
    Vector player;
};
