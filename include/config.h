#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define NAME_BUF 64
#define ALGO_BUF 16

struct configuration {
	char name[NAME_BUF];

	char algorithms[ALGO_BUF][NAME_BUF];
	uint8_t algorithm_count;

	uint8_t step;

	uint32_t repetitions;

	uint64_t min_size;
	uint64_t max_size;

	uint64_t seed;
};

void config_init(struct configuration* config);

int config_load(char* config_filename, struct configuration* config);

#endif
