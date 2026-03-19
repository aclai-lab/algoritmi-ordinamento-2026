#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>

#include "config.h"

/* Questa è la rappresentazione generica con cui lavora `benchmark`.
 * Viene popolata da un generatore di input (vedi `generate_array` in `sort.h`).
 */
struct benchmark_input {
	void* data;
	uint64_t size;
};

/* Riferimento a funzione che popola una `struct benchmark_input` con dimensione
 * un certo `uint32_t`.
 */
typedef void (*input_generator_ptr)(struct benchmark_input*, uint32_t);

/* Riferimento ad un algoritmo che manipola una `struct benchmark_input`,
 * ad esempio ordinandone il contenuto (il campo `data`).
 */
typedef void (*algorithm_ptr)(struct benchmark_input*);

/* Riferimento a funzione che, da una descrizione in caratteri, restituisce
 * un `algorithm_ptr`.
 */
typedef algorithm_ptr (*algorithm_selector_ptr)(char*);

/* Riferimento a una funzione per testare la validità di alcune proprietà
 * di una `struct benchmark_input`. */
typedef int (*test_ptr)(struct benchmark_input*);

/* Esegue la pipeline degli esperimenti;
 * per ogni algoritmo specificato in `config`, si misura il tempo di esecuzione
 * medio su più ripetizioni per una certa dimensione dell'input,
 * incrementalmente da `config.min_size` fino a `config.max_size`.
 */
void benchmark(struct configuration* config,
			   input_generator_ptr input_generator,
			   algorithm_selector_ptr algorithm_selector,
			   test_ptr test_strategy);

#endif
