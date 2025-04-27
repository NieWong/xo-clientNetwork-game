#ifndef TICTAC_H
#define TICTAC_H

#include "csapp.h"

#define B_size 20
#define W_score 5
#define P1 'X'
#define P2 'O'
#define BL '.'

#define G_on 0
#define P1_win 1
#define P2_win 2
#define Tie 3

#define M_move 1
#define M_board 2
#define M_over 3

typedef struct {
    int type;
    union {
        struct {
            int row;
            int col;
        } move;
        
        struct {
            char grid[B_size][B_size];
            int turn;
        } board_update;
        
        int result;
    } data;
} game_msg_t;

void init_board(char grid[B_size][B_size]);
void dis_board(char grid[B_size][B_size]);
int check_win(char grid[B_size][B_size], int row, int col);
int check_tie(char grid[B_size][B_size]);
int valid_move(char grid[B_size][B_size], int row, int col);
void clear_screen();

#endif