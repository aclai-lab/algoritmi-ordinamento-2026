#include <stdio.h>
#include <stdlib.h>

#include "benchmark.h"
#include "config.h"
#include "sort.h"

#define CONF_PATH_BUF 128

int main(int argc, char** argv) {
	char conf_path[CONF_PATH_BUF];

	// The configuration must be specified in the command line
	if (argc == 2) {
		snprintf(conf_path, sizeof(conf_path), "%s", argv[1]);
	} else {
		printf("Usage: ./name <configuration_filepath>");
		return EXIT_FAILURE;
	}

	struct configuration configuration;
	config_init(&configuration);
	config_load(conf_path, &configuration);

	benchmark(&configuration, generate_array, select_sorting_algorithm,
			  test_sort);

	return EXIT_SUCCESS;
}
