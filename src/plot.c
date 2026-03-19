#include <stdio.h>

#include "plot.h"

// popen must be renamed in windows
// https://learn.microsoft.com/it-it/search/?terms=popen
#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

int32_t plot(struct configuration* config, double** results) {
	FILE* gp = POPEN("gnuplot -persistent", "w");
	if (!gp) {
		fprintf(stderr, "Error: Could not open pipe to Gnuplot.\n");
		return 0;
	}

	fprintf(gp, "set title 'Benchmark results'\n");
	fprintf(gp, "set xlabel 'size'\n");
	fprintf(gp, "set ylabel 'time [s]'\n");
	fprintf(gp, "set grid\n");
	fprintf(gp, "set key left top\n");

	fprintf(gp, "plot ");
	for (uint8_t a = 0; a < config->algorithm_count; a++) {
		fprintf(gp, "'-' with linespoints title '%s'%s", config->algorithms[a],
				(a < config->algorithm_count - 1) ? ", " : "\n");
	}

	uint64_t num_steps =
		(config->max_size - config->min_size) / config->step + 1;

	for (uint8_t a = 0; a < config->algorithm_count; a++) {
		uint64_t current_size = config->min_size;
		for (uint64_t s = 0; s < num_steps; s++) {
			fprintf(gp, "%lu %g\n", current_size, results[a][s]);
			current_size += config->step;
		}
		fprintf(gp, "e\n");
	}

	PCLOSE(gp);

	return 1;
}

