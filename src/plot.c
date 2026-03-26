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

int32_t fallback_plot(struct configuration* config, double** results) {
	char filename[FILENAME_MAX];
	snprintf(filename, sizeof(filename), "results/%s.csv", config->name);

	FILE* out = fopen(filename, "w");
	if (!out) {
		perror("Error while creating a new file for the plot fallback logic\n");
	}

	fprintf(out, "size");
	for (uint8_t a = 0; a < config->algorithm_count; a++) {
		fprintf(out, ",%s", config->algorithms[a]);
	}
	fprintf(out, "\n");

	uint64_t num_steps =
		(config->max_size - config->min_size) / config->step + 1;
	for (uint64_t s = 0; s < num_steps; s++) {
		uint64_t current_size = config->min_size + s * config->step;
		fprintf(out, "%lu", current_size);

		for (uint8_t a = 0; a < config->algorithm_count; a++) {
			fprintf(out, ",%g", results[a][s]);
		}

		fprintf(out, "\n");
	}

	fclose(out);
	fprintf(stdout, "Fallback data saved to '%s'\n", filename);
	return 1;
}

int32_t plot(struct configuration* config, double** results) {
	FILE* gp = POPEN("gnuplot -persistent", "w");
	if (!gp) {
		fprintf(stdout, "Could not open pipe to Gnuplot. Is it installed?\n");
		fprintf(stdout, "Invoking fallback logic...\n");
		return fallback_plot(config, results);
	}

	fprintf(gp, "set terminal pngcairo enhanced font 'Verdana,10'\n");
	fprintf(gp, "set output 'results/%s.png'\n", config->name);

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
