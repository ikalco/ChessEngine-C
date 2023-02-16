#include "include/game.h"

int main()
{
	perft_test_stockfish("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6, 119060324);

	// run_perft_tests();

	return EXIT_SUCCESS;
}
