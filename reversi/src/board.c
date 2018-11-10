#include "board.h"
#include "moves.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>

#include <err.h>
#include <string.h>

/*Internal board_t structure (hidden from the outside)*/
struct board_t
{
  size_t size;
  stone_t player;
  stone_t **grid;
  moves_t *moves;
};

struct moves_t
{
  size_t size;
  move_t *list;
};

/* This function will allocate in memory the needed space
for a board. We will use the parameter size to calculate
the needed space and player pour dire wich player will
start the game. We return the board.*/
board_t *board_alloc (size_t size, stone_t player)
{
  board_t *board = malloc (size * sizeof(board_t));
  if (board == NULL)
  {
    return NULL;
  }

  board->grid = malloc (size * sizeof(stone_t*));
  if (board->grid == NULL)
  {
    free(board);
    return NULL;
  }

  board->moves = moves_alloc(1);
  if (board->moves == NULL)
  {
    free(board->grid);
    free(board);
    return(NULL);
  }

  for (size_t i = 0; i < size; i++)
  {
    board->grid[i] = calloc(size, sizeof(stone_t));
    if(board->grid[i] == NULL)
    {
      for (size_t k = 0; k < i; k++)
      {
        free(board->grid[k]);
      }
      free(board->grid);
      moves_free(board->moves);
      free(board);
    }
  }

  board->size = size;
  board->player = player;
  return board;
}


/* this function creates and initialize the board
with the 4 initial stones placed in the center
we use the parameter size to create the board and in the end
we return it already initialized */
board_t *board_init (size_t size)
{
  if(size == 2 || size == 4 || size == 6 || size == 8)
  {
    board_t *boardinit = board_alloc(size, BLACK_STONE);

    if (boardinit == NULL)
    {
      return NULL;
    }

    for(size_t i = 0; i < size ; i++)
    {
      for(size_t j = 0; j < size ; j++)
      {
        boardinit->grid[i][j] = EMPTY_STONE;
      }
    }

    boardinit->grid[size / 2 - 1][size / 2 - 1] = WHITE_STONE;
    boardinit->grid[size / 2 ][size / 2 ] = WHITE_STONE;
    boardinit->grid[size / 2 - 1][size / 2 ] = BLACK_STONE;
    boardinit->grid[size / 2 ][size / 2 - 1 ] = BLACK_STONE;

    return boardinit;
  }

  else
    return NULL;
}

/*frees the espace in memory used by the board in the parameter
it doesnt return nothing*/
void board_free (board_t *board)
{
  if (board == NULL)
  {
    return;
  }

  for (size_t i = 0; i < board->size; i++)
    {
      free(board->grid[i]);
    }
  moves_free(board->moves);
  free(board->grid);
  free(board);
}

/*we do an exact copy of the board in the parametre
after the fonction we return a new board with the
same caractheristiques of the other*/
board_t *board_copy (board_t *board)
{
  if (board == NULL)
  {
    return NULL;
  }

  else
  {
    board_t *copy = NULL;
    copy = board_init(board->size);
    copy->player = board->player;

    if (copy == NULL)
    {
      err(EXIT_FAILURE, NULL);
    }

    for (size_t i = 0; i < board->size; i++)
    {
      for (size_t j = 0; j < board->size; j++)
      {
        copy->grid[i][j] = board->grid[i][j];
      }
    }
    copy->moves = moves_copy(board->moves);
    return copy;
  }
}

/*this function will place a stone, given as parameter
in the row and column of the board given as parameter as well
we return an integer value to verify that the process
have been done properly*/
int board_set (board_t *board, stone_t stone,
size_t row, size_t column)
{
  int ver = -1;

  if (board != NULL)
  {
    if (row < board->size && column < board->size)
    {
      if (stone == BLACK_STONE)
      {
        board->grid[row][column] = BLACK_STONE;
        ver = 0;
      }

      else if (stone == WHITE_STONE)
      {
        board->grid[row][column] = WHITE_STONE;
        ver = 0;
      }

      else if (stone == EMPTY_STONE)
      {
        board->grid[row][column] = EMPTY_STONE;
        ver = 0;
      }
    }
  }

return ver;
}

