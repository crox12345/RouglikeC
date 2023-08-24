#include <curses.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "rogue_func.h"


bool m_placed = 0;
bool p_placed = 0;
int r_placed = 0;

Monster monsters[10]; 

char roof_buffer[100] ="Gold: %d \t Dungeon level: %d \t HP:  %d \t Attack: %d";
char bottom_buffer[100] = "hihhiahahaha";


void sidebar(Game* g){
    int rows = g->field->rows;
    int colums = g->field->colums;
    int bottom_sidebar_Y = rows - 1;
    int roof_sidebar_Y = 0;
    for(int sidebar_X = 0; sidebar_X <= colums - 1; sidebar_X++){
        /*map[sidebar_Y][sidebar_X] = '=';*/
        mvaddch(bottom_sidebar_Y, sidebar_X, ' ');
    }
    for(int sidebar_X = 0; sidebar_X <= colums - 1; sidebar_X++){
        /*map[sidebar_Y][sidebar_X] = '=';*/
        mvaddch(roof_sidebar_Y, sidebar_X, ' ');
    }
    mvprintw(roof_sidebar_Y, 0,roof_buffer, g->player->gold, g->dlvl, g->player->hp ,  g->player->attack);
    mvprintw(bottom_sidebar_Y, 0, bottom_buffer);
}

/* drawing dungeon */
void dungeon_draw(Game* g){
    int rows = g->field->rows;
    int colums = g->field->colums;
    char* map = g->field->map;
    for (int screenY = 0; screenY < rows; screenY++){
        for (int screenX = 0; screenX < colums; screenX++){
            if (map[screenY * colums +screenX] == '>'){
                attron(A_BOLD);
                    mvaddch(screenY,screenX, '>');
                attroff(A_BOLD);
            } 
            else if (map[screenY * colums + screenX] == '%'){
                mvaddch(screenY,screenX, '%');
            }
            else if (map[screenY * colums + screenX] == ' '){
                mvaddch(screenY,screenX, ' ');
            }
            else if (map[screenY *colums + screenX] == '#'){
                attron(COLOR_PAIR(MAGENTA));
                    mvaddch(screenY,screenX, '#');
                attroff(COLOR_PAIR(MAGENTA));
            }
            else if (map[screenY * colums + screenX] == 'm'){
                for (int m = 0; m < 10;m++){
                    if (monsters[m].y == screenY && monsters[m].x == screenX){
                        attron(COLOR_PAIR(RED));
                           mvaddch(screenY,screenX,monsters[m].type);
                        attroff(COLOR_PAIR(RED));
                    }
                }
            }
        }
    }
   sidebar(g);
}


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

