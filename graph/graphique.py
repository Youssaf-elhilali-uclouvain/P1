#!/usr/bin/env python3

import matplotlib.pyplot as plt
import pandas as pd

# Lire les fichiers CSV
df_philo = pd.read_csv('../dataCSV/results_philosophes.csv')
df_rw = pd.read_csv('../dataCSV/results_lecteurs_ecrivains.csv')
df_pc = pd.read_csv('../dataCSV/results_prod_cons.csv')
df_spin = pd.read_csv('../dataCSV/results_spinlock.csv')
df_ttas = pd.read_csv('../dataCSV/results_ttas.csv')

# Calculer moyenne et écart-type pour chaque nombre de threads
stats_philo = df_philo.groupby('N_Threads')['Temps_Execution_s'].agg(['mean', 'std'])
stats_rw = df_rw.groupby('N_Threads')['Temps_Execution_s'].agg(['mean', 'std'])
stats_pc = df_pc.groupby('N_Threads')['Temps_Execution_s'].agg(['mean', 'std'])
stats_spin = df_spin.groupby('N_Threads')['Temps_Execution_s'].agg(['mean','std'])
stats_ttas = df_ttas.groupby('N_Threads')['Temps_Execution_s'].agg(['mean','std'])

# Graphique 1: Philosophes
plt.figure(figsize=(10, 6))
plt.errorbar(stats_philo.index, stats_philo['mean'], yerr=stats_philo['std'], 
             marker='o', capsize=5, capthick=2, label='Philosophes')
plt.xlabel('Nombre de threads')
plt.ylabel('Temps d\'exécution (s)')
plt.title('Performance du problème des philosophes')
plt.ylim(bottom=0)
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('graphique_philosophes.pdf')
plt.close()

# Graphique 2: Lecteurs/Écrivains
plt.figure(figsize=(10, 6))
plt.errorbar(stats_rw.index, stats_rw['mean'], yerr=stats_rw['std'], 
             marker='o', capsize=5, capthick=2, label='Lecteurs/Écrivains')
plt.xlabel('Nombre de threads')
plt.ylabel('Temps d\'exécution (s)')
plt.title('Performance du problème des lecteurs/écrivains')
plt.ylim(bottom=0)
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('graphique_lecteurs_ecrivains.pdf')
plt.close()

# Graphique 3: Producteurs/Consommateurs
plt.figure(figsize=(10, 6))
plt.errorbar(stats_pc.index, stats_pc['mean'], yerr=stats_pc['std'], 
             marker='o', capsize=5, capthick=2, label='Producteurs/Consommateurs')
plt.xlabel('Nombre de threads')
plt.ylabel('Temps d\'exécution (s)')
plt.title('Performance du problème des producteurs/consommateurs')
plt.ylim(bottom=0)
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('graphique_prod_cons.pdf')
plt.close()

# Graphique 4: spinlock TAS 
plt.figure(figsize=(10,6))
plt.errorbar(stats_spin.index, stats_spin['mean'], yerr=stats_spin['std'],
             marker='o', capsize=5, capthick=2, label='Test-and-Set', color='blue')
plt.xlabel('Nombre de threads')
plt.ylabel("Temps d'exécution (s)")
plt.title('Performance du verrou Test-and-Set')
plt.ylim(bottom=0)
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('graphique_spinlock.pdf')
plt.close()

# Graphique 5: spinlock TTAS 

plt.figure(figsize=(10,6))
plt.errorbar(stats_ttas.index, stats_ttas['mean'], yerr=stats_ttas['std'],
            marker='s', capsize=5, capthick=2, label='Test-and-Test-and-Set (TTAS)', color='green')
plt.xlabel('Nombre de threads')
plt.ylabel("Temps d'exécution (s)")
plt.title('Performance du verrou Test-and-Test-and-Set (TTAS)')
plt.ylim(bottom=0)
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('graphique_ttas.pdf')
plt.close()

# Graphique 6: comparaison TAS vs TTAS

plt.figure(figsize=(10,6))
plt.errorbar(stats_spin.index, stats_spin['mean'], yerr=stats_spin['std'],
            marker='o', capsize=5, capthick=2, label='Test-and-Set (TAS)', color='blue')
plt.errorbar(stats_ttas.index, stats_ttas['mean'], yerr=stats_ttas['std'],
            marker='s', capsize=5, capthick=2, label='Test-and-Test-and-Set (TTAS)', color='green')
plt.xlabel('Nombre de threads')
plt.ylabel("Temps d'exécution (s)")
plt.title('Comparaison TAS vs TTAS')
plt.ylim(bottom=0)
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('graphique_comparaison_tas_ttas.pdf')
plt.close()

print("7 graphiques générés avec succès!")

