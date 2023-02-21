#include "include/game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fdopen(int fd, const char *mode);

#define PIPE_READ 0
#define PIPE_WRITE 1

bool numeric(char c)
{
	return (c >= '0' && c <= '9');
}

bool alpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool wait_exited_successfully(int pid, int *status)
{
	if (waitpid(pid, status, 0) > 0)
	{
		if (WIFEXITED(*status) && !WEXITSTATUS(*status))
		{
			return true;
			// printf("program execution successful\n");
		}

		else if (WIFEXITED(*status) && WEXITSTATUS(*status))
		{
			if (WEXITSTATUS(*status) == 127)
			{
				return false;
				// execv failed
				// printf("execv failed\n");
			}
			else
			{
				return true;
				// printf("program terminated normally, but returned a non-zero status\n");
			}
		}
		else
		{
			return false;
			// printf("program didn't terminate normally\n");
		}
	}
	else
	{
		// waitpid() failed
		return false;
	}
}

void run_perft_tests()
{
	char *perft_tests[] = {
		"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1;6=119060324",
		"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -;5=193690690",
		"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -;7=178633661",
		"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1;6=706045033",
		"1k6/1b6/8/8/7R/8/8/4K2R b K - 0 1;5=1063513",
		"3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1;6=1134888",
		"8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1;6=1015133",
		"8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1;6=1440467",
		"5k2/8/8/8/8/8/8/4K2R w K - 0 1;6=661072",
		"3k4/8/8/8/8/8/8/R3K3 w Q - 0 1;6=803711",
		"r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1;4=1274206",
		"r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1;4=1720476",
		"2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1;6=3821001",
		"8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1;5=1004658",
		"4k3/1P6/8/8/8/8/K7/8 w - - 0 1;6=217342",
		"8/P1k5/K7/8/8/8/8/8 w - - 0 1;6=92683",
		"K1k5/8/P7/8/8/8/8/8 w - - 0 1;6=2217",
		"8/k1P5/8/1K6/8/8/8/8 w - - 0 1;7=567584",
		"8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1;4=23527",
	};

	printf("Starting Perft Test Full Suite\n");
	bool results[19];

	clock_t first_start_time = clock();

	for (int i = 0; i < 19; i++)
	{
		printf("========== Test %d ==========\n", i);

		char *deconstructed_perft_string[2];
		char *split_str1 = split_string_get_num_substring(perft_tests[i], ";", deconstructed_perft_string, 2);

		char *deconstructed_perft_test_string[2];
		char *split_str2 = split_string_get_num_substring(deconstructed_perft_string[1], "=", deconstructed_perft_test_string, 2);

		int depth = (int)strtol(deconstructed_perft_test_string[0], NULL, 10);
		int expected_result = (int)strtol(deconstructed_perft_test_string[1], NULL, 10);

		// create game
		struct Game *game = malloc(sizeof(struct Game));

		// load fen string
		load_fen_string(deconstructed_perft_string[0], game);

		clock_t start_time = clock();
		int result = game_perft_divide(game, depth);
		clock_t elapsed_time = clock() - start_time;

		char right[4] = "✅";
		char wrong[4] = "❌";

		printf("Expected: %d | Got: %d\n", expected_result, result);
		printf("%s Test %d took: %fms\n", result == expected_result ? right : wrong, i, (elapsed_time / CLOCKS_PER_SEC) * 1000.0f);
		results[i] = result == expected_result ? true : false;

		free_game(game);
		free(split_str1);
		free(split_str2);
	}

	double result = 0;
	for (int i = 0; i < 19; i++)
	{
		result += results[i];
	}
	result /= 19;
	result *= 100;

	printf("%f%% Accuracy\n", result);

	clock_t total_elapsed_time = clock() - first_start_time;
	printf("Time to run full suite: %fms\n", (total_elapsed_time / CLOCKS_PER_SEC) * 1000.0f);
}

int parse_stockfish_perft(int pipe_read_fd, struct ExpectedMove ***emarray, int *emarray_size)
{
	FILE *pipe_read = fdopen(pipe_read_fd, "r");

	int expected_result = -1;

	char line[256];
	while (fgets(line, 256, pipe_read))
	{
		int length = (int)strcspn(line, "\n") + 1;
		if (length < 5)
			continue;

		// replace newline with string null terminate
		line[--length] = 0;

		if (line[0] == 'N' && line[14] == ':')
		{
			bool next = false;
			for (int i = 16; i < length; i++)
			{
				if (!numeric(line[i]))
				{
					next = true;
					break;
				}
			}

			if (next)
				abort();

			char *strs[2];
			char *split_str = split_string_get_num_substring(line, ":", strs, 2);

			expected_result = (int)strtol(strs[1], NULL, 10);

			free(split_str);

			continue;
		}

		if (line[4] != ':')
			continue;

		if (line[5] != ' ')
			continue;

		// if the first 4 chars are in the shape of a move ea. g1h3, g1f3
		if (alpha(line[0]) && numeric(line[1]) && alpha(line[2]) && numeric(line[3]))
		{
			// make sure the rest of the string is a number
			bool next = false;
			for (int i = 6; i < length; i++)
			{
				if (!numeric(line[i]))
				{
					next = true;
					break;
				}
			}

			if (next)
				continue;
		}

		struct ExpectedMove *expected_move = add_expected_move_array(emarray, emarray_size);

		char *strs[2];
		char *split_str = split_string_get_num_substring(line, ":", strs, 2);

		str_to_move(&expected_move->move, strs[0]);
		expected_move->expected_result = (int)strtol(strs[1], NULL, 10);

		free(split_str);
	}

	downsize_expected_move_array(emarray, emarray_size);

	fclose(pipe_read);

	return expected_result;
}

void send_stockfish_perft(char *fen_string, int depth, int pipe_write_fd)
{
	FILE *pipe_write = fdopen(pipe_write_fd, "w");
	fprintf(pipe_write, "position %s\r\ngo perft %d\r\nquit\r\n", fen_string, depth);
	fclose(pipe_write);
}

void get_stockfish_perft(char *fen_string, int depth, int *expected_result, struct ExpectedMove ***emarray, int *emarray_size)
{
	if (emarray == NULL || *emarray == NULL || emarray_size == NULL)
		abort();

	for (int i = 0; i < *emarray_size; i++)
	{
		if ((*emarray)[i] != NULL)
			free((*emarray)[i]);
		else
			break;

		(*emarray)[i] = NULL;
	}

	*emarray_size = 2;
	*emarray = realloc(*emarray, sizeof(struct ExpectedMove *) * *emarray_size);

	int fd[2];
	if (pipe(fd) == -1)
	{
		abort();
	}

	int status;
	pid_t pid = fork();

	if (pid == -1)
	{
		abort();
	}
	else if (pid == 0) // child
	{
		dup2(fd[PIPE_WRITE], STDOUT_FILENO);
		dup2(fd[PIPE_READ], STDIN_FILENO);
		close(fd[PIPE_READ]);
		close(fd[PIPE_WRITE]);

		char *const args[] = {"./stockfish", NULL};
		execv(args[0], args);
	}
	else // parent
	{
		send_stockfish_perft(fen_string, depth, fd[PIPE_WRITE]);

		if (!wait_exited_successfully(pid, &status))
		{
			abort();
		}

		*expected_result = parse_stockfish_perft(fd[PIPE_READ], emarray, emarray_size);
	}
}
