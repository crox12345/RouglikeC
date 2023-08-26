#include <stdlib.h> 
#include <time.h>
#include "rogue_func.h"
#include <stdbool.h>
#include <ncurses.h>

#ifndef ROGUE_FUNC_C
#define ROGUE_FUNC_C

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
                    if (g->monsters[m].y == screenY && g->monsters[m].x == screenX){
                        attron(COLOR_PAIR(RED));
                           mvaddch(screenY,screenX,g->monsters[m].type);
                        attroff(COLOR_PAIR(RED));
                    }
                }
            }
        }
    }
   sidebar(g);
}
/* generating dungeon */
void dungeon_gen(Game* g){
    int rows = g->field->rows;
    int colums = g->field->colums;
    srand(time(NULL));
    /* creating rooms */
    if (!g->r_placed){
        bool r_colision;
        int ry, rx; 
        int r_size_y, r_size_x;
        int room_num = rand() % 5 + 5;
    /* generate base of dungeon and safe it in DB */
        for (int screenY = 0; screenY < rows; screenY++){
            for (int screenX = 0; screenX < colums; screenX++){
                if (screenY == 0 || screenY == 1 ||
                    screenY == rows - 2 ||  screenY == rows -1 || 
                    screenX == 0 || screenX == colums - 1 ){
                    g->field->map[screenY * colums + screenX] = '%';/* boarders */
                }else {

                    g->field->map[screenY * colums +screenX] = '#'; /* walls */
                }
            }
        } 

    /* creating rooms */
        do{
            int try_counter = 0;
            int r_old_center_y;
            int r_old_center_x;
            int r_center_y;
            int r_center_x;
            /* generating rooms until no colisions */
            do{
                r_colision = 0;
                /* room coords */
                ry = rand() % (rows - 4) + 1;
                rx = rand() % (colums - 4) + 1;
        
                /* room size */
                r_size_y = rand() % 5 + 4; 
                r_size_x = rand() % 10 + 8;
                
                /* checking for room colisions */
                for (int screenY = ry; screenY <= r_size_y + ry ; screenY++){
                    for (int screenX = rx; screenX <= r_size_x + rx; screenX++){
                        if (g->field->map[screenY * colums +screenX] == '%' || g->field->map[screenY * colums + screenX] == ' ' ||
                            g->field->map[(screenY + 1) * colums + screenX] == '%' || g->field->map[(screenY - 1) * colums +screenX] == '%' ||
                            g->field->map[ screenY * colums + (screenX + 1)]== '%' || g->field->map[screenY * colums + (screenX - 1)] == '%'  ||
                            g->field->map[(screenY + 2) * colums + screenX] == '%' || g->field->map[(screenY - 2) * colums +screenX] == '%' ||
                            g->field->map[screenY * colums + (screenX + 2)] == '%' || g->field->map[screenY * colums + (screenX - 2)] == '%'  )
                        {
                            r_colision = 1;
                            screenY = ry + r_size_y;
                            break;
                        }
                    }
                }

                try_counter++;
                if (try_counter > 100){
                    ry = rx = 3;
                    r_size_y = r_size_x = 3;
                    break;
                }
            }while(r_colision); 

            /* savimg roooms in DB*/
            for (int screenY = ry; screenY <= r_size_y + ry ; screenY++){
                for (int screenX = rx; screenX <= r_size_x + rx; screenX++){
                    if (g->field->map[screenY * colums + screenX] == '%'){
                        screenY = ry + r_size_y; 
                        break;
                    }
                    else{
                        g->field->map[screenY * colums + screenX] = ' ';
                    }
                }
            } 

            g->r_placed++;

        /* Creating corridors */
            /* rooms centers */
            if (g->r_placed > 1){
                r_old_center_y = r_center_y;
                r_old_center_x = r_center_x;
            }
            r_center_y = ry + r_size_y / 2;
            r_center_x = rx + r_size_x /2;
            

            if (g->r_placed > 1){
                int path_y = r_old_center_y;
                for (; path_y != r_center_y; ){
                    g->field->map[path_y * colums + r_old_center_x] = ' ';
                    if (path_y < r_center_y){
                        path_y++; 
                    }else if (path_y > r_center_y){
                        path_y--;
                    }
                }
                for (int x = r_old_center_x; x != r_center_x; ){
                    g->field->map[path_y * colums + x] = ' ';
                    if (x < r_center_x){
                        x++; 
                    }else if (x > r_center_x){
                        x--;
                    }
                }
            }
        }while(g->r_placed <= room_num);
    /* stairs generetion */
        int sy,sx;
        do{
            sy = rand() % (rows - 4) + 1;
            sx = rand() % (colums - 4) + 1;
        }while(g->field->map[sy * colums + sx] != ' ');
        g->field->map[sy * colums + sx] = '>';
    }
}


