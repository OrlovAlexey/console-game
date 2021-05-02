#include "include.h"

#define KEY_RETURN '\n'
#define KEY_ESC 27

using namespace std;
typedef unsigned int uint;

uint TIME = 0;
uint CLK_TCK = CLOCKS_PER_SEC / 10; //~500000 - one second

int counter = 0;
void tester() {
    cout << (to_string(++counter) + '\n').c_str();
}

void timing() {
    //timing
    uint i = clock() / CLK_TCK;
    while (i < 1000000) {
        if ((clock() / CLK_TCK) % 1 == 0) {
            if (i != (clock() / CLK_TCK)) {
                std::cout << i << std::endl;
                i = clock() / CLK_TCK;
            }
        }
    }
}

void clean_screen(uint n_seconds) {
    if (TIME % n_seconds == 0) { // clean every n_seconds seconds
        clear();
    }
}

void start() {
    initscr();
    printw("Running game...\n");
    refresh();
    sleep(1);
    printw("Use English keyboard please.\n");
    refresh();// отправляет текст из буфера в консоль
    sleep(2);
}


const uint NUMBER_OF_FRACTIONS = 3;
uint PLAYER_FRACTION = 0;
uint OPPONENT_FRACTION = 0;

class CountryManager {
    string countries[NUMBER_OF_FRACTIONS];
    Country *player_country, *opponent_country;
public:
    CountryManager() {
        countries[0] = "Russia";
        countries[1] = "China";
        countries[2] = "India";
    }
    string operator[] (uint index) const {
        return countries[index];
    }
    void initialize_countries(uint player_index, uint opponent_index) {
        switch (player_index) {
            case 0:
                player_country = new Russia();
                break;
            case 1:
                player_country = new China();
                break;
            case 2:
                player_country = new India();
                break;
        }
        switch (opponent_index) {
            case 0:
                opponent_country = new Russia();
                break;
            case 1:
                opponent_country = new China();
                break;
            case 2:
                opponent_country = new India();
                break;
        }
    }
    Country* get_country(uint fraction) const {
        if (fraction == PLAYER_FRACTION) {
            return player_country;
        }
        else {
            return opponent_country;
        }
    }
};

uint fraction_choice(const CountryManager& country_manager, bool player){
    uint choice = 0;
    bool done = false;
    while (true) {
        clear();
        refresh();
        if (player) {
            printw("Which fraction would you like to play?\n");
        }
        else {
            printw("Which fraction would your opponent like to play?\n");
        }
        for (uint i = 0; i < NUMBER_OF_FRACTIONS; i++) {
            if (i == choice) {
                addch('-');
                addch('>');
            }
            else {
                addch(' ');
                addch(' ');
            }
            printw("%s\n", country_manager[i].c_str());
        }
        switch (getch()) {
            case KEY_UP:
                if (choice) {
                    choice--;
                }
                break;
            case KEY_DOWN:
                if (choice != NUMBER_OF_FRACTIONS - 1) {
                    choice++;
                }
                break;
            case KEY_RETURN:
                done = true;
                break;
        }
        if (done) {
            break;
        }
    }
    return choice;
}

void initializing_countries(CountryManager& country_manager) {
    curs_set(0); // ставим курсор невидимым
    keypad(stdscr, true);// enter и стрелочки работают
    PLAYER_FRACTION = fraction_choice(country_manager, true);
    printw("You have chosen %s\n", country_manager[PLAYER_FRACTION].c_str());
    refresh();
    sleep(1);
    OPPONENT_FRACTION = fraction_choice(country_manager, false);
    printw("Opponent has chosen %s\n", country_manager[OPPONENT_FRACTION].c_str());
    refresh();
    sleep(1);

    country_manager.initialize_countries(PLAYER_FRACTION, OPPONENT_FRACTION);
}

const uint NUMBER_OF_LINES = 5;
//uint CONSOLE_HEIGHT = getmaxy(stdscr);
//uint CONSOLE_WIDTH = getmaxx(stdscr);

class LineManager {
    vector<pair<Unit*, char>> lines[NUMBER_OF_LINES];
    vector<queue<Unit*>> left_buffer;
    vector<queue<Unit*>> right_buffer;
    /*
    {   ***CLARIFICATION***
        NOBODY = '-',
        PLAYER = 'p',
        OPPONENT = 'o'
     }
     */
public:
    LineManager(uint lines_width) {
        for (auto& line : lines) {
            line.resize(lines_width);
            for (auto& element : line) {
                element.first = nullptr;
                element.second = '-';
            }
        }
        for (uint i = 0; i < NUMBER_OF_LINES; ++i) {
            left_buffer.emplace_back();
            right_buffer.emplace_back();
        }
    }
    vector<pair<Unit*, char>>& operator[] (uint line) {
        return lines[line];
    }
    char get_owner(uint line, uint index) const {
        return lines[line][index].second;
    }
    bool is_left_buffer_empty(uint line) const {
        return left_buffer[line].empty();
    }
    bool is_right_buffer_empty(uint line) const {
        return right_buffer[line].empty();
    }
    void add_in_left_buffer(Unit* unit, uint line) {
        left_buffer[line].push(unit);
    }
    void add_in_right_buffer(Unit* unit, uint line) {
        right_buffer[line].push(unit);
    }
    Unit* get_top_left_buffer(uint line) {
        Unit* temp = left_buffer[line].front();
        left_buffer[line].pop();
        return temp;
    }
    Unit* get_top_right_buffer(uint line) {
        Unit* temp = right_buffer[line].front();
        right_buffer[line].pop();
        return temp;
    }
};

