#!/bin/bash

echo "========================================"
echo "  Tests de performance - Projet multithread"
echo "========================================"
echo ""

# On teste avec 2, 4, 8, 16 et 32 threads au total
THREAD_COUNTS="2 4 8 16 32"
# On fait 5 mesures pour chaque config (comme demandé dans l'énoncé)
NB_MESURES=5

# ----------------------------------------------
# Test 1: Philosophes
# ----------------------------------------------
if [ -f "../philosophes" ]; then
    echo "--- Test philosophes ---"
    EXEC_FILE="../philosophes"
    OUTPUT_FILE="../results_philosophes.csv"

    # Créer l'en-tête du CSV
    echo "N_Threads,Mesure,Temps_Execution_s" > $OUTPUT_FILE
    
    for N in $THREAD_COUNTS; {
        echo "  Test avec $N philosophes"
        for ((i=1; i<=NB_MESURES; i++)); {
            echo -n "    Mesure $i/$NB_MESURES..."
            # 2>/dev/null pour pas polluer la sortie
            TIME=$($EXEC_FILE $N 2>/dev/null)
            
            if [ $? -eq 0 ]; then
                echo "$N,$i,$TIME" >> $OUTPUT_FILE
                echo " OK ($TIME s)"
            else
                echo "$N,$i,ERREUR" >> $OUTPUT_FILE
                echo " ERREUR"
            fi
        }
    }
    
    echo "  -> Résultats dans $OUTPUT_FILE"
    echo ""
else
    echo "Programme 'philosophes' pas trouvé, fais 'make' d'abord"
    echo ""
fi

# ----------------------------------------------
# Test 2: Lecteurs/Écrivains
# ----------------------------------------------
if [ -f "../lecteurs_ecrivains" ]; then
    echo "--- Test lecteurs/écrivains ---"
    EXEC_FILE="../lecteurs_ecrivains"
    OUTPUT_FILE="../results_lecteurs_ecrivains.csv"
    
    echo "N_Threads,N_Lecteurs,N_Ecrivains,Mesure,Temps_Execution_s" > $OUTPUT_FILE
    
    for N_TOTAL in $THREAD_COUNTS; {
        # On divise en deux: moitié lecteurs, moitié écrivains
        N_LECTEURS=$((N_TOTAL / 2))
        N_ECRIVAINS=$((N_TOTAL / 2))
        
        echo "  Test avec $N_TOTAL threads ($N_LECTEURS lecteurs + $N_ECRIVAINS écrivains)"
        for ((i=1; i<=NB_MESURES; i++)); {
            echo -n "    Mesure $i/$NB_MESURES..."
            TIME=$($EXEC_FILE $N_LECTEURS $N_ECRIVAINS 2>/dev/null)
            
            if [ $? -eq 0 ]; then
                echo "$N_TOTAL,$N_LECTEURS,$N_ECRIVAINS,$i,$TIME" >> $OUTPUT_FILE
                echo " OK ($TIME s)"
            else
                echo "$N_TOTAL,$N_LECTEURS,$N_ECRIVAINS,$i,ERREUR" >> $OUTPUT_FILE
                echo " ERREUR"
            fi
        }
    }
    
    echo "  -> Résultats dans $OUTPUT_FILE"
    echo ""
else
    echo "Programme 'lecteurs_ecrivains' pas trouvé, fais 'make' d'abord"
    echo ""
fi

# ----------------------------------------------
# Test 3: Producteurs/Consommateurs
# ----------------------------------------------
if [ -f "../prod_cons" ]; then
    echo "--- Test producteurs/consommateurs ---"
    EXEC_FILE="../prod_cons"
    OUTPUT_FILE="../results_prod_cons.csv"
    
    echo "N_Threads,N_Producteurs,N_Consommateurs,Mesure,Temps_Execution_s" > $OUTPUT_FILE
    
    for N_TOTAL in $THREAD_COUNTS; {
        # Pareil, on divise en deux
        N_PROD=$((N_TOTAL / 2))
        N_CONS=$((N_TOTAL / 2))
        
        echo "  Test avec $N_TOTAL threads ($N_PROD prod + $N_CONS conso)"
        for ((i=1; i<=NB_MESURES; i++)); {
            echo -n "    Mesure $i/$NB_MESURES..."
            TIME=$($EXEC_FILE $N_PROD $N_CONS 2>/dev/null)
            
            if [ $? -eq 0 ]; then
                echo "$N_TOTAL,$N_PROD,$N_CONS,$i,$TIME" >> $OUTPUT_FILE
                echo " OK ($TIME s)"
            else
                echo "$N_TOTAL,$N_PROD,$N_CONS,$i,ERREUR" >> $OUTPUT_FILE
                echo " ERREUR"
            fi
        }
    }
    
    echo "  -> Résultats dans $OUTPUT_FILE"
    echo ""
else
    echo "Programme 'prod_cons' pas encore codé"
    echo ""
fi

echo "========================================"
echo "  Tous les tests sont terminés !"
echo "========================================"