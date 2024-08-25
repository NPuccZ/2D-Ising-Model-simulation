#!/bin/bash

# Directory contenente i file di output
OUTPUT_DIR="."

# Nome del file di output consolidato
RESULTS_FILE="results.txt"

# Rimuovi il file di output consolidato se esiste già
rm -f $RESULTS_FILE

# Aggiungi solo la prima intestazione e i dati dai file
# Assumendo che la prima intestazione venga dalla prima riga del primo file
first_file=$(ls ${OUTPUT_DIR}/thermodynamic_results_T*_B*.txt | head -n 1)

if [ -n "$first_file" ]; then
    # Aggiungi l'intestazione solo una volta
    head -n 1 "$first_file" >> $RESULTS_FILE
    
    # Aggiungi i dati da tutti i file, escludendo le intestazioni
    for file in ${OUTPUT_DIR}/thermodynamic_results_T*_B*.txt; do
        tail -n +2 "$file" >> $RESULTS_FILE
    done

    echo "Tutti i file sono stati uniti in $RESULTS_FILE"
else
    echo "Nessun file di output trovato."
fi

