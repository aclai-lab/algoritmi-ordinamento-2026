#ifndef SORT_H
#define SORT_H

#include "benchmark.h"

/* Generatore di un generico input per il caso degli algoritmi di ordinamento;
 * vedi `input_generator_ptr` in `benchmark.h`.
 */
void generate_array(struct benchmark_input* binput, uint32_t size);

/* Ponte tra l'identificativo di un algoritmo (ad esempio ottenuto da una 
 * configurazione, o dall'input stdin dell'utente) e la sua logica effettiva;
 * vedi `algorithm_ptr` in `benchmark.h`.
 */
algorithm_ptr select_sorting_algorithm(char* algo_name);

/* Funzione di test per il caso degli algoritmi di ordinamento;
 * controlla che la lista codificata da `binput` sia ordinata.
 * Vedi `test_ptr` in `benchmark.h`.
 */
int test_sort(struct benchmark_input* binput);

#endif
