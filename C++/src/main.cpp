#include <getopt.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>

#include "game.hpp"

using namespace std;

int main(int argc, char **argv) {
    size_t rows{10};
    size_t cols{10};
    size_t bombs{10};

    const struct option longOptions[] = {
        {"rows", required_argument, nullptr, 'r'},
        {"cols", required_argument, nullptr, 'c'},
        {"bombs", required_argument, nullptr, 'b'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}  // Required for the last element
    };
    int option_index{};
    int c;
    while ((c = getopt_long(argc, argv, "r:c:b:h", longOptions,
                            &option_index)) != -1) {
        switch (c) {
            case 'r':
                rows = atoi(optarg);
                break;
            case 'c':
                cols = atoi(optarg);
                break;
            case 'b':
                bombs = atoi(optarg);
                break;
            case 'h':
                cout << "Use: " << argv[0] << endl;
                cout << endl;
                cout << " -h, --help      Print this help page" << endl;
                cout << " -r, --rows      Number of table rows (integer)"
                     << endl;
                cout << " -c, --cols      Number of table columns (integer)"
                     << endl;
                cout << " -b, --bombs     Number of bombs (integer)" << endl;
                return 0;
            case '?':
                if (optopt == 'r' || optopt == 'c' || optopt == 'b') {
                    cerr << "Option -" << static_cast<char>(optopt)
                         << " requires an argument." << endl;
                } else {
                    cerr << "Unknown option -" << static_cast<char>(optopt)
                         << "." << endl;
                }
                return 1;
            default:
                return 1;
        }
    }

    Game table = Game(rows, cols, bombs);

    while (table.is_running) {
        table.update();
        table.get_key_press();
    }

    return 0;
}
