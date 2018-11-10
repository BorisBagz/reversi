#include "board.h"
#include "moves.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#define INFINITY 10000;

/*we created a little auxiliar structure that
will help us with the move when we will implement
the tree search algorythmes*/
struct moves_t
{
  size_t size;
  move_t *list;
};

/*this is the function called by the main, it will call
one of the tree search algorythmes that we have made
and run it in the given board with a depth given
by the param parametre. It will return the
best move returned by the algorythm*/
move_t random_player(board_t *board, size_t param)
{
  move_t moverandom;
  moves_t *moves = board_get_moves(board);
  size_t size = moves->size;
  size_t randpos = rand() % size;
  moverandom = moves->list[randpos];
  moves_free(moves);
  return moverandom;
}

/*this function will calculate the heuristique value of
the nodes of the three. it will add points if the position
is good or substract if the position is vulnerable.
we return the value.*/
static size_t score_heuristic(board_t *board, moves_t *moves)
{
  size_t value = 0;
  size_t movescount = moves_count(moves);
  move_t move;
  size_t size = board_get_size(board);
  value = moves_count(moves);

  for(size_t i = 0; i < movescount; i++)
  {
    move = moves_get(moves, i);

    //searching for corners
    //left high corner
    if(move.row == 0 && move.column == 0)
    {
      value++;
    }

    //right down corner
    if(move.row == size - 1 && move.column == size - 1)
    {
      value++;
    }

    //right high corner
    if(move.row == 0 && move.column == size - 1)
    {
      value++;
    }

    //left down corner
    if(move.row == size - 1 && move.column == 0)
    {
      value++;
    }

    //searching for edges
    //left high corner
    if(move.row == 0 && move.column == 1)
    {
      value--;
    }

    if(move.row == 1 && move.column == 0)
    {
      value--;
    }

    if(move.row == 1 && move.column == 1)
    {
      value--;
    }

    //right high corner
    if(move.row == 0 && move.column == size - 2)
    {
      value--;
    }

    if(move.row == 1 && move.column == size - 1)
    {
      value--;
    }

    if(move.row == 1 && move.column == size - 1)
    {
      value--;
    }

    //right low corner
    if(move.row == size - 2 && move.column == 0)
    {
      value--;
    }

    if(move.row == size - 1 && move.column == 1)
    {
      value--;
    }

    if(move.row == size - 2 && move.column == 1)
    {
      value--;
    }

    //left low corner
    if(move.row == size - 1 && move.column == size - 2)
    {
      value--;
    }

    if(move.row == size - 2 && move.column == size - 2)
    {
      value--;
    }

    if(move.row == size - 2 && move.column == size - 1)
    {
      value--;
    }

  }
  return value;
}

/*this function implements the minimax alpha-beta pruning algo.
for searching the best moves for the player and the
worst for the adversaire. also it will cut the ramification
that will no longer be needed*/
static node_t minimax_ab(board_t *board, size_t depth, stone_t player,
node_t alpha, node_t beta)
{
  move_t move;
  node_t node;
  size_t countmoves = 0;

  if(depth == 0)
  {
    moves_t *movesheur = NULL;
    movesheur = board_get_moves(board);
    node.value = score_heuristic(board, movesheur);
    node.move = (move_t) {0,0};
    moves_free(movesheur);
    return node;
  }

  countmoves = board_count_moves(board);

  if(player == board_get_player(board))
  {
    for(size_t i = 0 ; i < countmoves; i++)
    {
      board_t *board_aux = NULL;
      moves_t *moves = NULL;
      board_aux = board_copy(board);
      moves = board_get_moves(board_aux);
      move = moves_get(moves, i);
      board_play(board_aux, move);
      node = minimax_ab(board_aux, depth - 1, player, alpha, beta);

      if(node.value > alpha.value)
      {
        alpha.value = node.value;
        alpha.move = move;
      }
      if(alpha.value >= beta.value)
      {
        break;
      }
      board_free(board_aux);
      moves_free(moves);
    }
    return alpha;
  }

  else
  {
    for(size_t i = 0 ; i < countmoves; i++)
    {
      board_t *board_aux = NULL;
      moves_t *moves = NULL;
      board_aux = board_copy(board);
      moves = board_get_moves(board_aux);
      move = moves_get(moves, i);
      board_play(board_aux, move);
      node = minimax_ab(board_aux, depth - 1, player, alpha, beta);

      if(beta.value > node.value)
      {
        beta.value = node.value;
        beta.move = move;
      }

      if(alpha.value >= beta.value)
      {
        break;
      }
      board_free(board_aux);
      moves_free(moves);
    }
    return beta;
  }
}

/*this function makes the call to the alpha-beta prungin algorithm*/
move_t minimax_ab_player(board_t *board, size_t depth)
{
  if(depth == 0)
  {
    return random_player(board, 0);
  }

  node_t node, alpha, beta;
  stone_t player = board_get_player(board);
  alpha.value = -10 - 5;
  beta.value = 10 + 5;
  node = minimax_ab(board, depth, player, alpha, beta);
  return node.move;
}