/*this function will return the stone that is in the
column an row in the given board, if the space is
empty it will return EMPTY_STONE*/
stone_t board_get (board_t *board, size_t row, size_t column)
{
  stone_t stone;

  if (board != NULL)
  {
    if (row < board->size && column < board->size)
    {
      stone = board->grid[row][column];
      return stone;
    }
    else
    return EMPTY_STONE;
  }
  else
  return EMPTY_STONE;
}


/*we will calculate and return the score of each player, by counting
how many stones it has in the given board*/
score_t board_score (board_t *board)
{

  score_t score;
  score.white = 0;
  score.black = 0;

  if (board != NULL)
  {
    for (size_t i = 0; i < board->size; i++)
    {
      for (size_t j = 0; j < board->size; j++)
      {
        if (board->grid[i][j] == BLACK_STONE)
        {
          score.black++;
        }

        if (board->grid[i][j] == WHITE_STONE)
        {
          score.white++;
        }
      }
    }
  }

  return score;
}


/*this function returns the size of the given board*/
size_t board_get_size (board_t *board)
{
  size_t size = 0;
  if (board != NULL)
  {
    size = board->size;
  }

  return size;
}


/*this function returns the current player of the given board*/
stone_t board_get_player (board_t *board)
{
  stone_t player;
  if (board != NULL)
  {
    if (board->size == 2 || board_count_moves(board) == 0)
    {
      return EMPTY_STONE;
    }

    else
    {
      player = board->player;
    }
  }
  return player;
}


/*this functions transforms the board and its stones
in a string, wich we will be able to print after that
we will return an array of characters with the board*/
char *board_str (board_t *board)
{

  if (board == NULL)
  {
    return NULL;
  }

  else
  {
    size_t size = board->size;
    char *boardstring = calloc(size * size, sizeof(char*));
    char *buff = calloc(size, sizeof(char*));
    char countrows = '@';
    int countcolumns = 0;
    move_t move;
    strcat(boardstring, "   ");

    for (size_t t = 0; t < size; t++)
    {
      sprintf(buff, "%d ", countcolumns++);
      strcat(boardstring, buff);
    }

    strcat(boardstring, "\n");

    for (size_t i = 0; i < size ; i ++)
    {
      countrows++;
      sprintf(buff, "%c  ", countrows);
      strcat(boardstring, buff);

      for(size_t j = 0; j < size ; j++)
      {
        move.column = j;
        move.row = i;

        if (j != size - 1)
        {
          if(board_is_move_valid(board,move) == 1)
          {
            sprintf(buff, "* ");
          }

          else
          {
            sprintf(buff,"%c ", (char)board->grid[i][j]);
          }
        }

        else
        {
          if(board_is_move_valid(board,move) == 1)
          {
            sprintf(buff, "*\n");
          }

          else
          {
            sprintf(buff, "%c\n", (char)board->grid[i][j]);
          }
        }
        strcat(boardstring, buff);
      }
    }
  free(buff);
  return boardstring;
  }
}

/*here we will print the given board by using the character string
returned by board_str, we could save it in a file or print it
in the screen. we wont return nothing.*/
void board_print (board_t *board, FILE *fd)
{
  if(board != NULL && fd != NULL)
  {
    char *boardstr = {0};
    boardstr = board_str(board);
    if(fd != stdout)
    {
      fd = fopen ("BOARD.txt", "w+");
      fprintf (fd, "%s", boardstr);
      score_t scorepr = board_score(board);
      fprintf(fd, "\nScore: 'X' = %zu "
      ", 'O' = %zu \n", scorepr.black, scorepr.white);
    }
    else
    {
      fputs(boardstr, stdout);
      score_t scorepr = board_score(board);
      fprintf(fd, "\nScore: 'X' = %zu "
      ", 'O' = %zu \n", scorepr.black, scorepr.white);
    }
    free(boardstr);
  }
}

