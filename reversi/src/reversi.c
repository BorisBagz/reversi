#include "board.h"
#include "moves.h"
#include "player.h"
#include "reversi.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <err.h>
#include <getopt.h>
#include <string.h>

static size_t board_size = 8; //default size

static struct option long_opts [] = {

  {"size", required_argument, NULL, 's'},
  {"contest", no_argument, NULL, 'c'},
  {"black-ai", no_argument, NULL, 'b'},
  {"white-ai", no_argument, NULL, 'w'},
  {"all-ai", no_argument, NULL, 'a'},
  {"help", no_argument, NULL, 'h'},
  {"verbose", no_argument, NULL, 'v'},
  {"version", no_argument, NULL, 'V'},
  {NULL, no_argument, NULL, 0},
};


static void version (void)
{
  printf ("\n%s %d. %d. %d", PROG_NAME, VERSION, SUBVERSION, REVISION);
  printf("\nThis software allows you to play REVERSI.\n");
}

static bool playervalid (board_t *board)
{
  bool valid = false;
  size_t size;
  size = board_get_size(board);
  move_t move;

  for (size_t i = 0; i < size; i++)
  {
    for (size_t j = 0; j < size; j++)
    {
      move.row = i;
      move.column = j;

      if (board_is_move_valid (board, move))
      {
        valid = true;
      }
    }
  }

  return valid;
}


int game (move_t (*white)(board_t*, size_t),
move_t (*black)(board_t*, size_t), board_t *board)
{
  int ans = 4;

  if (board != NULL)
  {
    bool blackturn = false;
    bool whiteturn = false;
    size_t size = board_get_size(board);
    move_t move;
    score_t score;
    bool gameending = false;

    while (gameending == false)
    {
      blackturn = playervalid(board);

      //round of games-black

      if (blackturn == true)
      {
        move = (*black)(board, 3);
        board_play(board, move);

        if (move.row == size)
        {
            gameending = true;
        }

        if (move.row == size || move.column == size)
        {
            return 3;
        }
      }

      else
      {
        blackturn = false;
      }


      if (gameending == false)
      {

        whiteturn = playervalid(board);

        if (whiteturn == true)
        {
          move = (*white)(board, 3);
          board_play(board, move);

          if (move.row == size)
          {
            gameending = true;
          }

          if (move.row == size || move.column == size)
          {
            return 3;
          }
        }

        else
        {
          whiteturn = false;
        }

        if ((whiteturn == false)
        && (blackturn == false))
        {
          gameending = true;
        }
      }
    }

    score = board_score(board);
    printf ("Score : 'X' = %zu; 'O' = %zu\n\n",
    score.black, score.white);

    if (score.black == score.white)
      return 0;

    if (score.black > score.white)
      return 1;

    else
      return 2;
  }

  return ans;
}


void printgamemode (int GM)
{
  switch (GM)
  {
    case 0 :
      printf ("\nBLACK PLAYER (X) is human and");
      printf(" WHITE PLAYER (0) is human.\n");
      break;
    case 1 :
      printf ("\nBLACK PLAYER (X) is A.I and");
      printf(" WHITE PLAYER (0) is human.\n");
      break;
    case 2 :
      printf ("\nBLACK PLAYER (X) is human and");
      printf(" WHITE PLAYER (0) is AI.\n");
      break;
    case 3 :
      printf ("\nBLACK PLAYER (X) is AI and");
      printf(" WHITE PLAYER (0) is AI.\n");
      break;
    default :
      printf ("\aInvalid game mode.\n");
  }
}


