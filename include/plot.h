#ifndef plot_h
#define plot_h

#include "config.h"

/* Passa i risultati in `results` (righe->algoritmo, colonne->dimensione,
 * intersezione->tempo di esecuzione) allo strumento `gnuplot`, se possibile.
 */
int32_t plot(struct configuration* config, double** results);

#endif