/*this function will validate if the given move
is valid in the given board. In other words, it will
calculate if there is the other players stones
at its sides so it could make a valid move.
we return a boolean if the move is "valid"*/

bool board_is_move_valid(board_t *board, move_t move)
{
  size_t i, j, size;
  stone_t opponent, player;
  i = move.row;
  j = move.column;
  
  if(board == NULL || (i > board->size) || (j > board->size))
  {
    return false;
  }
  
  //we have to verify if the case is empty
  if(board->grid[move.row][move.column] != EMPTY_STONE)
  {
    return false;
  }

  player = board->player;
  size = board->size;

  if(player == BLACK_STONE)
  {
    opponent = WHITE_STONE;
  }

  else if (player == WHITE_STONE)
  {
    opponent = BLACK_STONE;
  }

  else
  {
    return false;
  }

  //up vertical
  i = (move.row == 0) ? 0 : (move.row - 1);
  j = move.column;

  while((i > 0)&& (board->grid[i][j] == opponent))
  {
    i--;
  }

  if (((move.row - i) > 1) && (board->grid[i][j] == player))
  {
    return true;
  }

  //down vertical

  i = (move.row == size - 1 ) ? (size - 1) : (move.row + 1);
  j = move.column;

  while((i < size -1 ) && (board->grid[i][j] == opponent))
  {
    i++;
  }

  if(((i - move.row) > 1) && (board->grid[i][j] == player))
  {
    return true;
  }

  //left horizontal
  i = move.row;
  j = (move.column == 0) ?0 : (move.column -1);

  while((j > 0) && (board->grid[i][j] == opponent))
  {
    j--;
  }

  if (((move.column - j) > 1) && (board->grid[i][j] == player))
  {
    return true;
  }

  //right horizontal
  i = move.row;
  j = (move.column == size - 1) ? (size - 1) : (move.column + 1);

  while((j < size - 1) && (board->grid[i][j] == opponent))
  {
    j++;
  }

  if(((j - move.column) > 1) && (j < size) && board->grid[i][j] == player)
  {
    return true;
  }

  //down /
  i = (move.row == size - 1 ) ? (size - 1) : (move.row + 1);
  j = (move.column == 0) ? 0 : (move.column - 1);

  while((i < size - 1) && (j > 0) && (board->grid[i][j] == opponent))
  {
    i++;
    j--;
  }

  if(((i - move.row) > 1) && (board->grid[i][j] == player))
  {
    return true;
  }

  //down \

  i = (move.row == size - 1 ) ? (size - 1) : (move.row + 1);
  j = (move.column == size - 1) ? (size - 1) : (move.column + 1);

  while((i < size - 1) && (j < size - 1) &&
  (board->grid[i][j] == opponent))
  {
    i++;
    j++;
  }

  if(((i - move.row) > 1) && (board->grid[i][j] == player))
  {
    return true;
  }

  // up \ 

  i = (move.row == 0) ? 0 : (move.row - 1);
  j = (move.column == 0) ? 0 : (move.column - 1);

  while((i > 0) && (j > 0) && (board->grid[i][j] == opponent))
  {
    i--;
    j--;
  }

  if(((move.row - i) > 1) && (board->grid[i][j] == player))
  {
    return true;
  }

  //up /
  i = (move.row == 0 ) ? 0 : (move.row -1);
  j = (move.column == size - 1) ? (size - 1) : (move.column + 1);

  while((i > 0)&& ( j< size - 1) && (board->grid[i][j] == opponent))
  {
    i--;
    j++;
  }

  if(((move.row - i) > 1) && (board->grid[i][j] == player))
  {
    return true;
  }

  return false;
}


