#include "include/game.h"

void split_string_get_num_substring(char *string_, const char *delimiter, char **results, int results_length)
{
	int str_length = strlen(string_);
	char *string = malloc(sizeof(char) * str_length);
	strncpy(string, string_, str_length);

	// get first token when splitting string by delimiter
	char *token = strtok(string, delimiter);

	if (token == NULL)
	{
		free(string);
		abort();
	}

	for (int i = 0; i < results_length; i++)
	{
		if (token == NULL)
			break;

		results[i] = token;

		// get next token when splitting string by delimiter
		token = strtok(NULL, delimiter);
	}

	free(string);
}

void load_pieces_into_board(char *fen_board, struct Game *game)
{
	char *board_rows[8];

	split_string_get_num_substring(fen_board, "/", board_rows, 8);

	for (int i = 0; i < 8; i++)
	{
		int fen_string_row_length = strlen(board_rows[i]);

		for (int j = 0; j < fen_string_row_length; j++)
		{
			char current_piece = board_rows[i][j];

			init_piece(game, current_piece, &i, &j);
		}
	}
}

void determine_active_player_color(char *active_player_color, struct Game *game)
{
	if (active_player_color[0] == 'w')
	{
		game->active_player_color = White;
	}
	else if (active_player_color[0] == 'b')
	{
		game->active_player_color = Black;
	}
	else
	{
		abort(); // exit_game(game, INVALID_PIECE_COLOR);
	}
}

void determine_castling_availability(char *castling_availability, struct Game *game)
{
	game->black_can_castle_queenside = false;
	game->black_can_castle_kingside = false;
	game->white_can_castle_queenside = false;
	game->white_can_castle_kingside = false;

	if (castling_availability[0] == '-')
		return;

	const int length_of_castling_availability = strlen(castling_availability);

	if (length_of_castling_availability == 1 || length_of_castling_availability > 4)
		abort(); // exit_game(game, INVALID_STRING);

	for (int i = 0; i < length_of_castling_availability; i++)
	{
		switch (castling_availability[i])
		{
		case 'K':
			game->white_can_castle_kingside = true;
			break;
		case 'k':
			game->black_can_castle_kingside = true;
			break;
		case 'Q':
			game->white_can_castle_queenside = true;
			break;
		case 'q':
			game->black_can_castle_queenside = true;
			break;
		default:
			abort(); // exit_game(game, INVALID_PIECE_CHAR);
			break;
		}
	}
}

void determine_last_enpassant(char *last_enpassant, struct Game *game)
{
	if (last_enpassant[0] == '-')
	{
		game->last_enpassant = NULL;
		return;
	}

	game->last_enpassant = malloc(sizeof(struct Move));

	int rank = 8 - (last_enpassant[1] - '0');
	if (!(rank >= 0 && rank < 8))
		abort();

	if (rank == 2)
	{
		game->last_enpassant->start_rank = 1;
		game->last_enpassant->target_rank = 3;
	}
	else if (rank == 5)
	{
		game->last_enpassant->start_rank = 6;
		game->last_enpassant->target_rank = 4;
	}

	int file = 0;
	switch (last_enpassant[0])
	{
	case 'a':
		file = 0;
		break;
	case 'b':
		file = 1;
		break;
	case 'c':
		file = 2;
		break;
	case 'd':
		file = 3;
		break;
	case 'e':
		file = 4;
		break;
	case 'f':
		file = 5;
		break;
	case 'g':
		file = 6;
		break;
	case 'h':
		file = 7;
		break;
	default:
		abort(); // exit_game(game, INVALID_PIECE_CHAR);
		break;
	}

	game->last_enpassant->start_file = file;
	game->last_enpassant->target_file = file;
}

void determine_halfmove_clock(char *halfmove_clock, struct Game *game)
{
	int halfmove_clock_int = (int)strtol(halfmove_clock, NULL, 10);

	if (halfmove_clock[0] != '0' && halfmove_clock_int == 0)
		abort(); // exit_game(game, INVALID_STRING);
	if (halfmove_clock_int < 0)
		abort(); // exit_game(game, INVALID_CLOCK_VALUE);

	game->halfmove_clock = halfmove_clock_int;
}

void determine_fullmove_clock(char *fullmove_clock, struct Game *game)
{
	int fullmove_clock_int = (int)strtol(fullmove_clock, NULL, 10);

	if (fullmove_clock[0] != '0' && fullmove_clock_int == 0)
		abort(); // exit_game(game, INVALID_STRING);
	if (fullmove_clock_int < 1)
		abort(); // exit_game(game, INVALID_CLOCK_VALUE);

	game->fullmove_clock = fullmove_clock_int;
}

