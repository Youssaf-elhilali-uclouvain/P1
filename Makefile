# --- Configuration Globale ---
PROGS = philosophes lecteurs_ecrivains prod_cons
SRC_DIR = src
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3 -pthread

# ----------------------------------------------------------------------
# 1. Cibles de Construction (all et règle générique)
# ----------------------------------------------------------------------
.PHONY: all
all: $(PROGS)

# Règle générique de compilation : cible : dépendance
$(PROGS): %: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# ----------------------------------------------------------------------
# 2. Cibles de Test (Ajouter les autres cibles ici plus tard)
# ----------------------------------------------------------------------

# Cible pour l'évaluation de performance des Philosophes
.PHONY: philo_bench
philo_bench: philosophes
	@echo "--- Lancement Évaluation Performance Philosophes ---"
	scripts/run_tests_philo.sh

.PHONY: test_perf
test_perf: $(PROGS)
	@echo "--- Lancement de TOUS les tests de performance (scripts/performance_scripts.sh) ---"
	# Assurez-vous que le script a les droits d'exécution (U)
	scripts/performance_scripts.sh
    
# ----------------------------------------------------------------------
# 3. Nettoyage
# ----------------------------------------------------------------------
.PHONY: clean
clean:
	@echo "Nettoyage des exécutables..."
	rm -f $(PROGS)