int main (int argc, char *argv[])
{
  bool verbose = false;
  bool contest_mode = false;
  int opc;
  int game_mode = 0;
  score_t score;
  FILE *contestfile;
  int typegame;
  size_t aux = 0;
  board_t *board = NULL;
  char *boardstr = {0};

  while ((opc = getopt_long (argc, argv, "abchs:vVw",
  long_opts, NULL)) != -1)
  {
    switch (opc)
    {
      case 's':
        aux = atoi(optarg);

        if ((aux > 0) && (aux < 5))
        {
          board_size = 2 * aux;
        }

        else
        {
          printf ("\nERROR Size not accepted\n");
          return EXIT_FAILURE;
        }

      break;

      case 'c':
        if (contest_mode == true)
          contest_mode = false;
        else
          contest_mode = true;
        break;

      case 'a':
        game_mode = 3;
        break;

      case 'b':
        game_mode = (((game_mode == 2) || (game_mode == 4)) ? 3 : 1);
        break;

      case 'h':
        printf( "\n           Reversi Help \n\n"
        "Usage: reversi [OPTIONS] [-s SIZE] [FILE]\n"
        "Play reversi interactively with humans or AIs."
        "\n"
        "-a, --all-ai\t\t""set both players as an AI\n"
        "-b, --black-ai\t""set black player as an AI\n"
        "-c, --contest\t\t""enable 'contest mode'\n"
        "-s, --size SIZE\t""board size (min=1, max=4 "
        "(default))\n"
        "-h, --help\t\t""display this help and exit\n"
        "-v, --verbose\t\t""verbose output\n"
        "-V, --version\t\t""display version and exit\n"
        "-w, --white-ai\t""set white player as an"
        "AI\n\n");

      case 'v':
        verbose = true;
        return EXIT_SUCCESS;

      case 'V':
        version();
        return EXIT_SUCCESS;

      case 'w':
        game_mode = (((game_mode == 1) || (game_mode == 4)) ? 3 : 2);
        break;

      default:
        fprintf(stderr, "\aInvalid argument try"
        " reversi -h or --help"
        " for more information\n");
        return EXIT_FAILURE;
    }
  }

  if (contest_mode == true)
  {
    if (optind < argc)
    {
      if ((contestfile = fopen(argv[optind],"r")) == NULL)
      {
        printf("Unknown arguments\n");
        return EXIT_FAILURE;
      }
      else
      {
        board = board_load(argv[optind]);
      }
    }

    else
    {
      fprintf(stderr, "Argument c without filename\n");
      return EXIT_FAILURE;
    }
  }//end contest treatment

  if (contest_mode == false)
  {
    board = board_init (board_size);
  }

  if(board != NULL)
  {
    printf("\n\tWelcome to this Reversi Game!\n");
    if(verbose == true)
    {
      printf("\nTHIS GAME WAS CREATED IN BORDEAUX, FRANCE"
      " by GONZALEZ, Boris in 2017.\n");
    }
    printgamemode(game_mode);
    printf("\nBlack player start!\n");
  }

  switch (game_mode)
  {
    case 0 : //human vs human
      typegame = game (human_player, human_player, board);
      break;
    case 1 :
      typegame = game (human_player, minimax_ab_player, board);
      break;
    case 2 :
      typegame = game (minimax_ab_player, human_player, board);
      break;
    case 3 :
      typegame = game (minimax_ab_player, random_player, board);
      break;
    default :
      printf("ERROR\n\n");
  }

  switch (typegame)
  {
    case -2 :
      printf("Player 'O' resigned. Player 'X' won the game.\n\n");
      break;
    case -1 :
      printf("Player 'X' resigned. Player 'O' won the game.\n\n");
      break;
    case 0 :
      printf("Draw game, no winner.\n\n");
      break;
    case 1 :
      printf("Player 'X' won the game !\n\n");
      break;
    case 2 :
      printf("Player 'O' won the game !\n\n");
      break;
    case 3 :
      printf("You have quited the game.\n\n");
      break;
    default :
      printf("Error when trying to play\n");
      return EXIT_FAILURE;
  }

  if (typegame != 3 && board != NULL)
  {
    boardstr = board_str(board);
    printf ("%s\n", boardstr);
    score = board_score (board);
    printf ("Final score: 'X' = %zu 'O' = %zu\n"
    "Thanks for playing, see you soon!\n\n", score.black, score.white);
    free(boardstr);
  }
  board_free (board);
  return(EXIT_SUCCESS);
}