uint current_player_country_damage;
uint current_opponent_country_damage;

void move_line(uint line, LineManager& line_manager, uint scr_width) {
    uint j;
    vector<bool> to_delete(scr_width - 2, false);
    for (uint i = scr_width - 3; i > 0; --i) {
        j = i - 1;
        if (line_manager.get_owner(line, j) == 'p') {
            Unit* left = line_manager[line][j].first;
            Unit* right = line_manager[line][j + 1].first;
            if (line_manager.get_owner(line, j + 1) == 'o') {
                right->take_damage(left->attack());
                if (right->get_hp() <= 0) {
                    to_delete[j + 1] = true;
                }// killed
            }// battling
            if (left->get_hp() <= 0) {
                to_delete[j] = true;
            }// killed
            if (j != scr_width - 4 && line_manager.get_owner(line, j + 1) == '-') {
                line_manager[line][j].second = '-';
                line_manager[line][j + 1].first = line_manager[line][j].first;
                line_manager[line][j].first = nullptr;
                line_manager[line][j + 1].second = 'p';

            }// move
            if (j == scr_width - 4) {
                current_opponent_country_damage += left->attack();
            }// damaging opponents country
        }
    }
    for (uint i = 1; i < scr_width - 2; ++i) {
        if (line_manager.get_owner(line, i) == 'o') {
            Unit* left = line_manager[line][i - 1].first;
            Unit* right = line_manager[line][i].first;
            if (line_manager.get_owner(line, i - 1) == 'p') {
                left->take_damage(right->attack());
                if (left->get_hp() <= 0) {
                    to_delete[i - 1] = true;
                }// killed
            }// battling
            if (right->get_hp() <= 0) {
                to_delete[i] = true;
            }// killed
            if (i != 1 && line_manager.get_owner(line, i - 1) == '-') {
                line_manager[line][i].second = '-';
                line_manager[line][i - 1].first = line_manager[line][i].first;
                line_manager[line][i].first = nullptr;
                line_manager[line][i - 1].second = 'o';
            }//move
            if (i == 1) {
                current_player_country_damage += right->attack();
            }// damaging players country
        }
    }
    for (uint i = 0; i < scr_width - 2; ++i) {
        if (to_delete[i]) {
            Unit* temp = line_manager[line][i].first;
            delete temp;
            line_manager[line][i].first = nullptr;
            line_manager[line][i].second = '-';
        }
    }
    if (!line_manager.is_left_buffer_empty(line) && line_manager.get_owner(line, 0) == '-') {
        line_manager[line][0].first = line_manager.get_top_left_buffer(line);
        line_manager[line][0].second = 'p';
    }//moving left buffer
    if (!line_manager.is_right_buffer_empty(line) && line_manager.get_owner(line, scr_width - 3) == '-') {
        line_manager[line][scr_width - 3].first = line_manager.get_top_right_buffer(line);
        line_manager[line][scr_width - 3].second = 'o';
    }//moving right buffer

}


void draw_lines(LineManager& line_manager, uint scr_width, uint player_choice, uint opponent_choice) {
//    line_manager[line];// here the units from line
//    addstr(string(scr_width - 2, '-').c_str());// prints empty line
    current_player_country_damage = 0;
    current_opponent_country_damage = 0;
    bool is_move = false;
    if ((clock() / CLK_TCK) - TIME > 1) {
        is_move = true;
        TIME = clock() / CLK_TCK;
    }
    vector<string> lines;
    for (uint line = 0; line < NUMBER_OF_LINES; ++line) {
        string new_line(scr_width - 2, '-');
        if (is_move) {
            move_line(line, line_manager, scr_width);
        }// moving all  objects every second (approximately)
        for (uint i = 0; i < scr_width - 2; ++i) {
            if (line_manager[line][i].second != '-') {
                new_line[i] = (line_manager[line][i].first->get_symbol() == '\0' ? '?'
                                                                                 : line_manager[line][i].first->get_symbol());
            }
        }// putting objects on line

        lines.push_back(new_line);
    }
    for (uint i = 0; i < NUMBER_OF_LINES; ++i) {
        if (i == player_choice) {
            addch('>');
        } else {
            addch(' ');
        }
        addstr(lines[i].c_str());
        if (i == opponent_choice) {
            addch('<');
        } else {
            addch(' ');
        }
    }
}


