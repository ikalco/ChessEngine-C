#pragma once

enum Type;
enum Color;
struct Piece;

#include "game.h"

enum Type
{
	Empty,
	King,
	Queen,
	Rook,
	Bishop,
	Knight,
	Pawn
};

enum Color
{
	White,
	Black
};

struct Piece
{
	enum Type type;
	enum Color color;
	unsigned int file;
	unsigned int rank;

	unsigned char moved : 1;
	unsigned char prev_moved : 1;
};

void push_piece_array(struct Piece *array[], int *array_size, struct Piece *piece);

void init_piece(struct Game *game, char current_player_char, int *i, int *j);
char piece_to_char(struct Piece *piece);

void generate_all_king_moves(struct Game *game, struct Move ***king_moves, int *king_moves_size);
void generate_all_queen_moves(struct Game *game, struct Move ***queen_moves, int *queen_moves_size);
void generate_all_rook_moves(struct Game *game, struct Move ***rook_moves, int *rook_moves_size);
void generate_all_bishop_moves(struct Game *game, struct Move ***bishop_moves, int *bishop_moves_size);
void generate_all_knight_moves(struct Game *game, struct Move ***knight_moves, int *knight_moves_size);
void generate_all_pawn_moves(struct Game *game, struct Move ***pawn_moves, int *pawn_moves_size);
