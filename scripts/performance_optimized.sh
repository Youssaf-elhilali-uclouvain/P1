#!/bin/bash

# Script optimisé pour les mesures de performance
THREAD_COUNTS="2 4 8 16 32"
# On fait 5 mesures pour chaque config (comme demandé dans l'énoncé)
NB_MESURES=5

mkdir -p ../dataCSV >/dev/null 2>&1

# Test 1: Philosophes
if [ -f "../philosophes" ]; then
    EXEC_FILE="../philosophes"
    OUTPUT_FILE="../dataCSV/results_philosophes.csv"

    echo "N_Threads,Mesure,Temps_Execution_s" > $OUTPUT_FILE
    
    for N in $THREAD_COUNTS; {
        for ((i=1; i<=NB_MESURES; i++)); {
            TIME=$($EXEC_FILE $N 2>/dev/null)
            
            if [ $? -eq 0 ]; then
                echo "$N,$i,$TIME" >> $OUTPUT_FILE
            else
                echo "$N,$i,ERREUR" >> $OUTPUT_FILE
            fi
        }
    }
fi

# Test 2: Lecteurs/Écrivains

if [ -f "../lecteurs_ecrivains" ]; then
    EXEC_FILE="../lecteurs_ecrivains"
    OUTPUT_FILE="../dataCSV/results_lecteurs_ecrivains.csv"
    
    echo "N_Threads,N_Lecteurs,N_Ecrivains,Mesure,Temps_Execution_s" > $OUTPUT_FILE
    
    for N_TOTAL in $THREAD_COUNTS; {
        N_LECTEURS=$((N_TOTAL / 2))
        N_ECRIVAINS=$((N_TOTAL / 2))
        
        for ((i=1; i<=NB_MESURES; i++)); {
            TIME=$($EXEC_FILE $N_LECTEURS $N_ECRIVAINS 2>/dev/null)
            
            if [ $? -eq 0 ]; then
                echo "$N_TOTAL,$N_LECTEURS,$N_ECRIVAINS,$i,$TIME" >> $OUTPUT_FILE
            else
                echo "$N_TOTAL,$N_LECTEURS,$N_ECRIVAINS,$i,ERREUR" >> $OUTPUT_FILE
            fi
        }
    }
fi

# Test 3: Producteurs/Consommateurs

if [ -f "../prod_cons" ]; then
    EXEC_FILE="../prod_cons"
    OUTPUT_FILE="../dataCSV/results_prod_cons.csv"
    
    echo "N_Threads,N_Producteurs,N_Consommateurs,Mesure,Temps_Execution_s" > $OUTPUT_FILE
    
    for N_TOTAL in $THREAD_COUNTS; {
        N_PROD=$((N_TOTAL / 2))
        N_CONS=$((N_TOTAL / 2))
        
        for ((i=1; i<=NB_MESURES; i++)); {
            TIME=$($EXEC_FILE $N_PROD $N_CONS 2>/dev/null)
            
            if [ $? -eq 0 ]; then
                echo "$N_TOTAL,$N_PROD,$N_CONS,$i,$TIME" >> $OUTPUT_FILE
            else
                echo "$N_TOTAL,$N_PROD,$N_CONS,$i,ERREUR" >> $OUTPUT_FILE
            fi
        }
    }
fi

# Test 4: Spinlock Test-and-Set (TAS)
if [ -f "../test_spinlock" ]; then
    EXEC_FILE="../test_spinlock"
    OUTPUT_FILE="../dataCSV/results_spinlock.csv"

    echo "N_Threads,Mesure,Temps_Execution_s" > $OUTPUT_FILE

    for N in 1 2 4 8 16 32; do
        for ((i=1; i<=NB_MESURES; i++)); do
            RAW=$($EXEC_FILE $N 2>/dev/null)
            TIME=$(echo "$RAW" | awk -F',' '{print $2}' | xargs)
            echo "$N,$i,$TIME" >> $OUTPUT_FILE
        done
    done
fi

# Test 5: Spinlock Test-and-Test-and-Set (TTAS)
if [ -f "../test_ttas" ]; then
    EXEC_FILE="../test_ttas"
    OUTPUT_FILE="../dataCSV/results_ttas.csv"

    echo "N_Threads,Mesure,Temps_Execution_s" > $OUTPUT_FILE

    for N in 1 2 4 8 16 32; do
        for ((i=1; i<=NB_MESURES; i++)); do
            RAW=$($EXEC_FILE $N 2>/dev/null)
            TIME=$(echo "$RAW" | awk -F',' '{print $2}' | xargs)

            echo "$N,$i,$TIME" >> $OUTPUT_FILE
        done
    done
fi