/*this function implements the minimax algorithm
for searching the best moves for the player and the
worst for the adversaire.*/
static node_t minimax(board_t *board, size_t depth, stone_t player)
{

  node_t node, best_node;
  node.value = 0;
  move_t move;
  size_t movescount = 0;

  if(depth == 0)
  {
    moves_t *movesheur = NULL;
    movesheur = board_get_moves(board);
    best_node.value = score_heuristic(board, movesheur);
    best_node.move = (move_t) {0,0};
    moves_free(movesheur);
    return best_node;
  }

  movescount = board_count_moves(board);
  if(player == board_get_player(board))
  {
    best_node.value = -INFINITY;
    for(size_t i = 0 ; i < movescount; i++)
    {
      board_t *board_aux = NULL;
      moves_t *moves = NULL;
      board_aux = board_copy(board);
      moves = board_get_moves(board_aux);
      move = moves_get(moves, i);
      board_play(board_aux, move);
      node = minimax(board_aux, depth - 1, player);

      if(best_node.value < node.value)
      {
        best_node.value = node.value;
        best_node.move = move;
      }
      board_free(board_aux);
      moves_free(moves);
    }
  }
  else
  {
    best_node.value = INFINITY;

    for(size_t i = 0 ; i < movescount; i++)
    {
      board_t *board_aux = NULL;
      moves_t *moves = NULL;
      board_aux = board_copy(board);
      moves = board_get_moves(board_aux);
      move = moves_get(moves, i);
      board_play(board_aux, move);
      node = minimax(board_aux, depth - 1, player);

      if(best_node.value > node.value)
      {
        best_node.value = node.value;
        best_node.move = move;
      }
      board_free(board_aux);
      moves_free(moves);
    }
  }
  return best_node;
}


/*this function makes the call to the minimax algorithm*/
move_t minimax_player(board_t *board, size_t depth)
{
    if(depth == 0)
    {
        return random_player(board, 0);
    }

    node_t node;
    stone_t player = board_get_player(board);
    node = minimax(board, depth, player);
    return node.move;
}