void init_moves_history(struct Game *game)
{
	if (game->moves_history != NULL)
	{
		int size = *game->moves_history_size;
		for (int i = 0; i < size; i++)
		{
			free((*game->moves_history)[i]);
		}
		free(game->moves_history);

		game->moves_history = NULL;
		game->moves_history_size = NULL;
	}

	if (game->moves_history_size == NULL)
	{
		game->moves_history_size = malloc(sizeof(int));
	}

	if (game->moves_history == NULL)
	{
		game->moves_history = malloc(sizeof(struct Move **));
	}

	*game->moves_history_size = 8;
	*game->moves_history = malloc(sizeof(struct Move *) * (*game->moves_history_size));
}

void load_fen_string(char *fen_string, struct Game *game)
{
	char *deconstructed_fen_string[6];

	split_string_get_num_substring(fen_string, " ", deconstructed_fen_string, 6);

	char *fen_placement = deconstructed_fen_string[0];
	char *active_player_color = deconstructed_fen_string[1];
	char *castling_availability = deconstructed_fen_string[2];
	char *last_enpassant = deconstructed_fen_string[3];
	char *halfmove_clock = deconstructed_fen_string[4];
	char *fullmove_clock = deconstructed_fen_string[5];

	load_pieces_into_board(fen_placement, game);

	determine_active_player_color(active_player_color, game);

	determine_castling_availability(castling_availability, game);

	determine_last_enpassant(last_enpassant, game);

	determine_halfmove_clock(halfmove_clock, game);

	determine_fullmove_clock(fullmove_clock, game);

	init_moves_history(game);
}

int find_pointer(void **array, int array_size, void *element)
{
	for (int i = 0; i < array_size; i++)
	{
		if (array[i] == element)
		{
			return i;
		}
	}

	abort(); // exit_game(game, POINTER_NOT_FOUND_IN_ARRAY)
}

struct Piece *remove_from_piece_array(struct Game *game, struct Piece *piece)
{
	int index;
	switch (piece->type)
	{
	case King:
		abort(); // exit_game(game, INVALID_MOVE);
		break;
	case Queen:
		if (piece->color == White)
		{
			game->white_queen = NULL;
		}
		else
		{
			game->black_queen = NULL;
		}
		break;
	case Rook:
		index = find_pointer((void **)game->rook_pieces, game->num_rook_pieces, piece);
		game->rook_pieces[index] = NULL;
		break;
	case Bishop:
		index = find_pointer((void **)game->bishop_pieces, game->num_bishop_pieces, piece);
		game->bishop_pieces[index] = NULL;
		break;
	case Knight:
		index = find_pointer((void **)game->knight_pieces, game->num_knight_pieces, piece);
		game->knight_pieces[index] = NULL;
		break;
	case Pawn:
		index = find_pointer((void **)game->pawn_pieces, game->num_pawn_pieces, piece);
		game->pawn_pieces[index] = NULL;
		break;
	case Empty:
		break;
	default:
		abort(); // exit_game(game, INVALID_PIECE_TYPE);
		break;
	}

	return piece;
}

struct Piece *add_to_piece_array(struct Game *game, struct Piece *piece)
{
	switch (piece->type)
	{
	case King:
		abort(); // exit_game(game, INVALID_MOVE);
		break;
	case Queen:
		if (piece->color == White)
		{
			game->white_queen = piece;
		}
		else
		{
			game->black_queen = piece;
		}
		break;
	case Rook:
		push_piece_array(game->rook_pieces, &game->num_rook_pieces, piece);
		break;
	case Bishop:
		push_piece_array(game->bishop_pieces, &game->num_bishop_pieces, piece);
		break;
	case Knight:
		push_piece_array(game->knight_pieces, &game->num_knight_pieces, piece);
		break;
	case Pawn:
		push_piece_array(game->pawn_pieces, &game->num_pawn_pieces, piece);
		break;
	case Empty:
		break;
	default:
		abort(); // exit_game(game, INVALID_PIECE_TYPE);
		break;
	}

	return piece;
}

void multiple_moves_game(struct Game *game, int num_moves, ...)
{
	va_list valist;

	/* initialize valist for num number of arguments */
	va_start(valist, num_moves);

	/* access all the arguments assigned to valist */
	for (int i = 0; i < num_moves; i++)
	{
		struct Move *move = malloc(sizeof(struct Move));
		char *move_str = va_arg(valist, char *);

		str_to_move(move, move_str);
		move_game(game, move);
	}

	/* clean memory reserved for valist */
	va_end(valist);
}

void move_game(struct Game *game, struct Move *move)
{
	move->removed_piece = remove_from_piece_array(game, game->board[move->target_rank][move->target_file]);

	game->board[move->target_rank][move->target_file] = game->board[move->start_rank][move->start_file];
	game->board[move->target_rank][move->target_file]->rank = move->target_rank;
	game->board[move->target_rank][move->target_file]->file = move->target_file;

	game->board[move->target_rank][move->target_file]->prev_moved = game->board[move->target_rank][move->target_file]->moved;
	game->board[move->target_rank][move->target_file]->moved = true;

	struct Piece *new_piece = malloc(sizeof(struct Piece));
	new_piece->file = move->start_file;
	new_piece->rank = move->start_rank;
	new_piece->type = Empty;
	new_piece->moved = false;
	new_piece->color = White;

	game->board[move->start_rank][move->start_file] = new_piece;

	game->active_player_color = game->active_player_color == White ? Black : White;

	push_move_array(game->moves_history, game->moves_history_size, move);
}

