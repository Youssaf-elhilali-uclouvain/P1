# --- Configuration Globale ---
PROGS = philosophes
# Une fois que vous aurez codé les autres, vous mettrez : PROGS = philosophes prod_cons lecteurs_ecrivains
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

# Test simple pour la partie Philosophes
.PHONY: philo_test
philo_test: philosophes
	@echo "--- Lancement test simple Philosophes (N=5) ---"
	./philosophes 5

# Cible pour l'évaluation de performance des Philosophes
.PHONY: philo_bench
philo_bench: philosophes
	@echo "--- Lancement Évaluation Performance Philosophes ---"
	scripts/run_tests_philo.sh
    
# ----------------------------------------------------------------------
# 3. Nettoyage
# ----------------------------------------------------------------------
.PHONY: clean
clean:
	@echo "Nettoyage des exécutables..."
	rm -f $(PROGS)