/*this function will get every entry of the player, then validate
if it is valid with a parser. if it is valid then
it will return it as a move. it will also get the name of the
file if the player want to save the game*/
move_t human_player(board_t *board, size_t param)
{
  char entry[32] = {0};
  char entryprem[32] = {0};
  char quit[8] = {0};
  char filename[32] = {0};
  char str[] = {'a','b','c','d','e','f','g','h',
  'A','B','C','D','E','F','G','H'};
  score_t scorepr;
  move_t move;
  size_t size = 0;
  FILE *fd;
  int rowval = 0;
  size_t colval = 0;
  int colerr = 0;
  int err = 0;
  char *boardstr = {0};
  boardstr = board_str(board);

  size = board_get_size(board);

  printf("\n");
  fputs(boardstr, stdout);
  board_score(board);
  scorepr = (score_t)board_score(board);
  printf("\nScore: 'X' = %zu,"
  " 'O' = %zu \n", scorepr.black, scorepr.white);

  if(board_get_player(board) == BLACK_STONE)
  {
    printf("'X' Player's turn.\n");
  }

  else if(board_get_player(board) == WHITE_STONE)
  {
    printf("'O' Player's turn.\n");
  }

  while(err == 0)
  {
    err = 1;
    move.row = 0;
    move.column = 0;
    colerr = 0;
    colval = 0;
    rowval = 0;
    printf("\nGive your move (ex.'A5' or 'a5') "
    "press 'q' or 'Q' to quit: ");
    fgets(entryprem, 32, stdin);
    int j = 0;

    for(int i = 0; i < (int) strlen(entryprem) - 1; i++)
    {
      if(entryprem[i] != ' ')
      {
        entry[j++] = entryprem[i];
      }
    }

    entry[strlen(entryprem)- (j-1)] = '\0';

    if (entry[0] == 'Q' || entry[0] == 'q')
    {
      printf("Quittin, do you want to "
      "save this game (y/n)?:\n");
      fgets(quit, 8, stdin);

      if (quit[0] == 'y' || quit[0] == 'Y')
      {
        printf("Give a filename to save the game "
        "(default: 'board.txt')\n");
        fgets(filename, 32, stdin);

         if (filename[0] == '\n' || filename[0] == ' ')
        {
          strcpy(filename, "board.txt");
          fd = fopen(filename, "w");
          board_save(board, fd);
        }

        else
        {
          strtok(filename,"\n");
          fd = fopen(filename, "w");
          board_save(board, fd);
        }

        move.row = size;
        move.column = size;
        board = board_set_player(board, EMPTY_STONE);
      }

      else if (quit[0] == 'n' || quit[0] == 'N')
      {
        move.row = size;
        move.column = size;
        board = board_set_player(board, EMPTY_STONE);
      }

      else
      {
        move.row = size;
        move.column = size;
        board = board_set_player(board, EMPTY_STONE);
      }
    }

    else
    {
      if (entry[1] >= 48 && entry[1] <= 57)
      {
        if (size == 2)
        {
          if (entry[0] == 65 || entry[0] == 97)
          {
            move.row = 0;
          }

          if (entry[0] == 66 || entry[0] == 98)
          {
            move.row = 1;
          }

          for (size_t i = 0 ; i < size * 2 ; i++)
          {
            if(entry[0] == str[i])
            {
              rowval = 1;
            }
          }

          if (rowval == 0)
          {
            printf("Row out of bounds. Wrong input, try again!\n");
            err = 0;
          }

          colval = atoi(&entry[1]);

          if (colval < size)
          {
            move.column = colval;
            colerr = 1;
          }

          if (colerr != 1)
          {
            printf("Column out of bounds. Wrong Input, try again!\n\n");
            err = 0;
          }

          if (err == 1)
          {
            if(board_is_move_valid(board,move) != 1)
            {
              printf("This move is invalid. Wrong input, try again!\n");
              err = 0;
            }
          }
        }

        if (size == 4)
        {
          if (entry[0] == 65 || entry[0] == 97)
          {
            move.row = 0;
          }

          if (entry[0] == 66 || entry[0] == 98)
          {
            move.row = 1;
          }

          if (entry[0] == 67 || entry[0] == 99)
          {
            move.row = 2;
          }

          if (entry[0] == 68 || entry[0] == 100)
          {
            move.row = 3;
          }

          for (size_t i = 0 ; i < size * 2 ; i++)
          {
            if(entry[0] == str[i])
            {
              rowval = 1;
            }
          }

          if (rowval == 0)
          {
            printf("Row out of bounds. Wrong input, try again!\n");
            err = 0;
          }

          colval = atoi(&entry[1]);

          if (colval < size)
          {
            move.column = colval;
            colerr = 1;
          }

          if (colerr != 1)
          {
            printf("Column out of bounds. Wrong input, try again!\n");
            err = 0;
          }

          if (err == 1)
          {
            if(board_is_move_valid(board,move) != 1)
            {
              printf("This move is invalid. Wrong input, try again!\n");
              err = 0;
            }
          }

        }//end if 4

        if (size == 6)
        {
          if (entry[0] == 65 || entry[0] == 97)
          {
            move.row = 0;
          }

          if (entry[0] == 66 || entry[0] == 98)
          {
            move.row = 1;
          }

          if (entry[0] == 67 || entry[0] == 99)
          {
            move.row = 2;
          }

          if (entry[0] == 68 || entry[0] == 100)
          {
            move.row = 3;
          }

          if (entry[0] == 69 || entry[0] == 101)
          {
            move.row = 4;
          }

          if (entry[0] == 70 || entry[0] == 102)
          {
            move.row = 5;
          }

          for (size_t i = 0 ; i < size * 2 ; i++)
          {
            if(entry[0] == str[i])
            {
              rowval = 1;
            }
          }

          if (rowval == 0)
          {
            printf("Row out of bounds. Wrong input, try again!\n");
            err = 0;
          }

          colval = atoi(&entry[1]);

          if (colval < size)
          {
            move.column = colval;
            colerr = 1;
          }

          if (colerr != 1)
          {
            printf("Column out of bounds. Wrong input, try again!\n");
            err = 0;
          }

          if (err == 1)
          {
            if(board_is_move_valid(board,move) != 1)
            {
              printf("This move is invalid. Wrong input, try again!\n");
              err = 0;
            }
          }
        }//end if 6

        if (size == 8)
        {
          if (entry[0] == 65 || entry[0] == 97)
          {
            move.row = 0;
          }

          if (entry[0] == 66 || entry[0] == 98)
          {
            move.row = 1;
          }

          if (entry[0] == 67 || entry[0] == 99)
          {
            move.row = 2;
          }

          if (entry[0] == 68 || entry[0] == 100)
          {
            move.row = 3;
          }

          if (entry[0] == 69 || entry[0] == 101)
          {
            move.row = 4;
          }

          if (entry[0] == 70 || entry[0] == 102)
          {
            move.row = 5;
          }

          if (entry[0] == 71 || entry[0] == 103)
          {
            move.row = 6;
          }

          if (entry[0] == 72 || entry[0] == 104)
          {
            move.row = 7;
          }

          for (size_t i = 0 ; i < size * 2 ; i++)
          {
            if(entry[0] == str[i])
            {
              rowval = 1;
            }
          }

          if (rowval == 0)
          {
            printf("Row out of bounds. Wrong input, try again!\n");
            err = 0;
          }

          colval = atoi(&entry[1]);

          if (colval < size)
          {
            move.column = colval;
            colerr = 1;
          }

          if (colerr != 1)
          {
            printf("Column out of bounds. Wrong input, try again!\n");
            err = 0;
          }

          if (err == 1)
          {
            if(board_is_move_valid(board,move) != 1)
            {
              printf("This move is invalid. Wrong input, try again!\n");
              err = 0;
            }
          }
        }//end if 8
      }//end if entryy

      else
      {
        printf("Wrong input, try again!\n");
        err = 0;
      }
    }//end else
  }//end while
  free(boardstr);
  return move;
}//end func
