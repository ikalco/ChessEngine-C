#include "include/game.h"

void push_piece_array(struct Piece *array[], int *array_size, struct Piece *piece)
{
	int size = *array_size;

	if (size == 0)
		abort();

	for (int j = 0; j < size; j++)
	{
		if (array[j] == NULL)
		{
			array[j] = piece;
			return;
		}
		else if (j == size - 1)
		{
			abort();
		}
	}

	abort(); // exit_game(game, MEMORY_FAILURE);
}

char piece_to_char(struct Piece *piece)
{
	char piece_char;

	switch (piece->type)
	{
	case King:
		piece_char = 'k';
		break;
	case Queen:
		piece_char = 'q';
		break;
	case Rook:
		piece_char = 'r';
		break;
	case Bishop:
		piece_char = 'b';
		break;
	case Knight:
		piece_char = 'n';
		break;
	case Pawn:
		piece_char = 'p';
		break;
	case Empty:
		piece_char = ' ';
		break;
	default:
		abort(); // exit_game(game, PIECE_DOES_NOT_EXIST);
		break;
	}

	if (piece->color == White)
	{
		return toupper(piece_char);
	}
	else
	{
		return piece_char;
	}
}

void set_piece_type_from_fen_letter(const char letter, struct Piece *piece)
{
	switch (tolower(letter))
	{
	case 'k':
		piece->type = King;
		break;
	case 'q':
		piece->type = Queen;
		break;
	case 'r':
		piece->type = Rook;
		break;
	case 'b':
		piece->type = Bishop;
		break;
	case 'n':
		piece->type = Knight;
		break;
	case 'p':
		piece->type = Pawn;
		break;
	default:
		abort(); // exit_game(game, PIECE_DOES_NOT_EXIST);
		break;
	}
}

void set_piece_color_from_fen_letter(const char letter, struct Piece *piece)
{
	switch (tolower(letter))
	{
	case 'k':
		break;
	case 'q':
		break;
	case 'r':
		break;
	case 'b':
		break;
	case 'n':
		break;
	case 'p':
		break;
	default:
		abort(); // exit_game(game, INVALID_PIECE_CHAR);
		break;
	}

	if (tolower(letter) == letter)
	{
		piece->color = Black;
	}
	else
	{
		piece->color = White;
	}
}

void set_piece_in_piece_array(struct Game *game, struct Piece *piece)
{
	if (piece == NULL)
		abort();

	switch (piece->type)
	{
	case King:
		if (piece->color == White)
		{
			game->white_king = piece;
		}
		else
		{
			game->black_king = piece;
		}
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
		game->rook_pieces[game->num_rook_pieces] = piece;
		game->num_rook_pieces++;
		break;
	case Bishop:
		game->bishop_pieces[game->num_bishop_pieces] = piece;
		game->num_bishop_pieces++;
		break;
	case Knight:
		game->knight_pieces[game->num_knight_pieces] = piece;
		game->num_knight_pieces++;
		break;
	case Pawn:
		game->pawn_pieces[game->num_pawn_pieces] = piece;
		game->num_pawn_pieces++;
		break;
	case Empty:
		break;
	}
}

void init_piece(struct Game *game, char current_piece_char, int *i_, int *j_)
{
	int i = *i_;
	int j = *j_;

	if (current_piece_char - '0' > 0 && current_piece_char - '0' <= 8)
	{
		int after_skip = j + (current_piece_char - '0');

		for (; j < after_skip; j++)
		{
			game->board[i][j] = malloc(sizeof(struct Piece));
			struct Piece *current_piece = game->board[i][j];

			current_piece->type = Empty;

			current_piece->rank = i;
			current_piece->file = j;
			current_piece->moved = false;
		}

		*j_ += current_piece_char - '0';
	}
	else
	{
		game->board[i][j] = malloc(sizeof(struct Piece));
		struct Piece *current_piece = game->board[i][j];

		set_piece_type_from_fen_letter(current_piece_char, current_piece);
		set_piece_color_from_fen_letter(current_piece_char, current_piece);

		set_piece_in_piece_array(game, current_piece);

		current_piece->rank = i;
		current_piece->file = j;
		current_piece->moved = false;
	}
}

