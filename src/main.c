#include <curses.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "rogue_func.h"


int main(){
    int input = 0;
    int rows ,colums;
    initscr();
    start_color();

    init_pair(BLACK, COLOR_BLACK, COLOR_WHITE);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
    
    use_default_colors();
    keypad(stdscr, 1); /* allow symbols like F1...F12 and arrows */
    noecho();
    curs_set(0);
    getmaxyx(stdscr,rows, colums); 
    Game* game = createGame(rows,colums);
    do{
        ;
    }while ((input = gameloop(input,game)) != 27);
    freeGame(game); 
    endwin();
    return 0;
}

