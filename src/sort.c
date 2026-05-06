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

uint32_t median_of_three(int64_t* data, uint32_t a, uint32_t b, uint32_t c) {
	int64_t data_a = data[a];
	int64_t data_b = data[b];
	int64_t data_c = data[c];

	if (data_a > data_b) {
		if (data_b > data_c)
			return b;
		else if (data_a < data_c)
			return a;
		else
			return c;
	} else {
		if (data_a > data_c)
			return a;
		else if (data_b < data_c)
			return b;
		else
			return c;
	}
}

uint32_t mot_partition(int64_t* data, uint32_t start, uint32_t end) {
	uint32_t median =
		median_of_three(data, start, end, start + (end - start) / 2);
	swap(data, median, end);

	return partition(data, start, end);
}

void _mot_quick_sort(int64_t* data, uint32_t start, uint32_t end) {
	if (start < end) {
		uint32_t mid = mot_partition(data, start, end);
		if (mid > 0) {
			_mot_quick_sort(data, start, mid - 1);
		}
		if (mid < end - 1) {
			_mot_quick_sort(data, mid + 1, end);
		}
	}
}

void mot_quick_sort(struct benchmark_input* binput) {
	int64_t* data = (int64_t*)binput->data;
	uint32_t size = binput->size;

	_mot_quick_sort(data, 0, size - 1);
}

void _tail_quick_sort(int64_t* data, uint32_t start, uint32_t end) {
	while (start < end) {
		uint32_t mid = mot_partition(data, start, end);
		if (mid > 0) {
			_tail_quick_sort(data, start, mid - 1);
		}
		start = mid + 1;
	}
}

void tail_quick_sort(struct benchmark_input* binput) {
	int64_t* data = (int64_t*)binput->data;
	uint32_t size = binput->size;

	_tail_quick_sort(data, 0, size - 1);
}

void _max_heapify(int64_t* data, uint32_t size, uint32_t root) {
	uint32_t largest = root;
	uint32_t left = 2 * root + 1;
	uint32_t right = 2 * root + 2;

	if (left < size && data[left] > data[largest]) {
		largest = left;
	}

	if (right < size && data[right] > data[largest]) {
		largest = right;
	}

	if (largest != root) {
		swap(data, root, largest);
		_max_heapify(data, size, largest);
	}
}

void _heap_sort(int64_t* data, uint32_t size) {
	// usiamo un iteratore signed per semplificare un attimo il codice,
	// ma i cicli for sotto si potrebbero riscrivere per supportare direttamente
	// un iteratore uint32_t;
    // esempio del primo ciclo: for (uint32_t i = (size / 2); i-- > 0; )
	int32_t i;

	// crea la max heap
	for (i = (size / 2) - 1; i >= 0; i--) {
		_max_heapify(data, size, i);
	}

	// ogni volta "estrai" l'elemento più grande dalla heap (la radice)
	// e posizionalo nell'ultima posizione libera (la i-esima) in data;
	for (i = size - 1; i > 0; i--) {
		// estrazione e posizionamento sono fatte contemporaneamente dallo swap
		swap(data, 0, i);
		// devo ripristinare la proprietà di max heap
		_max_heapify(data, i, 0);
	}
}

void heap_sort(struct benchmark_input* binput) {
	int64_t* data = (int64_t*)binput->data;
	uint32_t size = binput->size;

	_heap_sort(data, size);
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
	} else if (strcmp(algo_name, "MOTQUICK") == 0) {
		return &mot_quick_sort;
	} else if (strcmp(algo_name, "TAILQUICK") == 0) {
		return &tail_quick_sort;
	} else if (strcmp(algo_name, "HEAPSORT") == 0) {
		return &heap_sort;
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
