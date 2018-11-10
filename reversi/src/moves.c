#include "moves.h"
#include "board.h"
#include "player.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

struct moves_t
{
  size_t size;
  move_t *list;
};

/*this function will alloc the space in memory
for a moves structure of a given size that will stock 
the possible moves in a board. it will return the moves_t*/
moves_t *moves_alloc(size_t size)
{
  moves_t *movesalloc;

  if(size > 0)
  {
    movesalloc = malloc(sizeof(moves_t));

    if (movesalloc == NULL)
    {
      return NULL;
    }

    movesalloc->list = calloc(size, sizeof(move_t));

    if (movesalloc->list == NULL)
    {
      return NULL;
    }

    if (movesalloc != NULL)
    {
      movesalloc->size = size;
    }

    return movesalloc;
  }

  else
  {
    movesalloc = NULL;
    return movesalloc;
  }
}

/*we will free the memory space of a given moves structure*/
void moves_free(moves_t *moves)
{
  if (moves != NULL)
  {
    free(moves->list);
    free(moves);
  }
}

/*this function returns the number of possible moves
inside a moves structure*/
size_t moves_count(moves_t *moves)
{
  if (moves == NULL)
  {
    return 0;
  }

  else
  {
    size_t size;
    size = moves->size;
    return size;
  }
}

/*this function will return the nth given move
in the given moves structure*/
move_t moves_get(moves_t *moves, size_t n)
{
  move_t moven;
  if(moves != NULL)
  { 
    if(n < moves->size)
    {
      moven = moves->list[n];
    }

    else
    {
      moven.row = MAX_SIZE;
      moven.column = MAX_SIZE;
    }
  }
  return moven;
}

/*this function will set the given move in the 
n space of the given moves structure*/
int moves_set(moves_t *moves, size_t n, move_t move)
{
  int valid = 0;
  if(moves != NULL)
  {
    if(n < moves->size)
    {
      moves->list[n] = move;
    }

    else
    {
      valid = -1;
    }
  }
  return valid;
}

/*we make an exact copy of a given moves structure and
return it to the caller*/
moves_t *moves_copy(moves_t *moves)
{
  moves_t *movescopy;
  if (moves == NULL)
  {
    return NULL;
  }

  else
  {
    size_t size = moves->size;
    movescopy = moves_alloc(size);
    for (size_t i = 0; i < size ; i++)
    {
      movescopy->list[i] = moves->list[i];
    }
  }
  return movescopy;
}

/*this function returns a string of characters of a 
specific move {row, column}*/
char *move2str(move_t move)
{
  char *movestr = calloc(8, sizeof(char*));
  //we're usin 8 characters for the impression
  if (movestr == NULL)
  {
    return NULL;
  }

  sprintf(movestr, "{ %zu , %zu }", move.row, move.column);
  return movestr;
}

/*this function returns a string of characters with
all the posible moves in a moves structure*/
char *moves2str(moves_t *moves)
{
  if(moves == NULL)
  {
    return NULL;
  }

  size_t size = moves->size;
  size_t i = 0;
  char *buff = {0};
  char *movesstr = calloc(size, sizeof(char*));
  
  if (movesstr == NULL)
  {
    return NULL;
  }

  for (i = 0; i < size; i++)
  {
    buff = move2str(moves->list[i]);
  }
  strcat(movesstr, buff);
  free(buff);
  return movesstr;
}