void generate_king_moves(struct Game *game, struct Piece *king, struct Move ***king_moves, int *king_moves_size)
{
	int file_offsets[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
	int rank_offsets[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

	for (unsigned int i = 0; i < 8; i++)
	{
		int rank = king->rank + rank_offsets[i];
		int file = king->file + file_offsets[i];

		if (rank > 7 || file > 7 || rank < 0 || file < 0)
		{
			continue;
		}

		bool valid_move = false;

		if (game->board[rank][file]->type != Empty)
		{
			if (game->board[rank][file]->color != king->color)
			{
				valid_move = true;
			}
		}
		else
		{
			valid_move = true;
		}

		if (valid_move)
		{
			struct Move *king_move = add_move_array(king_moves, king_moves_size);

			king_move->start_file = king->file;
			king_move->start_rank = king->rank;
			king_move->target_file = file;
			king_move->target_rank = rank;
		}
	}
}

void generate_queen_moves(struct Game *game, struct Piece *queen, struct Move ***queen_moves, int *queen_moves_size)
{
	int file_offsets[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
	int rank_offsets[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

	for (int i = 0; i < 8; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			int rank = queen->rank + (rank_offsets[i] * j);
			int file = queen->file + (file_offsets[i] * j);

			if (rank > 7 || file > 7 || rank < 0 || file < 0)
			{
				break;
			}

			if (game->board[rank][file]->type == Empty)
			{
				struct Move *queen_move = add_move_array(queen_moves, queen_moves_size);

				queen_move->start_file = queen->file;
				queen_move->start_rank = queen->rank;
				queen_move->target_file = file;
				queen_move->target_rank = rank;

				continue;
			}
			else if (game->board[rank][file]->color != queen->color)
			{
				struct Move *queen_move = add_move_array(queen_moves, queen_moves_size);

				queen_move->start_file = queen->file;
				queen_move->start_rank = queen->rank;
				queen_move->target_file = file;
				queen_move->target_rank = rank;
			}

			break;
		}
	}
}

void generate_rook_moves(struct Game *game, struct Piece *rook, struct Move ***rook_moves, int *rook_moves_size)
{
	int file_offsets[4] = {-1, 0, 1, 0};
	int rank_offsets[4] = {0, -1, 0, 1};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			int rank = rook->rank + (rank_offsets[i] * j);
			int file = rook->file + (file_offsets[i] * j);

			if (rank > 7 || file > 7 || rank < 0 || file < 0)
			{
				break;
			}

			if (game->board[rank][file]->type == Empty)
			{
				struct Move *rook_move = add_move_array(rook_moves, rook_moves_size);

				rook_move->start_file = rook->file;
				rook_move->start_rank = rook->rank;
				rook_move->target_file = file;
				rook_move->target_rank = rank;

				continue;
			}
			else if (game->board[rank][file]->color != rook->color)
			{
				struct Move *rook_move = add_move_array(rook_moves, rook_moves_size);

				rook_move->start_file = rook->file;
				rook_move->start_rank = rook->rank;
				rook_move->target_file = file;
				rook_move->target_rank = rank;
			}

			break;
		}
	}
}

void generate_bishop_moves(struct Game *game, struct Piece *bishop, struct Move ***bishop_moves, int *bishop_moves_size)
{
	int file_offsets[4] = {-1, 1, -1, 1};
	int rank_offsets[4] = {-1, -1, 1, 1};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			int rank = bishop->rank + (rank_offsets[i] * j);
			int file = bishop->file + (file_offsets[i] * j);

			if (rank > 7 || file > 7 || rank < 0 || file < 0)
			{
				break;
			}

			if (game->board[rank][file]->type == Empty)
			{
				struct Move *bishop_move = add_move_array(bishop_moves, bishop_moves_size);

				bishop_move->start_file = bishop->file;
				bishop_move->start_rank = bishop->rank;
				bishop_move->target_file = file;
				bishop_move->target_rank = rank;

				continue;
			}
			else if (game->board[rank][file]->color != bishop->color)
			{
				struct Move *bishop_move = add_move_array(bishop_moves, bishop_moves_size);

				bishop_move->start_file = bishop->file;
				bishop_move->start_rank = bishop->rank;
				bishop_move->target_file = file;
				bishop_move->target_rank = rank;
			}

			break;
		}
	}
}