/* spawning monsters */
void monster_gen(Game* g){
    int rows = g->field->rows;
    int colums = g->field->colums;
    srand(time(NULL));
    int y,x; 
    if (!g->m_placed){
        for (int m = 0; m < 10;m++){
            do{
                y = rand() % (rows - 4) + 2;
                x = rand() % (colums - 4) + 2;
            }while (g->field->map[y * colums + x] != ' ');
            g->monsters[m].y = y;
            g->monsters[m].x = x;
            g->monsters[m].lvl = rand() % g->dlvl + 2 ;
            g->monsters[m].type = rand() % g->dlvl + 97;
            g->monsters[m].awake = 0;
            g->field->map[y * colums + x] = 'm';
        }
        g->m_placed = 1;
    }
}
void player_gen(Game* game, Player* player){
    int colums = game->field->colums;
    do{
        player->y = rand() % game->field->rows;
        player->x = rand() % game->field->colums;
    }while(game->field->map[player->y * colums + player->x] != ' ');
}

void monster_turn(Game* g){
    int colums = g->field->colums;
    int dist_y, dist_x;
    for (int m = 0; m < 10;m++){

        if (g->monsters[m].lvl < 1){
            continue;
        }
        dist_y = abs(g->monsters[m].y - g->player->y);
        dist_x = abs(g->monsters[m].x - g->player->x);
        if (dist_y < 5 && dist_x < 5){
            g->monsters[m].awake = 1;
        }
        if (g->monsters[m].awake  == 0){
            continue;
        }
    /* default movement */
        int dir_y = g->monsters[m].y;
        int dir_x = g->monsters[m].x;
        if(dist_y > dist_x){
            if( dir_y < g->player->y){
                dir_y++;
            }else{
                dir_y--;
            }

        }else{
            if( dir_x < g->player->x){
                dir_x++;
            }else{
                dir_x--;
            }
        }
    /* diagonal movement */
        if(g->field->map[dir_y * colums + dir_x] == '#' || g->field->map[dir_y * colums + dir_x] == '%'){
            dir_y = g->monsters[m].y;
            dir_x = g->monsters[m].x;
            if( dir_y < g->player->y){
                dir_y++;
            }else{
                dir_y--;
            }

            if( dir_x < g->player->x){
                dir_x++;
            }else{
                dir_x--;
            }
        }
        
    /* inverse movement */
        if(g->field->map[dir_y * colums + dir_x] == '#' || g->field->map[dir_y * colums + dir_x] == '%'){
            dir_y = g->monsters[m].y;
            dir_x = g->monsters[m].x;
            if( dir_y > g->player->y){
                dir_y++;
            }else{
                dir_y--;
            }

            if( dir_x > g->player->x){
                dir_x++;
            }else{
                dir_x--;
            }
        }

        if(dist_y < 2 && dist_x < 2){
           g->player->hp -= g->dlvl / 2 + 1;
        }else if (g->field->map[dir_y * colums + dir_x] == ' '){
            g->field->map[g->monsters[m].y * colums + g->monsters[m].x] = ' ';
            g->monsters[m].y = dir_y;
            g->monsters[m].x = dir_x;
            g->field->map[g->monsters[m].y * colums + g->monsters[m].x] = 'm';

        }
    }
}
void battle(Game* g,int dir_y,int dir_x){
    int colums = g->field->colums;
    int m = 0;
    for (; m < 10; m++){
        if ((g->monsters[m].y == dir_y) && (g->monsters[m].x == dir_x)){
            if (g->monsters[m].lvl <= 0){
                g->field->map[dir_y * colums + dir_x] = ' ';
                g->player->gold += rand() % 3 + 1;
            }else{
                g->monsters[m].lvl -= g->player->attack;
            }
            break;
        }
    }
}

