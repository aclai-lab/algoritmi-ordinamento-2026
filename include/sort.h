#ifndef SORT_H
#define SORT_H

#include "benchmark.h"

void generate_array(struct benchmark_input* binput, uint32_t size);

algorithm_ptr select_sorting_algorithm(char* algo_name);

int test_sort(struct benchmark_input* binput);

#endif