void add_unit_on_line(Unit* unit, uint line, LineManager& line_manager, bool player) {
    if (player) {
        if (line_manager.get_owner(line, 0) != '-') {
            line_manager.add_in_left_buffer(unit, line);
        }
        else {
            line_manager[line][0] = {unit, 'p'};
        }
    }
    else {
        uint last = getmaxx(stdscr) - 3;
        if (line_manager.get_owner(line, last) != '-') {
            line_manager.add_in_right_buffer(unit, line);
        }
        else {
            line_manager[line][last] = {unit, 'o'};
        }
    }
}

void draw_statistic(const CountryManager& country_manager, uint src_width) {
    mvprintw(6, 0, (string("Your hp: ") + to_string(country_manager.get_country(PLAYER_FRACTION)->hp)).c_str());
    mvprintw(7, 0, (string("Opponents hp: ") + to_string(country_manager.get_country(OPPONENT_FRACTION)->hp)).c_str());
}

void game(LineManager& line_manager, const CountryManager& country_manager) {
    noecho();// не отображается то, что печатаешь
    nodelay(stdscr, true);// getch() не ждет нажатия
    clear();
    uint player_choice = 0;
    uint opponent_choice = 0;
    bool pause = false;

    // timing
    TIME = clock() / CLK_TCK;

    while (true) {
        refresh();
        move(0, 0); // перемещает курсор в origin

        draw_lines(line_manager, getmaxx(stdscr), player_choice, opponent_choice);
        if (country_manager.get_country(PLAYER_FRACTION)->take_damage(current_player_country_damage) <= 0) {
            clear();
            mvprintw(10, getmaxx(stdscr) / 2, "OPPONENT WINS");
            refresh();
            sleep(4);
            return;
        }
        if (country_manager.get_country(OPPONENT_FRACTION)->take_damage(current_opponent_country_damage) <= 0) {
            clear();
            mvprintw(10, getmaxx(stdscr) / 2, "PLAYER WINS");
            refresh();
            sleep(4);
            return;
        };
        draw_statistic(country_manager, getmaxx(stdscr));
        refresh();
        move(5, 0);
        Unit* temp;
        switch (getch()) {
            case 'w':
                if (player_choice) {
                    player_choice--;
                }
                break;
            case 's':
                if (player_choice != NUMBER_OF_LINES - 1) {
                    player_choice++;
                }
                break;

            case KEY_UP:
                if (opponent_choice) {
                    opponent_choice--;
                }
                break;
            case KEY_DOWN:
                if (opponent_choice != NUMBER_OF_LINES - 1) {
                    opponent_choice++;
                }
                break;
            case '1':
                add_unit_on_line(country_manager.get_country(PLAYER_FRACTION)->get_new_unit1(), player_choice, line_manager, 1);
                break;
            case '2':
                add_unit_on_line(country_manager.get_country(PLAYER_FRACTION)->get_new_unit2(), player_choice, line_manager, 1);
                break;
            case '3':
                add_unit_on_line(country_manager.get_country(PLAYER_FRACTION)->get_new_unit3(), player_choice, line_manager, 1);
                break;
            case '4':
                add_unit_on_line(country_manager.get_country(PLAYER_FRACTION)->get_new_unit4(), player_choice, line_manager, 1);
                break;
            case '5':
                temp = country_manager.get_country(PLAYER_FRACTION)->get_new_unit5();
                add_unit_on_line(temp, player_choice, line_manager, 1);
                if (temp->get_symbol() == 'X') {
                    for (uint i = 0; i < 7; ++i)
                        add_unit_on_line(country_manager.get_country(PLAYER_FRACTION)->get_new_unit1(), player_choice, line_manager, 1);
                }
                break;
            case '6':
                add_unit_on_line(country_manager.get_country(OPPONENT_FRACTION)->get_new_unit1(), opponent_choice, line_manager, 0);
                break;
            case '7':
                add_unit_on_line(country_manager.get_country(OPPONENT_FRACTION)->get_new_unit2(), opponent_choice, line_manager, 0);
                break;
            case '8':
                add_unit_on_line(country_manager.get_country(OPPONENT_FRACTION)->get_new_unit3(), opponent_choice, line_manager, 0);
                break;
            case '9':
                add_unit_on_line(country_manager.get_country(OPPONENT_FRACTION)->get_new_unit4(), opponent_choice, line_manager, 0);
                break;
            case '0':
                temp = country_manager.get_country(PLAYER_FRACTION)->get_new_unit5();
                add_unit_on_line(temp, player_choice, line_manager, 1);
                if (temp->get_symbol() == 'X') {
                    for (uint i = 0; i < 7; ++i)
                        add_unit_on_line(country_manager.get_country(PLAYER_FRACTION)->get_new_unit1(), player_choice, line_manager, 1);
                }
                break;

            case KEY_ESC:
                pause = true;
                break;
        }
        clean_screen(100);
        if (pause) {
            break;
        }
    }
}

void finish() {
    getch();
    endwin();
}

int main() {
    start();

    CountryManager country_manager;
    initializing_countries(country_manager);

    LineManager line_manager(getmaxx(stdscr) - 2);
    game(line_manager, country_manager);

    finish();
    return 0;
}