/*this function will do the play, with the given move in
the given board, this means that it will turn over the
existing stones in between
the caller should validate if the move is
valid before calling this function. It will return
a bool if it works without problems*/
bool board_play (board_t *board, move_t move)
{
  stone_t adv;
  bool stat = false;
  size_t i, j;
  board_t *brd = board;

  if (board != NULL)
  {
    if (brd->player == BLACK_STONE)
    {
      adv = WHITE_STONE; //adversaire
    }

    else if (brd->player == WHITE_STONE)
    {
      adv = BLACK_STONE;
    }

    else
      return false;

    if (move.row < brd->size && move.column < brd->size)
    {

      brd->grid[move.row][move.column] = brd->player;
      i = (move.row == 0) ? 0 : (move.row - 1);
      j = move.column;

      while ((i > 0) && (brd->grid[i][j] == adv))
      {
        i--;
      }

      if (((move.row - i) > 1) && (brd->grid[i][j] == brd->player))
      {
        for (i++; i < move.row; i++)
        {
          brd->grid[i][j] = brd->player;
        }
        stat = true;
      }

      i = (move.row == brd->size - 1) ? (brd->size - 1) : (move.row + 1);
      j = move.column;

      while ((i < brd->size - 1) && (brd->grid[i][j] == adv))
      {
        i++;
      }

      if (((i - move.row) > 1) && (brd->grid[i][j] == brd->player))
      {
        for (i--; i > move.row; i--)
        {
          brd->grid[i][j] = brd->player;
        }
        stat = true;
      }

      i = move.row;
      j = (move.column == brd->size - 1)
      ? (brd->size - 1) : (move.column + 1);

      while ((j < brd->size - 1) && (brd->grid[i][j] == adv))
      {
        j++;
      }

      if(((j - move.column) > 1) &&
      (j < brd->size) && (brd->grid[i][j] == brd->player))
      {
        for (j--; j > move.column; j--)
        {
          brd->grid[i][j] = brd->player;
        }
        stat = true;
      }

      i = (move.row == 0) ? 0 : (move.row - 1);
      j = (move.column == brd->size - 1)
      ? (brd->size - 1) : (move.column + 1);

      while ((i > 0) && (j < brd->size - 1) && (brd->grid[i][j] == adv))
      {
        i--;
        j++;
      }

      if (((move.row - i) > 1) && (brd->grid[i][j] == brd->player))
      {
        for (size_t k = 1; k < move.row - i; k++)
        {
          brd->grid[i + k][j - k] = brd->player;
        }
        stat = true;
      }

      i = (move.row == brd->size - 1)
      ? (brd->size - 1) : (move.row + 1);
      j = (move.column == brd->size - 1)
      ? (brd->size - 1) : (move.column + 1);

      while((i < brd->size - 1) &&
      (j < brd->size - 1) && (brd->grid[i][j] == adv))
      {
        i++;
        j++;
      }

      if (((i - move.row) > 1) && (brd->grid[i][j] == brd->player))
      {
        for (size_t k = 1; k < i - move.row; k++)
        {
          brd->grid[i - k][j - k] = brd->player;
        }
        stat = true;
      }

      i = move.row;
      j = (move.column == 0) ? 0 : (move.column - 1);

      while ((j > 0) && (brd->grid[i][j] == adv))
      {
        j--;
      }

      if (((move.column - j) > 1)
      && (brd->grid[i][j] == brd->player))
      {
        for (j++; j < move.column; j++)
        {
          brd->grid[i][j] = brd->player;
        }
        stat = true;
      }

      i = (move.row == 0) ? 0 : (move.row - 1);
      j = (move.column == 0) ? 0 : (move.column - 1);

      while ((i > 0) && (j > 0) && (brd->grid[i][j] == adv))
      {
        i--;
        j--;
      }

      if (((move.row - i) > 1) && (brd->grid[i][j] == brd->player))
      {
        for (size_t k = 1; k < move.row - i; k++)
        {
          brd->grid[i + k][j + k] = brd->player;
        }
        stat = true;
      }

      i = (move.row == brd->size - 1)
      ? (brd->size - 1) : (move.row + 1);
      j = (move.column == 0) ? 0 : (move.column - 1);

      while ((i < brd->size - 1) && (j > 0) && (brd->grid[i][j] == adv))
      {
        i++;
        j--;
      }

      if (((i - move.row) > 1) && (brd->grid[i][j] == brd->player))
      {
        for (size_t k = 1; k < move.column - j; k++)
        {
          brd->grid[i - k][j + k] = brd->player;
        }
        stat = true;
      }

    }

    if(stat == true)
    {
      if(brd->player == BLACK_STONE)
      {
        brd->player = WHITE_STONE;
        return stat;
      }

      else if(brd->player == WHITE_STONE)
      {
        brd->player = BLACK_STONE;
        return stat;
      }
    }

  }//end if board null
  return stat;
}


