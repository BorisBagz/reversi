#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"
#include "moves.h"

#include <stdio.h>
#include <stdbool.h>

struct node_t
{
  int value;
  move_t move;
};

typedef struct board_t board_t;
typedef struct node_t node_t;

move_t random_player(board_t *board, size_t param);
move_t human_player(board_t *board, size_t param);
move_t minimax_player(board_t *board, size_t depth);
move_t minimax_ab_player(board_t *board, size_t depth);

#endif /*PLAYER_H*/
