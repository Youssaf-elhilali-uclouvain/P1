# --- Configuration Globale ---
PROGS = philosophes lecteurs_ecrivains prod_cons test_spinlock test_ttas philosophes_spinlock prod_cons_spinlock lecteurs_ecrivains_spinlock
SRC_DIR = src
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3 -pthread -Iinclude

# ----------------------------------------------------------------------
# 1. Cibles de Construction (all et règles spécifiques)
# ----------------------------------------------------------------------
.PHONY: all
all: $(PROGS)

# --- Règles génériques pour philosophes, lecteurs_ecrivains, prod_cons ---
philosophes: $(SRC_DIR)/philosophes.c
	$(CC) $(CFLAGS) $< -o $@

lecteurs_ecrivains: $(SRC_DIR)/lecteurs_ecrivains.c
	$(CC) $(CFLAGS) $< -o $@

prod_cons: $(SRC_DIR)/prod_cons.c
	$(CC) $(CFLAGS) $< -o $@

# --- Règle pour test_spinlock avec TAS (test-and-set simple) ---
test_spinlock: $(SRC_DIR)/test_spinlock.c $(SRC_DIR)/spinlock.c
	$(CC) $(CFLAGS) $(SRC_DIR)/test_spinlock.c $(SRC_DIR)/spinlock.c -o $@

# --- Règle pour test_ttas avec TTAS (test-and-test-and-set) ---
test_ttas: $(SRC_DIR)/test_spinlock.c $(SRC_DIR)/ttas_spinlock.c
	$(CC) $(CFLAGS) $(SRC_DIR)/test_spinlock.c $(SRC_DIR)/ttas_spinlock.c -o $@

# --- Règles pour versions avec primitives d'attente active (Tâche 2.5) ---
philosophes_spinlock: $(SRC_DIR)/philosophes_spinlock.c $(SRC_DIR)/my_semaphore.c $(SRC_DIR)/ttas_spinlock.c
	$(CC) $(CFLAGS) $(SRC_DIR)/philosophes_spinlock.c $(SRC_DIR)/my_semaphore.c $(SRC_DIR)/ttas_spinlock.c -o $@

prod_cons_spinlock: $(SRC_DIR)/prod_cons_spinlock.c $(SRC_DIR)/my_semaphore.c $(SRC_DIR)/ttas_spinlock.c
	$(CC) $(CFLAGS) $(SRC_DIR)/prod_cons_spinlock.c $(SRC_DIR)/my_semaphore.c $(SRC_DIR)/ttas_spinlock.c -o $@

lecteurs_ecrivains_spinlock: $(SRC_DIR)/lecteurs_ecrivains_spinlock.c $(SRC_DIR)/my_semaphore.c $(SRC_DIR)/ttas_spinlock.c
	$(CC) $(CFLAGS) $(SRC_DIR)/lecteurs_ecrivains_spinlock.c $(SRC_DIR)/my_semaphore.c $(SRC_DIR)/ttas_spinlock.c -o $@

# ----------------------------------------------------------------------
# 2. Cibles de Test
# ----------------------------------------------------------------------
.PHONY: test_perf
test_perf: $(PROGS)
	@echo "--- Lancement de TOUS les tests de performance ---"
	scripts/performance_optimized.sh

# ----------------------------------------------------------------------
# 3. Nettoyage
# ----------------------------------------------------------------------
.PHONY: clean
clean:
	@echo "Nettoyage des exécutables..."
	rm -f $(PROGS)
	rm -f philosophes_spinlock prod_cons_spinlock lecteurs_ecrivains_spinlock
	rm -f dataCSV/*.csv
	rm -f graph/*.pdf