/*this functions save the current game of the given board
we must save it in a file with a given name*/
void board_save(board_t *board, FILE *fd)
{

  size_t size;
  if (board != NULL && fd != NULL)
  {
    size = board->size;

    char *boardstring = calloc(size * (size), sizeof(char*));
    char *buff = calloc(size * size, sizeof(char*));

    for(size_t i = 0; i < size; i++)
    {
      for(size_t j = 0; j < size; j++)
      {
        if(board->grid[i][j] == BLACK_STONE
        || board->grid[i][j] == WHITE_STONE
        || board->grid[i][j] == EMPTY_STONE)
        {
          sprintf(buff, "%c", (char)board->grid[i][j]);
        }

        strcat (boardstring, buff);

        if (j % size == size - 1)
        {
          sprintf (buff, "\n");
          strcat (boardstring, buff);
        }

        else
        {
          sprintf (buff, " ");
          strcat (boardstring, buff);
        }
      }
    }
    fprintf(fd, "%c\n%s", (char)board->player, boardstring);
    free(buff);
    free(boardstring);
  }
}


/*this function will load a board, if theres any with the
given filename, and it will return the board so
we could start playing again*/
/*it has a parser that will not accept if the file is not valid
or with a unusual format of board*/
board_t *board_load(char *filename)
{
  FILE *fd;
  board_t *board = NULL;
  if ((fd = fopen (filename, "r")) != NULL)
  {
    int size = 0;
    int filerows = 0;
    char str[80] = {0};
    bool firstrow = true;
    stone_t player = EMPTY_STONE;
    char auxchar = '\0';
    int aux = 0;
    int row = -1;
    int col = 0;
    int blacks = 0;
    int whites = 0;

    while ((fgets (str, 80, fd) != NULL) && (auxchar != EOF))
    {
      aux = 0;

      while ((str[aux] == ' ') || (str[aux] =='\t'))
      {
        aux++;
      }

      auxchar = str[aux];

      if (((auxchar != '\n') && (auxchar != '#')
      && (!firstrow)) && ((auxchar == EMPTY_STONE)
      || (auxchar == BLACK_STONE) || (auxchar == WHITE_STONE)))
      {
        row++;
        col = 0;
      }

      while ((auxchar != '\n') && (auxchar != '#'))
      {
        if ((auxchar == EMPTY_STONE) || (auxchar == BLACK_STONE)
        || (auxchar == WHITE_STONE))
        {
          if (firstrow)
          {
            if (auxchar != EMPTY_STONE)
            {
              player = (stone_t)auxchar;
              firstrow = false;
            }

            else
            {
              printf("ERROR: No player.\n");
              return NULL;
            }
          }

          else
          {
            if (size == 0)
            {
              char charac = auxchar;
              int s = aux;
              while (charac != '\n')
              {
                size++;
                s++;
                while((str[s] == ' ') || (str[s] == '\t'))
                {
                  s++;
                }
                charac = str[s];
              }

              if ((size != 8) && (size != 6) && (size != 4) && (size != 2))
              {
                return NULL;
              }

              if(size >= 2)
              {
                board = board_init(size);
              }
            }

            if ((row >= size))
            {
              printf("ERROR: Too many rows\n");
              return NULL;
            }

            if (auxchar == BLACK_STONE)
            {
              board->grid[row][col] = BLACK_STONE;
              blacks++;
            }
            else if (auxchar == WHITE_STONE)
            {
              board->grid[row][col] = WHITE_STONE;
              whites++;
            }

            if (!firstrow)
            {
              col++;
            }

            if (col > size)
            {
              printf("ERROR: Too long at line %d\n", filerows + 1);
              return NULL;
            }
          }
        }
        else
        {
          printf("ERROR: wrong "
          "character %c at line %d\n", auxchar, filerows);
          return NULL;
        }

        aux++;
        while ((str[aux] == ' ') || (str[aux] =='\t'))
        {
          aux++;
        }
        auxchar = str[aux];
      }
      filerows++;
      if (col < size)
      {
        printf ("ERROR: board width is too short at line %d\n", filerows);
        return NULL;
      }
    }

    if ((size == 0) || (row < (size - 1)))
    {
      if (row < size - 1)
      {
        printf("ERROR: board has too few rows\n");
      }
      return NULL;
    }

    if(blacks == 0 && whites == 0)
    {
      printf("ERROR. Board empty.\n");
      return NULL;
    }

    board->player = player;
    board->size = size;
    fclose (fd);

    for(size_t h = 0; h < board_get_size(board); h++)
    {
      for(size_t k = 0; k < board_get_size(board); k++)
      {
        if(board->grid[h][k] == BLACK_STONE)
        {
          if(((board->grid[h + 1][k] == WHITE_STONE)
          && (board->grid[h - 1][k] == WHITE_STONE))
          || ((board->grid[h][k + 1] == WHITE_STONE)
          && (board->grid[h][k -1] == WHITE_STONE)))
          {
            printf("ERROR: Impossible board.\n");
            return NULL;
          }
        }
      }
    }

    if (board->size <= 2)
    {
      return board;
    }

    if(board_count_moves(board) < 2)
    {
      printf("ERROR: Board has not enough possible moves.\n");
      return board;
    }
  }
  return board;
}


