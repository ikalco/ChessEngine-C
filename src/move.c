#include "include/game.h"

char file_to_char(int file)
{
	switch (file)
	{
	case 0:
		return 'a';
		break;
	case 1:
		return 'b';
		break;
	case 2:
		return 'c';
		break;
	case 3:
		return 'd';
		break;
	case 4:
		return 'e';
		break;
	case 5:
		return 'f';
		break;
	case 6:
		return 'g';
		break;
	case 7:
		return 'h';
		break;
	default:
		abort();
		break;
	}
}

char rank_to_char(int rank)
{
	return (char)(7 - rank) + '1';
}

int char_to_rank(char rank)
{
	int rank_ = 8 - (rank - '0');
	if (rank_ < 0 || rank_ > 7)
		abort();

	return rank_;
}

int char_to_file(char file)
{
	switch (file)
	{
	case 'a':
		return 0;
		break;
	case 'b':
		return 1;
		break;
	case 'c':
		return 2;
		break;
	case 'd':
		return 3;
		break;
	case 'e':
		return 4;
		break;
	case 'f':
		return 5;
		break;
	case 'g':
		return 6;
		break;
	case 'h':
		return 7;
		break;
	default:
		abort();
		break;
	}
}

void move_to_str(const struct Move *move, char *str)
{
	if (str == NULL)
		abort();

	snprintf(str, 5, "%c%c%c%c", file_to_char(move->start_file), rank_to_char(move->start_rank), file_to_char(move->target_file), rank_to_char(move->target_rank));
}

void str_to_move(struct Move *move, const char *str)
{
	if (move == NULL || str == NULL || strlen(str) != 4)
		abort();

	move->start_file = char_to_file(str[0]);
	move->start_rank = char_to_rank(str[1]);
	move->target_file = char_to_file(str[2]);
	move->target_rank = char_to_rank(str[3]);
	move->removed_piece = NULL;
}

struct Move *add_move_array(struct Move ***array, int *array_size)
{
	int size = *array_size;
	for (int j = 0; j < size; j++)
	{
		if ((*array)[j] == NULL)
		{
			(*array)[j] = malloc(sizeof(struct Move));
			return (*array)[j];
		}
		else if (j == size - 1)
		{
			size *= 2;

			(*array) = realloc((*array), size * (sizeof(struct Move *)));

			int j = *array_size;

			for (int k = j; k < size; k++)
			{
				(*array)[k] = NULL;
			}

			(*array)[j] = malloc(sizeof(struct Move));

			*array_size = size;
			return (*array)[j];
		}
	}

	return NULL;
}

void push_move_array(struct Move ***array, int *array_size, struct Move *move)
{
	int size = *array_size;

	if (size == 0)
	{
		size *= 2;

		(*array) = realloc((*array), size * (sizeof(struct Move *)));

		int j = *array_size;

		for (int k = j; k < size; k++)
		{
			(*array)[k] = NULL;
		}

		(*array)[j] = move;

		*array_size = size;
		return;
	}

	for (int j = 0; j < size; j++)
	{
		if ((*array)[j] == NULL)
		{
			(*array)[j] = move;
			return;
		}
		else if (j == size - 1)
		{
			size *= 2;

			(*array) = realloc((*array), size * (sizeof(struct Move *)));

			int j = *array_size;

			for (int k = j; k < size; k++)
			{
				(*array)[k] = NULL;
			}

			(*array)[j] = move;

			*array_size = size;
			return;
		}
	}

	abort(); // exit_game(game, MEMORY_FAILURE);
}

void pop_move_array(struct Move ***array, int *array_size, struct Move **move)
{
	if (*array_size < 1)
		return;

	int size = *array_size;

	for (int i = 0; i < size; i++)
	{
		if ((*array)[i] != NULL)
			continue;

		if (i != 0)
		{
			*move = (*array)[i - 1];
			(*array)[i - 1] = NULL;
			return;
		}
		else
		{
			move = NULL;
		}
	}
}

