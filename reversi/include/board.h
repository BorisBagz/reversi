#ifndef BOARD_H
#define BOARD_H

#include "moves.h"
#include "player.h"

#include <stdio.h>
#include <stdbool.h>

/* Min/Max width board */
#define MIN_BOARD_SIZE 2
#define MAX_BOARD_SIZE 8

/*Board Stones*/
typedef enum
{
BLACK_STONE = 'X',
WHITE_STONE = 'O',
EMPTY_STONE = '_',
HINT_STONE = '*',
} stone_t;

/*Store the score of a game*/
typedef struct
{
size_t black;
size_t white;
} score_t;


/*Reversi board (forward declaration to hide the implementation)*/
typedef struct board_t board_t;
typedef struct moves_t moves_t;


board_t *board_alloc (size_t size, stone_t player);
board_t *board_init (size_t size);
void board_free (board_t *board);
board_t *board_copy (board_t *board);
int board_set (board_t *board, stone_t stone, size_t row, size_t column);
stone_t board_get (board_t *board, size_t row, size_t column);
score_t board_score (board_t *board);
size_t board_get_size (board_t *board);
stone_t board_get_player (board_t *board);
char *board_str (board_t *board);
void board_print (board_t *board, FILE *fd);
bool board_is_move_valid (board_t *board, move_t move);
bool board_play (board_t *board, move_t move);

void board_save(board_t *board, FILE *fd);
board_t *board_load(char *filename);
moves_t *board_get_moves(board_t *board);
int board_count_moves(board_t *board);
board_t *board_set_player(board_t *board, stone_t player);


void valid_hints(board_t *board);
board_t *board_play_aux(board_t *board, move_t move);


#endif /*BOARD_H*/