void generate_knight_moves(struct Game *game, struct Piece *knight, struct Move ***knight_moves, int *knight_moves_size)
{
	int file_offsets[8] = {-1, -1, -2, -2, 1, 1, 2, 2};
	int rank_offsets[8] = {2, -2, 1, -1, 2, -2, 1, -1};

	for (unsigned int i = 0; i < 8; i++)
	{
		int rank = knight->rank + rank_offsets[i];
		int file = knight->file + file_offsets[i];

		if (rank > 7 || file > 7 || rank < 0 || file < 0)
		{
			continue;
		}
		// printf("%d\n", file);

		bool valid_move = false;

		if (game->board[rank][file]->type != Empty)
		{
			if (game->board[rank][file]->color != knight->color)
			{
				valid_move = true;
			}
		}
		else
		{
			valid_move = true;
		}

		if (valid_move)
		{
			struct Move *knight_move = add_move_array(knight_moves, knight_moves_size);

			knight_move->start_file = knight->file;
			knight_move->start_rank = knight->rank;
			knight_move->target_file = file;
			knight_move->target_rank = rank;
		}
	}
}

void generate_pawn_moves(struct Game *game, struct Piece *pawn, struct Move ***pawn_moves, int *pawn_moves_size)
{
	int rank_direction = pawn->color == White ? -1 : 1;

	int rank = pawn->rank + rank_direction;
	int file = pawn->file;

	if (game->board[rank][file]->type == Empty)
	{
		struct Move *pawn_move = add_move_array(pawn_moves, pawn_moves_size);

		pawn_move->start_file = pawn->file;
		pawn_move->start_rank = pawn->rank;
		pawn_move->target_file = file;
		pawn_move->target_rank = rank;

		if (game->board[rank + rank_direction][file]->type == Empty && pawn->moved == false)
		{
			if (pawn->color == White)
			{
				if (pawn->rank == 6)
				{
					struct Move *pawn_move = add_move_array(pawn_moves, pawn_moves_size);

					pawn_move->start_file = pawn->file;
					pawn_move->start_rank = pawn->rank;
					pawn_move->target_file = file;
					pawn_move->target_rank = rank + rank_direction;
				}
			}
			else if (pawn->rank == 1)
			{
				struct Move *pawn_move = add_move_array(pawn_moves, pawn_moves_size);

				pawn_move->start_file = pawn->file;
				pawn_move->start_rank = pawn->rank;
				pawn_move->target_file = file;
				pawn_move->target_rank = rank + rank_direction;
			}
		}
	}

	file = file - 1;

	if (file >= 0 && file <= 7 && game->board[rank][file]->type != Empty && game->board[rank][file]->color != pawn->color)
	{
		struct Move *pawn_move = add_move_array(pawn_moves, pawn_moves_size);

		pawn_move->start_file = pawn->file;
		pawn_move->start_rank = pawn->rank;
		pawn_move->target_file = file;
		pawn_move->target_rank = rank;
	}

	file = file + 2;

	if (file >= 0 && file <= 7 && game->board[rank][file]->type != Empty && game->board[rank][file]->color != pawn->color)
	{
		struct Move *pawn_move = add_move_array(pawn_moves, pawn_moves_size);

		pawn_move->start_file = pawn->file;
		pawn_move->start_rank = pawn->rank;
		pawn_move->target_file = file;
		pawn_move->target_rank = rank;
	}

	file = file - 1;

	// if last enpassant target is next to pawn, then add move
	if (game->last_enpassant == NULL)
		return;

	if (pawn->rank == 3 || pawn->rank == 4)
	{
		if (abs(pawn->file - game->last_enpassant->target_file) == 1)
		{
			struct Move *pawn_move = add_move_array(pawn_moves, pawn_moves_size);

			pawn_move->start_file = pawn->file;
			pawn_move->start_rank = pawn->rank;
			pawn_move->target_file = game->last_enpassant->start_file;

			if (game->last_enpassant->target_rank == 3)
			{
				pawn_move->target_rank = 2;
			}
			else
			{
				pawn_move->target_rank = 5;
			}
		}
	}
}

