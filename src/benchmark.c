#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "benchmark.h"
#include "plot.h"

double run(uint64_t size, uint32_t repetitions,
		   input_generator_ptr input_generator, algorithm_ptr algorithm,
		   test_ptr test_strategy) {
	double elapsed_time = 0.0;

	for (uint32_t i = 0; i < repetitions; i++) {
		struct benchmark_input binput;
		input_generator(&binput, size);

		clock_t start, end;
		start = clock();
		algorithm(&binput);
		end = clock();

		if (test_strategy(&binput) != 1) {
			printf("ERRORE: test fallito.\n");
			exit(-1);
		}

		elapsed_time += (end - start) / (double)CLOCKS_PER_SEC;

		free(binput.data);
	}

	return elapsed_time / repetitions;
}

void benchmark(struct configuration* config,
			   input_generator_ptr input_generator,
			   algorithm_selector_ptr algorithm_selector,
			   test_ptr test_strategy) {
	uint32_t initial_seed = config->seed;

	uint64_t num_steps =
		(config->max_size - config->min_size) / config->step + 1;
	double** results = malloc(config->algorithm_count * sizeof(double*));
	for (uint8_t a = 0; a < config->algorithm_count; a++) {
		results[a] = malloc(num_steps * sizeof(double));
	}

	for (uint8_t algo_idx = 0; algo_idx < config->algorithm_count; algo_idx++) {
		uint32_t mutable_seed = initial_seed;
		uint64_t step_idx = 0;

		for (uint64_t i = config->min_size; i <= config->max_size;
			 i += config->step) {
			srand(mutable_seed);

			double elapsed_time =
				run(i, config->repetitions, input_generator,
					algorithm_selector(config->algorithms[algo_idx]),
					test_strategy);

			results[algo_idx][step_idx++] = elapsed_time;
			++mutable_seed;
		}
	}

	uint32_t code = plot(config, results);
	if (code == 0) {
		fprintf(stderr, "Plotting error");
	}

	for (uint8_t a = 0; a < config->algorithm_count; a++) {
		free(results[a]);
	}
	free(results);
}
