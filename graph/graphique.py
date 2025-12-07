#!/usr/bin/env python3

import matplotlib.pyplot as plt
import pandas as pd

# Lire les fichiers CSV - Partie 1
df_philo = pd.read_csv('../dataCSV/results_philosophes.csv')
df_rw = pd.read_csv('../dataCSV/results_lecteurs_ecrivains.csv')
df_pc = pd.read_csv('../dataCSV/results_prod_cons.csv')

# Lire les fichiers CSV - Partie 2
df_spin = pd.read_csv('../dataCSV/results_spinlock.csv')
df_ttas = pd.read_csv('../dataCSV/results_ttas.csv')

# Lire les fichiers CSV - Tâche 2.5 (attente active)
df_philo_spin = pd.read_csv('../dataCSV/results_philosophes_spinlock.csv')
df_rw_spin = pd.read_csv('../dataCSV/results_lecteurs_ecrivains_spinlock.csv')
df_pc_spin = pd.read_csv('../dataCSV/results_prod_cons_spinlock.csv')

# Calculer moyenne et écart-type pour chaque nombre de threads
stats_philo = df_philo.groupby('N_Threads')['Temps_Execution_s'].agg(['mean', 'std'])
stats_rw = df_rw.groupby('N_Threads')['Temps_Execution_s'].agg(['mean', 'std'])
stats_pc = df_pc.groupby('N_Threads')['Temps_Execution_s'].agg(['mean', 'std'])
stats_spin = df_spin.groupby('N_Threads')['Temps_Execution_s'].agg(['mean','std'])
stats_ttas = df_ttas.groupby('N_Threads')['Temps_Execution_s'].agg(['mean','std'])

# Stats pour versions attente active
stats_philo_spin = df_philo_spin.groupby('N_Threads')['Temps_Execution_s'].agg(['mean', 'std'])
stats_rw_spin = df_rw_spin.groupby('N_Threads')['Temps_Execution_s'].agg(['mean', 'std'])
stats_pc_spin = df_pc_spin.groupby('N_Threads')['Temps_Execution_s'].agg(['mean', 'std'])

# Graphique 1: Philosophes (POSIX vs Attente Active)
plt.figure(figsize=(10, 6))
plt.errorbar(stats_philo.index, stats_philo['mean'], yerr=stats_philo['std'], 
             marker='o', capsize=5, capthick=2, label='POSIX (pthread_mutex)', color='blue')
plt.errorbar(stats_philo_spin.index, stats_philo_spin['mean'], yerr=stats_philo_spin['std'], 
             marker='s', capsize=5, capthick=2, label='Attente Active (my_semaphore)', color='red')
plt.xlabel('Nombre de threads')
plt.ylabel('Temps d\'exécution (s)')
plt.title('Performance du problème des philosophes')
plt.ylim(bottom=0)
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('graphique_philosophes.pdf')
plt.close()

# Graphique 2: Lecteurs/Écrivains (POSIX vs Attente Active)
plt.figure(figsize=(10, 6))
plt.errorbar(stats_rw.index, stats_rw['mean'], yerr=stats_rw['std'], 
             marker='o', capsize=5, capthick=2, label='POSIX (sem_t)', color='blue')
plt.errorbar(stats_rw_spin.index, stats_rw_spin['mean'], yerr=stats_rw_spin['std'], 
             marker='s', capsize=5, capthick=2, label='Attente Active (my_semaphore)', color='red')
plt.xlabel('Nombre de threads')
plt.ylabel('Temps d\'exécution (s)')
plt.title('Performance du problème des lecteurs/écrivains')
plt.ylim(bottom=0)
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('graphique_lecteurs_ecrivains.pdf')
plt.close()

# Graphique 3: Producteurs/Consommateurs (POSIX vs Attente Active)
plt.figure(figsize=(10, 6))
plt.errorbar(stats_pc.index, stats_pc['mean'], yerr=stats_pc['std'], 
             marker='o', capsize=5, capthick=2, label='POSIX (sem_t)', color='blue')
plt.errorbar(stats_pc_spin.index, stats_pc_spin['mean'], yerr=stats_pc_spin['std'], 
             marker='s', capsize=5, capthick=2, label='Attente Active (my_semaphore)', color='red')
plt.xlabel('Nombre de threads')
plt.ylabel('Temps d\'exécution (s)')
plt.title('Performance du problème des producteurs/consommateurs')
plt.ylim(bottom=0)
plt.grid(True, alpha=0.3)
plt.legend()
plt.savefig('graphique_prod_cons.pdf')
plt.close()


# Graphique 4: spinlock comparaison

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

print("5 graphiques générés avec succès!")