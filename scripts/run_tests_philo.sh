#!/bin/bash

EXEC_FILE="./philosophes"
OUTPUT_FILE="results_philosophes.csv"

# Liste des nombres de philosophes (N) à tester.
# Il est crucial de varier N par rapport au nombre de coeurs (16 sur la machine de test)
# N=1, 2, 4, 8, 16, 32 sont de bonnes valeurs de test.
N_VALUES="1 2 4 8 16 32"

# Entête du fichier CSV
echo "N_Philosophes,Temps_Execution_s" > $OUTPUT_FILE
echo "Démarrage de l'évaluation..."

# Boucle principale sur les valeurs de N
for N in $N_VALUES; do
    echo "--- Test N=$N Philosophes ---"
    
    # 1. Exécution du programme C
    # La sortie standard du programme C (le temps) est capturée dans la variable TIME
    TIME=$($EXEC_FILE $N)

    # 2. Vérification et enregistrement
    if [ $? -eq 0 ]; then
        # Enregistre N et le temps dans le fichier CSV
        echo "$N,$TIME" >> $OUTPUT_FILE
        echo "Temps mesuré : $TIME s"
    else
        echo "Erreur lors de l'exécution pour N=$N. Vérifiez le code C."
        echo "$N,ERREUR" >> $OUTPUT_FILE
    fi
done

echo "---"
echo "Évaluation terminée. Les résultats sont dans $OUTPUT_FILE"