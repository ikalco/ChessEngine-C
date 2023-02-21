#include "include/game.h"

int main()
{
	struct Game *game = malloc(sizeof(struct Game));
	char *fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	int depth = 6;

	load_fen_string(fen_string, game);

	int *emarray_size = malloc(sizeof(int));
	*emarray_size = 2;
	struct ExpectedMove ***emarray = malloc(sizeof(struct ExpectedMove **));
	*emarray = calloc(*emarray_size, sizeof(struct ExpectedMove *));

	int expected_result;
	get_stockfish_perft(fen_string, depth, &expected_result, emarray, emarray_size);

	int result = game_perft_divide_debug(game, depth, emarray, emarray_size);

	if (result != expected_result)
	{
		printf("Something went wrong!");
		abort();
	}

	free_game(game);

	return EXIT_SUCCESS;
}