/* move player */
int p_move(int input, Game* g){
    int colums = g->field->colums;
    srand(time(NULL));
    if(input != 0){
        int dir_y = g->player->y, dir_x = g->player->x;
        if (input == KEY_UP ){
            dir_y--;
        }
        else if (input == KEY_DOWN){
            dir_y++;
        }

        else if (input == KEY_LEFT){
            dir_x--;
        }

        else if (input == KEY_RIGHT){
            dir_x++;
        }
        if (g->field->map[dir_y * colums + dir_x] == '>' || input == '>'){
            return 1;
        }

        if(g->field->map[dir_y * colums + dir_x] == ' '){
            g->player->y = dir_y;
            g->player->x = dir_x;
        }else if ( g->field->map[dir_y * colums + dir_x] == 'm'){
            battle(g, dir_y, dir_x);
        }
    }
    return 0;
}
void freeMonster(Monster* m){
    if(m){
        free(m);
    }
}
Player* createPlayer(Game* game){
    Player* player = (Player*)malloc(sizeof(Player));
    int colums = game->field->colums;
    player_gen(game,player);
    player->hp = 10;
    player->gold = 0;
    player->attack = 1;
    return player;
}


void freePlayer(Player* p){
    if (p){
        free(p);
    }
}

Field* createField(int rows, int colums){
    Field* field = (Field*)malloc(sizeof(Field));
    field->rows = rows;
    field->colums = colums;
    field->map = (char*)malloc(sizeof(char) * (rows + 1)  * (colums + 1));
    return field;
}
void freeField(Field* f){
    if(f){
        if(f->map){
            free(f->map);
        }
        free(f);
    }
}

Game* createGame(int rows, int colums){
    Game* game = (Game*)malloc(sizeof(Game));
    game->m_placed = 0;
    game->p_placed = 0;
    game->r_placed = 0;
    game->field = createField(rows, colums);
    dungeon_gen(game);
    game->dlvl = 1;
    game->monsters = (Monster*)malloc(sizeof(Monster) * 10);
    monster_gen(game);
    game->player = createPlayer(game);
    return game;
}
void freeGame(Game* g){
    if (g){
        freePlayer(g->player);
        freeField(g->field);
        freeMonster(g->monsters);
        free(g);
    }
}

int gameloop(int input, Game* game){
    int new_lvl;
    int rows = game->field->rows;
    int colums = game->field->colums;
    new_lvl = p_move(input,game);
    monster_turn(game);
    dungeon_draw(game);

    attron(COLOR_PAIR(GREEN));
        mvaddch(game->player->y,game->player->x, '@');
    attroff(COLOR_PAIR(GREEN));
    if (new_lvl){
        clear();
        mvprintw(rows / 2, colums / 2, "LEVEL %d", ++game->dlvl);
        game->m_placed = 0;
        game->r_placed = 0;
        game->p_placed = 0;
        dungeon_gen(game);
        monster_gen(game);
        player_gen(game, game->player);
    }
    if(game->player->hp < 1){
        clear();
        mvprintw(rows/2, colums / 2 - 4 , "Game over");
        game->m_placed = 0;
        game->r_placed = 0;
        game->p_placed = 0;
        dungeon_gen(game);
        monster_gen(game);
        player_gen(game, game->player);
        game->dlvl = 1;
        game->player->attack = 1;
        game->player->hp = 10;
        game->player->gold = 0;
    }
    input = getch();
    return input;
}

#endif
