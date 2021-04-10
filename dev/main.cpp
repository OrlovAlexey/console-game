#include "include.h"

#define KEY_RETURN '\n'
#define KEY_ESC 27

using namespace std;
typedef unsigned int uint;

uint TIME = 0;
uint CLK_TCK = 100000; //~100000 - one second

void timing() {
    //timing
    unsigned int i = clock() / CLK_TCK;
    while (i < 1000000) {
        if ((clock() / CLK_TCK) % 1 == 0) {
            if (i != (clock() / CLK_TCK)) {
                std::cout << i << std::endl;
                i = clock() / CLK_TCK;
            }
        }
    }
}

void start() {
    printw("Running game...\n");
    refresh();
    sleep(1);
    printw("Use English keyboard please.\n");
    refresh();//отправляет текст из буфера в консоль
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


const uint NUMBER_OF_LINES = 5;
//uint CONSOLE_HEIGHT = getmaxy(stdscr);
//uint CONSOLE_WIDTH = getmaxx(stdscr);

class LineManager {
    vector<pair<Unit, uint>> lines[NUMBER_OF_LINES];
    vector<char> underlines[NUMBER_OF_LINES];// underlines[line][i] shows who owns a unit in this line in position i
    /*
    {
        NOBODY = '-',
        PLAYER = 'p',
        OPPONENT = 'o'
     }
     */
public:
    LineManager() {
        for (auto& underline : underlines) {
            underline.resize(getmaxx(stdscr));
            for (char& element : underline) {
                element = '-';
            }
        }
    }
    vector<pair<Unit, uint>>& operator[] (uint line) {
        return lines[line];
    }
    vector<char>& at(uint line) {
        return underlines[line];
    }
    char get_owner(uint line, uint index) const {
        return underlines[line][index];
    }
};

void move_line(uint line, LineManager& line_manager, uint scr_width) {
    for (pair<Unit, uint>& unit_on_line1 : line_manager[line]) {
        if (line_manager.get_owner(line, unit_on_line1.second) == 'p') {

            if (unit_on_line1.second != scr_width - 3) {
                line_manager.at(line)[unit_on_line1.second] = '-';
                line_manager.at(line)[++unit_on_line1.second] = 'p';

                // BATTLING TODO
                /*
                for (pair<Unit, uint>& unit_on_line2 : line_manager[line]) {
                    if (unit_on_line2.second == unit_on_line1.second + 1) {

                    }
                }
                 */
            }
        }
        else {
            if (unit_on_line1.second != 1) {
                line_manager.at(line)[unit_on_line1.second--] = '-';
                line_manager.at(line)[unit_on_line1.second] = 'o';
            }
        }
    }
}


void draw_line(uint line, LineManager& line_manager, uint scr_width) {
//    line_manager[line];// here the units from line
//    addstr(string(scr_width - 2, '-').c_str());// prints empty line
    string new_line(scr_width - 2, '-');
    if ((clock() / CLK_TCK) % 1 == 0) {
        if (TIME != (clock() / CLK_TCK)) {
            move_line(line, line_manager, scr_width);
            TIME = clock() / CLK_TCK;
        }
    }// moving all  objects every second
    for (const pair<Unit, uint>& unit_on_line : line_manager[line]) {
        //left-buffer, right-buffer TODO
        if (new_line[unit_on_line.second] == '-') {
            new_line[unit_on_line.second] = (unit_on_line.first.get_symbol() == '\0' ? '?'
                                                                                     : unit_on_line.first.get_symbol());
        }
    }// putting objects on line
    addstr(new_line.c_str());
}


void add_unit_on_line(Unit& unit, uint line, LineManager& line_manager, bool player) {
    //left-buffer, right-buffer TODO
    if (player) {
        line_manager[line].push_back({unit, 0});
        line_manager.at(line)[0] = 'p';
    }
    else {
        uint last = getmaxx(stdscr) - 2;
        line_manager[line].push_back({unit, last});
        line_manager.at(line)[last] = 'o';

    }
}

void game(LineManager& line_manager, const CountryManager& country_manager) {
    clear();
    uint player_choice = 0;
    uint opponent_choice = 0;
    bool pause = false;

    // timing
    TIME = clock() / CLK_TCK;

    while (true) {
        refresh();
        move(0, 0); // перемещает курсор в origin
        for (uint i = 0; i < NUMBER_OF_LINES; i++) {// i = line
            if (i == player_choice) {
                addch('>');
            } else {
                addch(' ');
            }
            draw_line(i, line_manager, getmaxx(stdscr));
            if (i == opponent_choice) {
                addch('<');
            } else {
                addch(' ');
            }
        }
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
                add_unit_on_line(*country_manager.get_country(PLAYER_FRACTION)->get_new_unit1(), player_choice, line_manager, 1);
                break;
            case '2':
                add_unit_on_line(*country_manager.get_country(PLAYER_FRACTION)->get_new_unit2(), player_choice, line_manager, 1);
                break;
            case '3':
                add_unit_on_line(*country_manager.get_country(PLAYER_FRACTION)->get_new_unit3(), player_choice, line_manager, 1);
                break;
            case '4':
                add_unit_on_line(*country_manager.get_country(PLAYER_FRACTION)->get_new_unit4(), player_choice, line_manager, 1);
                break;
            case '5':
                add_unit_on_line(*country_manager.get_country(PLAYER_FRACTION)->get_new_unit5(), player_choice, line_manager, 1);
                break;
            case '6':
                add_unit_on_line(*country_manager.get_country(OPPONENT_FRACTION)->get_new_unit1(), opponent_choice, line_manager, 0);
                break;
            case '7':
                add_unit_on_line(*country_manager.get_country(OPPONENT_FRACTION)->get_new_unit2(), opponent_choice, line_manager, 0);
                break;
            case '8':
                add_unit_on_line(*country_manager.get_country(OPPONENT_FRACTION)->get_new_unit3(), opponent_choice, line_manager, 0);
                break;
            case '9':
                add_unit_on_line(*country_manager.get_country(OPPONENT_FRACTION)->get_new_unit4(), opponent_choice, line_manager, 0);
                break;
            case '0':
                add_unit_on_line(*country_manager.get_country(OPPONENT_FRACTION)->get_new_unit5(), opponent_choice, line_manager, 0);
                break;

            case KEY_ESC:
                pause = true;
                break;
        }
        if ((clock() / CLK_TCK) % 1 == 0) {
            if (TIME != (clock() / CLK_TCK)) {
                TIME = clock() / CLK_TCK;
            }
        }
        if (TIME % 100 == 0) {
            clear();
        }
        if (pause) {
            break;
        }
    }
}


int main() {
    initscr();
    start();

    curs_set(0); // ставим курсор невидимым
    keypad(stdscr, true);// enter и стрелочки работают
    CountryManager country_manager;
    PLAYER_FRACTION = fraction_choice(country_manager, true);
    printw("You have chosen %s\n", country_manager[PLAYER_FRACTION].c_str());
    refresh();
    sleep(1);
    OPPONENT_FRACTION = fraction_choice(country_manager, false);
    printw("Opponent has chosen %s\n", country_manager[OPPONENT_FRACTION].c_str());
    refresh();
    sleep(1);

    country_manager.initialize_countries(PLAYER_FRACTION, OPPONENT_FRACTION);

    noecho();// не отображается то, что печатаешь
    nodelay(stdscr, true);// getch() не ждет нажатия
    LineManager line_manager;
    game(line_manager, country_manager);

    getch();
    endwin();
    return 0;
}