/*this function uses the structure moves to get
the possibles moves in the given board and return it*/
moves_t *board_get_moves(board_t *board)
{
  if(board == NULL)
  {
    return NULL;
  }

  int valist = 0;
  size_t sizelist;
  sizelist = board_count_moves(board);
  size_t size = board->size;
  moves_t *moves = moves_alloc(sizelist);

  for(size_t i = 0; i < size ; i++)
  {
    for(size_t j = 0; j < size ; j++)
    {
      move_t move;
      move.row = i;
      move.column = j;
      if(board_is_move_valid(board, move) == 1)
      {
        moves_set(moves, valist, move);
        valist++;
      }
    }
  }
  return moves;
}


/*this function returns the number of valid moves in the
given board*/
int board_count_moves(board_t *board)
{
  move_t movement;
  int valids = 0;

  if(board != NULL)
  {
    for (size_t i = 0; i < board->size; i++)
    {
      for(size_t j = 0; j < board->size; j++)
      {
        movement.row = i;
        movement.column = j;
        if (board_is_move_valid(board, movement) == 1)
        {
          valids++;
        }
      }
    }
  }
  return valids;
}

/*this is an auxiliary function and it will set the given
player in the given board*/
board_t *board_set_player(board_t *board, stone_t player)
{
  if (board == NULL)
  {
    err(EXIT_FAILURE, NULL);
  }

  else
  {
    board->player = player;
  }

  return board;
}
