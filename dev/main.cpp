#include "include.h"

#define KEY_RETURN '\n'
#define KEY_ESC 27

using namespace std;
typedef unsigned int uint;

uint TIME = 0;
uint CLK_TCK = CLOCKS_PER_SEC / 5; //~500000 - one second
uint RECOVERY_DELAY_COEFF = 2;
uint SCREEN_RIGHT_BIAS = 20;

//int counter = 0;
//void tester() {
//    cout << (to_string(++counter) + '\n').c_str();
//} // for debug

//void clean_screen(uint n_seconds) {
//    if (TIME % n_seconds == 0) { // clean every n_seconds seconds
//        clear();
//    }
//}

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
const uint NUMBER_OF_UNITS = 5;
uint PLAYER_FRACTION = 0;
uint OPPONENT_FRACTION = 0;

class CountryManager {
    string countries[NUMBER_OF_FRACTIONS];
    Country *player_country, *opponent_country;
    vector<uint> player_recovery_times, opponent_recovery_times;
public:
    CountryManager() {
        countries[0] = "Russia";
        countries[1] = "China";
        countries[2] = "India";

        player_recovery_times.resize(NUMBER_OF_UNITS, 0);
        opponent_recovery_times.resize(NUMBER_OF_UNITS, 0);
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
    Country* get_player_country() const {
        return player_country;
    }
    Country* get_opponent_country() const {
        return opponent_country;
    }
    vector<uint>& get_player_recovery_times() {
        return player_recovery_times;
    }
    vector<uint>& get_opponent_recovery_times() {
        return opponent_recovery_times;
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
    uint i1;
    vector<bool> to_delete(scr_width - 2, false);
    for (uint i = scr_width - 3; i > 0; --i) {
        i1 = i - 1;
        if (line_manager.get_owner(line, i1) == 'p') {
            Unit* left = line_manager[line][i1].first;
            Unit* right = line_manager[line][i1 + 1].first;
            if (line_manager.get_owner(line, i1 + 1) == 'o') {
                right->take_damage(left->attack());
                if (right->get_hp() <= 0) {
                    to_delete[i1 + 1] = true;
                }// killed
            }// battling
            if (left->get_hp() <= 0) {
                to_delete[i1] = true;
            }// killed
            if (i1 != scr_width - 4 && line_manager.get_owner(line, i1 + 1) == '-') {
                line_manager[line][i1].second = '-';
                line_manager[line][i1 + 1].first = line_manager[line][i1].first;
                line_manager[line][i1].first = nullptr;
                line_manager[line][i1 + 1].second = 'p';

            }// move
            if (i1 == scr_width - 4) {
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
    }// killing actually
    if (!line_manager.is_left_buffer_empty(line) && line_manager.get_owner(line, 0) == '-') {
        line_manager[line][0].first = line_manager.get_top_left_buffer(line);
        line_manager[line][0].second = 'p';
    }//moving left buffer
    if (!line_manager.is_right_buffer_empty(line) && line_manager.get_owner(line, scr_width - 3) == '-') {
        line_manager[line][scr_width - 3].first = line_manager.get_top_right_buffer(line);
        line_manager[line][scr_width - 3].second = 'o';
    }//moving right buffer

}


void draw_lines(LineManager& line_manager, uint scr_width, uint player_choice, uint opponent_choice, bool is_move) {
//    line_manager[line];// here the units from line
//    addstr(string(scr_width - 2, '-').c_str());// prints empty line
    current_player_country_damage = 0;
    current_opponent_country_damage = 0;

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
    }// printing lines
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

bool checking_for_move() {
    if ((clock() / CLK_TCK) - TIME > 1) {
        TIME = clock() / CLK_TCK;
        return true;
    }
    return false;
}

void draw_statistic(const CountryManager& country_manager) {
    mvprintw(NUMBER_OF_LINES + 1, 1, (string("Your hp: ") + to_string(country_manager.get_player_country()->hp)).c_str());
    string opp = string("Opponents hp: ") + to_string(country_manager.get_opponent_country()->hp);
    mvprintw(NUMBER_OF_LINES + 1, getmaxx(stdscr) - SCREEN_RIGHT_BIAS, opp.c_str());
}

void start_recovery(CountryManager& country_manager) {
    for (uint i = 0; i < NUMBER_OF_UNITS; ++i) {
        country_manager.get_player_recovery_times()[i] = RECOVERY_DELAY_COEFF * (*country_manager.get_player_country())[i]->get_recovery_time();
    }
    for (uint i = 0; i < NUMBER_OF_UNITS; ++i) {
        country_manager.get_opponent_recovery_times()[i] = RECOVERY_DELAY_COEFF * (*country_manager.get_opponent_country())[i]->get_recovery_time();
    }
}

void change_recovery(CountryManager& country_manager) {
    for (uint i = 0; i < NUMBER_OF_UNITS; ++i) {
        if (country_manager.get_player_recovery_times()[i] != 0) {
            --country_manager.get_player_recovery_times()[i];
        }// decreasing recovery time
    }
    for (uint i = 0; i < NUMBER_OF_UNITS; ++i) {
        if (country_manager.get_opponent_recovery_times()[i] != 0) {
            --country_manager.get_opponent_recovery_times()[i];
        }// decreasing recovery time
    }
}

void draw_recovery(CountryManager& country_manager) {
    mvprintw(NUMBER_OF_LINES + 2, 1, "Player soldiers: ");
    for (uint i = 0; i < NUMBER_OF_UNITS; ++i) {
        Unit* player_unit = (*country_manager.get_player_country())[i];
        mvprintw(NUMBER_OF_LINES + 3 + i, 1, (player_unit->get_name() + "(" + player_unit->get_symbol() + ")" + ": " + std::to_string(country_manager.get_player_recovery_times()[i])).c_str());
    }// printing
    mvprintw(NUMBER_OF_LINES + 2, getmaxx(stdscr) - SCREEN_RIGHT_BIAS, "Opponent soldiers: ");
    for (uint i = 0; i < NUMBER_OF_UNITS; ++i) {
        Unit* opponent_unit = (*country_manager.get_opponent_country())[i];
        mvprintw(NUMBER_OF_LINES + 3 + i, getmaxx(stdscr) - SCREEN_RIGHT_BIAS, (opponent_unit->get_name() + "(" + opponent_unit->get_symbol() + ")" + ": " + std::to_string(country_manager.get_opponent_recovery_times()[i])).c_str());
    }// printing
}

void check_for_boss(const string& whois, Unit* temp, const CountryManager& country_manager, LineManager& line_manager, uint player_choice, uint opponent_choice) {
    if (whois == "player") {
        if (temp->get_name() == "Dictator") {
            clear();
            mvprintw(NUMBER_OF_LINES, 1, "Leader XI out!");
            for (uint i = 0; i < 7; ++i)
                add_unit_on_line(country_manager.get_player_country()->get_new_unit1(), player_choice, line_manager, 1);
        }
        if (temp->get_name() == "Artillery") {
            clear();
            mvprintw(NUMBER_OF_LINES, 1, "Topol-M out!");
        }
        if (temp->get_name() == "God") {
            clear();
            mvprintw(NUMBER_OF_LINES, 1, "Buddha out!");
        }
    }
    else if (whois == "opponent") {
        if (temp->get_name() == "Dictator") {
            clear();
            mvprintw(NUMBER_OF_LINES, getmaxx(stdscr) - SCREEN_RIGHT_BIAS, "Leader XI out!");
            for (uint i = 0; i < 7; ++i)
                add_unit_on_line(country_manager.get_opponent_country()->get_new_unit1(), opponent_choice, line_manager, 0);
        }
        if (temp->get_name() == "Artillery") {
            clear();
            mvprintw(NUMBER_OF_LINES, getmaxx(stdscr) - SCREEN_RIGHT_BIAS, "Topol-M out!");
        }
        if (temp->get_name() == "God") {
            clear();
            mvprintw(NUMBER_OF_LINES, getmaxx(stdscr) - SCREEN_RIGHT_BIAS, "Buddha out!");
        }
    }
}

bool if_endgame(const CountryManager& country_manager) {
    if (country_manager.get_player_country()->take_damage(current_player_country_damage) <= 0) {
        clear();
        mvprintw(10, (getmaxx(stdscr) - SCREEN_RIGHT_BIAS) / 2, "OPPONENT WINS");
        refresh();
        sleep(4);
        return true;
    }
    if (country_manager.get_opponent_country()->take_damage(current_opponent_country_damage) <= 0) {
        clear();
        mvprintw(10, (getmaxx(stdscr) - SCREEN_RIGHT_BIAS) / 2, "PLAYER WINS");
        refresh();
        sleep(4);
        return true;
    }
    return false;
}


void game(LineManager& line_manager, CountryManager& country_manager) {
    noecho();// не отображается то, что печатаешь
    nodelay(stdscr, true);// getch() не ждет нажатия
//    clear();
    uint player_choice = 0;
    uint opponent_choice = 0;
    bool pause = false;

    // timing
    TIME = clock() / CLK_TCK;
    start_recovery(country_manager);

    while (true) {
        refresh();
        move(0, 0); // перемещает курсор в origin

        bool is_move = checking_for_move();

        draw_lines(line_manager, getmaxx(stdscr), player_choice, opponent_choice, is_move);
        draw_statistic(country_manager);
        if (is_move)
            change_recovery(country_manager);
        draw_recovery(country_manager);
        refresh();
        move(NUMBER_OF_LINES, 0);
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
                if (country_manager.get_player_recovery_times()[0] == 0) {
                    temp = country_manager.get_player_country()->get_new_unit1();
                    add_unit_on_line(temp, player_choice, line_manager, 1);
                    country_manager.get_player_recovery_times()[0] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                }
                break;
            case '2':
                if (country_manager.get_player_recovery_times()[1] == 0) {
                    temp = country_manager.get_player_country()->get_new_unit2();
                    add_unit_on_line(temp, player_choice, line_manager, 1);
                    country_manager.get_player_recovery_times()[1] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                }
                break;
            case '3':
                if (country_manager.get_player_recovery_times()[2] == 0) {
                    temp = country_manager.get_player_country()->get_new_unit3();
                    add_unit_on_line(temp, player_choice, line_manager, 1);
                    country_manager.get_player_recovery_times()[2] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                }
                break;
            case '4':
                if (country_manager.get_player_recovery_times()[3] == 0) {
                    temp = country_manager.get_player_country()->get_new_unit4();
                    add_unit_on_line(temp, player_choice, line_manager, 1);
                    country_manager.get_player_recovery_times()[3] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                }
                break;
            case '5':
                if (country_manager.get_player_recovery_times()[4] == 0) {
                    temp = country_manager.get_player_country()->get_new_unit5();
                    add_unit_on_line(temp, player_choice, line_manager, 1);
                    country_manager.get_player_recovery_times()[4] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                    check_for_boss("player", temp, country_manager, line_manager, player_choice, opponent_choice);
                }
                break;
            case '6':
                if (country_manager.get_opponent_recovery_times()[0] == 0) {
                    temp = country_manager.get_opponent_country()->get_new_unit1();
                    add_unit_on_line(temp, opponent_choice,line_manager, 0);
                    country_manager.get_opponent_recovery_times()[0] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                }
                break;
            case '7':
                if (country_manager.get_opponent_recovery_times()[1] == 0) {
                    temp = country_manager.get_opponent_country()->get_new_unit2();
                    add_unit_on_line(temp, opponent_choice, line_manager, 0);
                    country_manager.get_opponent_recovery_times()[1] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                }
                break;
            case '8':
                if (country_manager.get_opponent_recovery_times()[2] == 0) {
                    temp = country_manager.get_opponent_country()->get_new_unit3();
                    add_unit_on_line(temp, opponent_choice, line_manager, 0);
                    country_manager.get_opponent_recovery_times()[2] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                }
                break;
            case '9':
                if (country_manager.get_opponent_recovery_times()[3] == 0) {
                    temp = country_manager.get_opponent_country()->get_new_unit4();
                    add_unit_on_line(temp, opponent_choice, line_manager, 0);
                    country_manager.get_opponent_recovery_times()[3] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                }
                break;
            case '0':
                if (country_manager.get_opponent_recovery_times()[4] == 0) {
                    temp = country_manager.get_opponent_country()->get_new_unit5();
                    add_unit_on_line(temp, opponent_choice, line_manager, 0);
                    country_manager.get_opponent_recovery_times()[4] = RECOVERY_DELAY_COEFF * temp->get_recovery_time();
                    check_for_boss("opponent", temp, country_manager, line_manager, player_choice, opponent_choice);
                }
                break;

            case KEY_ESC:
                pause = true;
                break;
        }
//        clean_screen(100);
        pause = if_endgame(country_manager);
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
