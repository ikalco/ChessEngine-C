#pragma once

#include "include.h"

struct Game;

#include "move.h"
#include "pieces.h"

#include "perft.h"

struct Game
{
	struct Piece *board[8][8];
	struct Move ***moves_history;
	int *moves_history_size;

	struct Piece *white_king;
	struct Piece *black_king;

	struct Piece *white_queen;
	struct Piece *black_queen;

	struct Piece *rook_pieces[4];
	int num_rook_pieces;

	struct Piece *bishop_pieces[4];
	int num_bishop_pieces;

	struct Piece *knight_pieces[4];
	int num_knight_pieces;

	struct Piece *pawn_pieces[16];
	int num_pawn_pieces;

	struct Move *last_enpassant;

	enum Color active_player_color;

	bool black_can_castle_queenside;
	bool black_can_castle_kingside;
	bool white_can_castle_queenside;
	bool white_can_castle_kingside;

	unsigned int halfmove_clock;
	unsigned int fullmove_clock;
};

void multiple_moves_game(struct Game *game, int num_moves, ...);
void move_game(struct Game *game, struct Move *move);
void unmove_game(struct Game *game);

char *split_string_get_num_substring(char *string, const char *delimiter, char **results, int results_length);
void load_fen_string(char *fen_string, struct Game *game);

void game_export(struct Game *game);
void exit_game(struct Game *game, int status);
void free_game(struct Game *game);

int game_perft_bulk(struct Game *game, int depth);
int game_perft_divide(struct Game *game, int depth);
