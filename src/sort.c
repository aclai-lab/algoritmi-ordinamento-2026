#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "benchmark.h"

#define HYBRID_MS_K 600

void _insertion_sort(int64_t* data, uint32_t start, uint32_t end) {
	int32_t i;
	uint32_t j;
	int64_t key;

	for (j = start + 1; j <= end; j++) {
		i = j - 1;
		key = data[j];

		while (i >= (int32_t)start && data[i] > key) {
			data[i + 1] = data[i];
			i--;
		}

		data[i + 1] = key;
	}
}

void insertion_sort(struct benchmark_input* binput) {
	int64_t* data = (int64_t*)binput->data;
	uint32_t size = binput->size;

	_insertion_sort(data, 0, size - 1);
}

void merge(int64_t* data, uint32_t start, uint32_t mid, uint32_t end) {
	int n1 = mid - start + 1;
	int n2 = end - mid;

	int i, j, left[n1], right[n2];

	for (i = 0; i < n1; i++) {
		left[i] = data[start + i];
	}

	for (j = 0; j < n2; j++) {
		right[j] = data[mid + 1 + j];
	}

	i = j = 0;

	for (uint32_t k = start; k <= end; k++) {
		if (i < n1) {
			if (j < n2) {
				data[k] = (left[i] <= right[j]) ? left[i++] : right[j++];
			} else {
				data[k] = left[i++];
			}
		} else {
			data[k] = right[j++];
		}
	}
}

void _merge_sort(int64_t* data, uint32_t start, uint32_t end) {
	if (start < end) {
		uint32_t mid = start + (end - start) / 2;
		_merge_sort(data, start, mid);
		_merge_sort(data, mid + 1, end);
		merge(data, start, mid, end);
	}
}

void merge_sort(struct benchmark_input* binput) {
	int64_t* data = (int64_t*)binput->data;
	uint32_t size = binput->size;

	_merge_sort(data, 0, size - 1);
}

void _hybrid_merge_sort(int64_t* data, uint32_t start, uint32_t end) {
	if (end - start > HYBRID_MS_K) {
		uint32_t mid = start + (end - start) / 2;
		_hybrid_merge_sort(data, start, mid);
		_hybrid_merge_sort(data, mid + 1, end);
		merge(data, start, mid, end);
	} else {
		_insertion_sort(data, start, end);
	}
}

void hybrid_merge_sort(struct benchmark_input* binput) {
	int64_t* data = (int64_t*)binput->data;
	uint32_t size = binput->size;

	_hybrid_merge_sort(data, 0, size - 1);
}

void swap(int64_t* arr, uint32_t i, uint32_t j) {
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

uint32_t partition(int64_t* data, uint32_t start, uint32_t end) {
	int64_t pivot = data[end];
	int32_t i = start - 1;

	for (uint32_t j = start; j < end; ++j) {
        if (data[j] <= pivot) {
            swap(data, ++i, j);
        }
	}

    swap(data, ++i, end);
    return i;
}

void _quick_sort(int64_t* data, uint32_t start, uint32_t end) {
	if (start < end) {
		uint32_t mid = partition(data, start, end);
		if (mid > 0) {
			_quick_sort(data, start, mid - 1);
		}
		if (mid < end - 1) {
			_quick_sort(data, mid + 1, end);
		}
	}
}

void quick_sort(struct benchmark_input* binput) {
	int64_t* data = (int64_t*)binput->data;
	uint32_t size = binput->size;

	_quick_sort(data, 0, size - 1);
}

void generate_array(struct benchmark_input* binput, uint32_t size) {
	binput->size = size;
	binput->data = malloc(size * sizeof(int64_t));

	int64_t* _data = (int64_t*)binput->data;

	for (uint64_t i = 0; i < size; i++) {
		_data[i] = rand();
	}
}

algorithm_ptr select_sorting_algorithm(char* algo_name) {
	if (strcmp(algo_name, "INSERTION") == 0) {
		return &insertion_sort;
	} else if (strcmp(algo_name, "MERGE") == 0) {
		return &merge_sort;
	} else if (strcmp(algo_name, "HMERGE") == 0) {
		return &hybrid_merge_sort;
	} else if (strcmp(algo_name, "QUICK") == 0) {
		return &quick_sort;
	} else {
		printf("The provided algorithm (%s) is not available.", algo_name);
		exit(-1);
	}
}

int test_sort(struct benchmark_input* binput) {
	int64_t* _data = (int64_t*)binput->data;

	for (uint64_t i = 1; i < binput->size; i++) {
		if (_data[i] < _data[i - 1]) {
			return 0;
		}
	}

	return 1;
}
