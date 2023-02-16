#ifndef GAME_INCLUDE
#define GAME_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NULL_TOKEN -1
#define INVALID_STRING -2
#define MEMORY_FAILURE -3
#define POINTER_NOT_FOUND_IN_ARRAY -4

#define PIECE_DOES_NOT_EXIST 1
#define INVALID_PIECE_CHAR 2
#define INVALID_PIECE_COLOR 3
#define INVALID_BOARD_POSITION 4
#define INVALID_CLOCK_VALUE 5
#define INVALID_MOVE 6
#define INVALID_PIECE_TYPE 7

#endif
