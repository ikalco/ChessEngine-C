#pragma once

void run_perft_tests();
void get_stockfish_perft(char *fen_string, int depth, int *expected_result, struct ExpectedMove ***emarray, int *emarray_size);