void filter_moves_array_by_color(struct Game *game, enum Color color, struct Move ***array, int *array_size)
{
	int size = *array_size;
	struct Move **new_array = malloc(sizeof(struct Move *) * size);
	int next_index = 0;

	for (int i = 0; i < size; i++)
	{
		struct Move *move = (*array)[i];
		if (move == NULL)
			abort();

		if (game->board[move->start_rank][move->start_file]->color == color)
		{
			new_array[next_index] = move;
			next_index++;
		}
		else
		{
			free(move);
		}
	}

	free(*array);

	*array_size = next_index;
	(*array) = realloc(new_array, *array_size * (sizeof(struct Move *)));
}

void generate_psuedo_legal_moves(struct Game *game, struct Move ***psuedo_legal_moves, int *psuedo_legal_moves_size)
{
	struct Move ***all_king_moves = malloc(sizeof(struct Move **));
	int *all_king_moves_size = malloc(sizeof(int));
	generate_all_king_moves(game, all_king_moves, all_king_moves_size);

	struct Move ***all_queen_moves = malloc(sizeof(struct Move **));
	int *all_queen_moves_size = malloc(sizeof(int));
	generate_all_queen_moves(game, all_queen_moves, all_queen_moves_size);

	struct Move ***all_rook_moves = malloc(sizeof(struct Move **));
	int *all_rook_moves_size = malloc(sizeof(int));
	generate_all_rook_moves(game, all_rook_moves, all_rook_moves_size);

	struct Move ***all_bishop_moves = malloc(sizeof(struct Move **));
	int *all_bishop_moves_size = malloc(sizeof(int));
	generate_all_bishop_moves(game, all_bishop_moves, all_bishop_moves_size);

	struct Move ***all_knight_moves = malloc(sizeof(struct Move **));
	int *all_knight_moves_size = malloc(sizeof(int));
	generate_all_knight_moves(game, all_knight_moves, all_knight_moves_size);

	struct Move ***all_pawn_moves = malloc(sizeof(struct Move **));
	int *all_pawn_moves_size = malloc(sizeof(int));
	generate_all_pawn_moves(game, all_pawn_moves, all_pawn_moves_size);

	*psuedo_legal_moves_size = *all_king_moves_size + *all_queen_moves_size + *all_rook_moves_size + *all_bishop_moves_size + *all_knight_moves_size + *all_pawn_moves_size;

	*psuedo_legal_moves = malloc(*psuedo_legal_moves_size * sizeof(struct Move *));

	int offset = 0;

	struct Move ***moves_arrays[6] = {all_king_moves, all_queen_moves, all_rook_moves, all_bishop_moves, all_knight_moves, all_pawn_moves};
	int *moves_arrays_sizes[6] = {all_king_moves_size, all_queen_moves_size, all_rook_moves_size, all_bishop_moves_size, all_knight_moves_size, all_pawn_moves_size};

	for (int j = 0; j < 6; j++)
	{
		int size = *moves_arrays_sizes[j];
		for (int i = 0; i < size; i++)
		{
			struct Move *move = (*moves_arrays[j])[i];
			if (move == NULL)
			{
				break;
			}
			else
			{
				(*psuedo_legal_moves)[offset] = move;
				offset++;
			}
		}

		free(*moves_arrays[j]);
		free(moves_arrays[j]);
		free(moves_arrays_sizes[j]);
	}

	*psuedo_legal_moves = realloc(*psuedo_legal_moves, offset * sizeof(struct Move *));
	*psuedo_legal_moves_size = offset;
}

void generate_moves(struct Game *game, struct Move ***moves, int *moves_size)
{
	struct Move ***psuedo_legal_moves = malloc(sizeof(struct Move **));
	int *psuedo_legal_moves_size = malloc(sizeof(int));
	generate_psuedo_legal_moves(game, psuedo_legal_moves, psuedo_legal_moves_size);

	// calculate pinned pieces
	// add moves that BLOCK or CAPTURE the pinning piece

	// calculate attacks

	// calculate check
	// add moves that BLOCK or CAPTURE the checking piece

	// calculate castling moves

	filter_moves_array_by_color(game, game->active_player_color, psuedo_legal_moves, psuedo_legal_moves_size);

	*moves = *psuedo_legal_moves;
	*moves_size = *psuedo_legal_moves_size;
}
