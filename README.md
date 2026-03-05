# algoritmi-ordinamento-2026
Laboratorio di programmazione del corso di Algoritmi e Strutture Dati dell'Università degli Studi di Ferrara, anno scolastico 2025-2026.

# Documentazione

La vista ad alto livello del progetto, si può riassumere con questo schema.

```mermaid
flowchart TB

    MAIN["main"]

    subgraph Configurazione
        CONFIG["config"]
        TOML["tomlc17"]
        STRUCT["struct configuration"]
    end

    subgraph Esperimenti
        BENCH["benchmark"]
    end

    subgraph Logica Esterna
        SORT["sort"]
    end

    MAIN -->|carica| CONFIG
    CONFIG -->|leggi TOML| TOML
    CONFIG -->|popola| STRUCT

    MAIN -->|esegui| BENCH
    BENCH -->|usa parametri| STRUCT

    BENCH -->|genera input| SORT
    BENCH -->|esegui algoritmo| SORT
    BENCH -->|testa| SORT
```

In particolare, il modulo `Configurazione` ha due responsabilità: definire la struttura di configurazione e caricarla.Un possibile esempio è di seguito.

```mermaid
classDiagram

class configuration {
    char name[NAME_BUF]

    char algorithms[ALGO_BUF][NAME_BUF]
    uint8_t algorithm_count

    uint8_t step

    uint32_t repetitions

    uint64_t min_size
    uint64_t max_size

    uint64_t seed
}
```

Il `main` è il punto di accesso del programma, e orchestratore (driver) degli esperimenti.

Dopo aver letto da tastiera il percorso file da cui leggere la configurazione, ne delega il caricamento al modulo `Configurazione`, ottenendo una struttura (vedi sopra) da inoltrare al modulo degli `Esperimenti`.

Gli esperimenti si occupano di:
    1. iterare una lista di algoritmi di cui misurare le prestazioni;
    2. per ogni algoritmo, generare un input casuale;
    3. chiamare l'algoritmo misurandone il tempo di esecuzione;
    4. salvare il tempo misurato;
    5. controllare la correttezza dell'algoritmo;
    6. ripetere il punto 2 per quanto specificato dalla configurazione;
    7. restituire la media dei tempi di esecuzione.

Si noti che i punti 1., 2. e 5. del processo sopra, non conoscono a priori la logica da chiamare.

Si aggiunge dunque una responsabilità al modulo degli `Esperimenti`: quella di esporre tre interfacce.

```mermaid
graph TD
    Benchmark[Benchmark]

    Generatore[Generatore input]
    SelezioneAlgoritmo[Seleziona algoritmo]
    Test[Strategia Test]

    Benchmark -->|espone interfaccia| Generatore
    Benchmark -->|espone interfaccia| SelezioneAlgoritmo
    Benchmark -->|espone interfaccia| Test
```

Un modulo `Logica Esterna` si occupa esattamente di implementare queste interfacce. Nel corso, ci focalizziamo sul contesto degli ordinamenti.

```mermaid
graph BT
    Generatore[Generatore input]
    SelezioneAlgoritmo[Seleziona algoritmo]
    Test[Strategia Test]

    Sort[Ordinamento]

    Sort -->|implementa interfaccia| Generatore
    Sort -->|implementa interfaccia| SelezioneAlgoritmo
    Sort -->|implementa interfaccia| Test
```
