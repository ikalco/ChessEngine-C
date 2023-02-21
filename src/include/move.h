#pragma once

struct Move;

#include "game.h"

struct Move
{
	struct Piece *removed_piece;
	unsigned short start_file : 3;
	unsigned short start_rank : 3;
	unsigned short target_file : 3;
	unsigned short target_rank : 3;
};

struct ExpectedMove
{
	struct Move move;
	int expected_result;
};

struct Move *add_move_array(struct Move ***array, int *array_size);
void push_move_array(struct Move ***array, int *array_size, struct Move *move);
void pop_move_array(struct Move ***array, int *array_size, struct Move **move);

struct ExpectedMove *add_expected_move_array(struct ExpectedMove ***array, int *array_size);
void push_expected_move_array(struct ExpectedMove ***array, int *array_size, struct ExpectedMove *move);
void pop_expected_move_array(struct ExpectedMove ***array, int *array_size, struct ExpectedMove **move);
void downsize_expected_move_array(struct ExpectedMove ***array, int *array_size);

void move_to_str(const struct Move *move, char *str);
void str_to_move(struct Move *move, const char *str);

void generate_psuedo_legal_moves(struct Game *game, struct Move ***psuedo_legal_moves, int *size);
void generate_moves(struct Game *game, struct Move ***moves, int *moves_size);