void generate_all_king_moves(struct Game *game, struct Move ***all_king_moves, int *all_king_moves_size)
{
	*all_king_moves = calloc(2, sizeof(struct Move *));
	*all_king_moves_size = 2;

	if (game->white_king == NULL)
		abort();
	if (game->black_king == NULL)
		abort();

	generate_king_moves(game, game->white_king, all_king_moves, all_king_moves_size);
	generate_king_moves(game, game->black_king, all_king_moves, all_king_moves_size);
}

void generate_all_queen_moves(struct Game *game, struct Move ***all_queen_moves, int *all_queen_moves_size)
{
	*all_queen_moves = calloc(2, sizeof(struct Move *));
	*all_queen_moves_size = 2;

	if (game->white_queen != NULL)
		generate_queen_moves(game, game->white_queen, all_queen_moves, all_queen_moves_size);
	if (game->black_queen != NULL)
		generate_queen_moves(game, game->black_queen, all_queen_moves, all_queen_moves_size);
}

void generate_all_rook_moves(struct Game *game, struct Move ***all_rook_moves, int *all_rook_moves_size)
{
	struct Piece **rooks = game->rook_pieces;
	int num_rooks = game->num_rook_pieces;

	*all_rook_moves = calloc(num_rooks, sizeof(struct Move *));
	*all_rook_moves_size = num_rooks;

	for (int i = 0; i < num_rooks; i++)
	{
		if (rooks[i] == NULL)
			continue;
		generate_rook_moves(game, rooks[i], all_rook_moves, all_rook_moves_size);
	}
}

void generate_all_bishop_moves(struct Game *game, struct Move ***all_bishop_moves, int *all_bishop_moves_size)
{
	struct Piece **bishops = game->bishop_pieces;
	int num_bishops = game->num_bishop_pieces;

	*all_bishop_moves = calloc(num_bishops, sizeof(struct Move *));
	*all_bishop_moves_size = num_bishops;

	for (int i = 0; i < num_bishops; i++)
	{
		if (bishops[i] == NULL)
			continue;
		generate_bishop_moves(game, bishops[i], all_bishop_moves, all_bishop_moves_size);
	}
}

void generate_all_knight_moves(struct Game *game, struct Move ***all_knight_moves, int *all_knight_moves_size)
{
	struct Piece **knights = game->knight_pieces;
	int num_knights = game->num_knight_pieces;

	*all_knight_moves = calloc(num_knights, sizeof(struct Move *));
	*all_knight_moves_size = num_knights;

	for (int i = 0; i < num_knights; i++)
	{
		if (knights[i] == NULL)
			continue;
		generate_knight_moves(game, knights[i], all_knight_moves, all_knight_moves_size);
	}
}

void generate_all_pawn_moves(struct Game *game, struct Move ***all_pawn_moves, int *all_pawn_moves_size)
{
	struct Piece **pawns = game->pawn_pieces;
	int num_pawns = game->num_pawn_pieces;

	*all_pawn_moves = calloc(num_pawns, sizeof(struct Move *));
	*all_pawn_moves_size = num_pawns;

	for (int i = 0; i < num_pawns; i++)
	{
		if (pawns[i] == NULL)
			continue;
		generate_pawn_moves(game, pawns[i], all_pawn_moves, all_pawn_moves_size);
	}
}
