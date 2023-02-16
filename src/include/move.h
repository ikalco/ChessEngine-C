#pragma once

struct Move;

#include "game.h"

struct Move
{
	unsigned int start_file;
	unsigned int start_rank;
	unsigned int target_file;
	unsigned int target_rank;
	struct Piece *removed_piece;
};

struct Move *add_move_array(struct Move ***array, int *array_size);
void push_move_array(struct Move ***array, int *array_size, struct Move *move);
void pop_move_array(struct Move ***array, int *array_size, struct Move **move);

void move_to_str(const struct Move *move, char *str);
void str_to_move(struct Move *move, const char *str);

void generate_psuedo_legal_moves(struct Game *game, struct Move ***psuedo_legal_moves, int *size);
void generate_moves(struct Game *game, struct Move ***moves, int *moves_size);
