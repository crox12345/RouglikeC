#include <stdbool.h>
#ifndef ROGUE_FUNC_H
#define ROGUE_FUNC_H



#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

typedef struct Field{
    int rows;
    int colums;
    char* map;
}Field;

typedef struct Player{
    int y;
    int x;
    int hp;
    int attack;
    int gold;
    char* inventory[10];
}Player;

typedef struct Monster{
    int y;
    int x;
    int lvl;
    int type;
    bool awake;
}Monster;

typedef struct Game{
    Field* field;
    Player* player;
    int dlvl;
    Monster* monsters;
}Game;



void sidebar(Game* g);
void dungeon_draw(Game* g);
void dungeon_gen(Game* g);
void monster_gen(Game* g);
void monster_turn(Game* g);
void battle(Game* g,int dir_y,int dir_x);
Player* createPlayer(Game* g);
void freePlayer(Player* p);
int p_move(int input,Game* g);
Game* createGame(int rows, int colums);
void freeGame(Game* g);
Field* createField(int rows, int colums);
void freeField(Field* f);
int gameloop(int input,Game* g);
#endif
