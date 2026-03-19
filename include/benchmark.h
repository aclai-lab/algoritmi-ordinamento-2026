#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>

#include "config.h"

struct benchmark_input {
	void* data;
	uint64_t size;
};

typedef void (*input_generator_ptr)(struct benchmark_input*, uint32_t);

typedef void (*algorithm_ptr)(struct benchmark_input*);
typedef algorithm_ptr (*algorithm_selector_ptr)(char*);

typedef int (*test_ptr)(struct benchmark_input*);

void benchmark(struct configuration* config,
			   input_generator_ptr input_generator,
			   algorithm_selector_ptr algorithm_selector,
			   test_ptr test_strategy);

#endif