void unmove_game(struct Game *game)
{
	struct Move **move = malloc(sizeof(struct Move *));
	pop_move_array(game->moves_history, game->moves_history_size, move);
	if (move == NULL)
	{
		abort(); // exit_game(game, MEMORY_FAILURE);
	}

	free(game->board[(*move)->start_rank][(*move)->start_file]);
	game->board[(*move)->start_rank][(*move)->start_file] = game->board[(*move)->target_rank][(*move)->target_file];
	game->board[(*move)->start_rank][(*move)->start_file]->moved = game->board[(*move)->start_rank][(*move)->start_file]->prev_moved;
	game->board[(*move)->start_rank][(*move)->start_file]->file = (*move)->start_file;
	game->board[(*move)->start_rank][(*move)->start_file]->rank = (*move)->start_rank;

	add_to_piece_array(game, (*move)->removed_piece);
	game->board[(*move)->target_rank][(*move)->target_file] = (*move)->removed_piece;

	free(*move);
	free(move);

	game->active_player_color = game->active_player_color == White ? Black : White;
}

int game_perft_bulk(struct Game *game, int depth)
{
	if (depth == 0)
		return 1;

	// calculate moves
	struct Move ***moves = malloc(sizeof(struct Move **));
	int *moves_size = malloc(sizeof(int));
	generate_moves(game, moves, moves_size);

	if (depth == 1)
		return *moves_size;

	int numOfPositions = 0;

	for (int i = 0; i < *moves_size; i++)
	{
		move_game(game, (*moves)[i]);
		numOfPositions += game_perft_bulk(game, depth - 1);
		unmove_game(game);
	}

	return numOfPositions;
}

int game_perft_divide(struct Game *game, int depth)
{
	printf("Perft Divide (depth = %d)\n", depth);
	printf("========== Moves ==========\n");

	int num_of_positions = 0;

	struct Move ***moves = malloc(sizeof(struct Move **));
	int *moves_size = malloc(sizeof(int));
	generate_moves(game, moves, moves_size);

	char *move_str = malloc(sizeof(char) * 5);

	for (int i = 0; i < *moves_size; i++)
	{
		struct Move *move = (*moves)[i];
		move_to_str(move, move_str);

		move_game(game, move);
		int result = game_perft_bulk(game, depth - 1);
		unmove_game(game);

		printf("%s: %d\n", move_str, result);
		num_of_positions += result;
	}

	printf("=================================\n");
	printf("Total amount of moves: %d\n", num_of_positions);

	return num_of_positions;
}

void exit_game(struct Game *game, int status)
{
	free_game(game);
	printf("Exited with Status Code: %d", status);
	abort();
}

void free_game(struct Game *game)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (game->board[i][j] != NULL)
				free(game->board[i][j]);
		}
	}

	if (game->last_enpassant != NULL)
		free(game->last_enpassant);

	if (game->moves_history != NULL)
	{
		int size = *game->moves_history_size;
		for (int i = 0; i < size; i++)
		{
			if ((*game->moves_history)[i] != NULL)
			{
				if ((*game->moves_history)[i]->removed_piece != NULL)
				{
					free((*game->moves_history)[i]->removed_piece);
				}
				free((*game->moves_history)[i]);
			}
		}
		free(*game->moves_history);
	}

	if (game->moves_history_size != NULL)
		free(game->moves_history_size);

	free(game);
}

void print_game_board(struct Game *game)
{
	printf("    a   b   c   d   e   f   g   h\n  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n");
	for (int i = 0; i < 8; i++)
	{
		printf("%d │", 8 - i);
		for (int j = 0; j < 8; j++)
		{
			char piece = '*';
			if (game->board[i][j] != NULL)
			{
				piece = piece_to_char(game->board[i][j]);
			}
			printf(" %c │", piece);
		}

		printf(" %d\n", 8 - i);

		if (i != 7)
		{
			printf("  ├───┼───┼───┼───┼───┼───┼───┼───┤\n");
		}
		else
		{
			printf("  └───┴───┴───┴───┴───┴───┴───┴───┘\n");
		}
	}
	printf("    a   b   c   d   e   f   g   h\n\n");
}

void print_game_move_history(struct Game *game)
{
	printf("Moves History: ");
	int size = *game->moves_history_size;
	for (int j = 0; j < size; j++)
	{
		if ((*game->moves_history)[j] == NULL)
			break;

		char *move_str = malloc(sizeof(char) * 5);
		move_to_str((*game->moves_history)[j], move_str);

		printf("%s ", move_str);
		free(move_str);
	}
	printf("\n\n");
}

void game_export(struct Game *game)
{
	print_game_board(game);
	print_game_move_history(game);
}
