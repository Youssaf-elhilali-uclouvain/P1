#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>   // Pour clock_gettime

// --- Variables Globales ---
// N mutex pour N fourchettes.
pthread_mutex_t *forks;
// Le nombre de philosophes (lu dans main).
int N_PHILOSOPHES;

// --- Fonctions utilitaires de mesure du temps ---

// Fonction pour simuler la phase Manger ou Penser sans attendre (simplement une boucle)
void busy_wait() {
    // 1,000,000 cycles sont gérés par la boucle dans la fonction du thread.
    // Cette boucle vide simule l'action immédiate demandée par le projet.
    for (volatile long i = 0; i < 1; i++); 
}

// Fonction exécutée par chaque thread/philosophe
void* philosophe_action(void* arg) {
    // Récupération de l'ID du philosophe.
    long id = (long)arg; 

    // Définition des fourchettes :
    // Fourchette gauche = ID du philosophe
    int fork_left = id; 
    // Fourchette droite = (ID + 1) % N
    int fork_right = (id + 1) % N_PHILOSOPHES; 

    // Chaque philosophe effectue 1,000,000 cycles penser/manger
    for (long cycle = 0; cycle < 1000000; cycle++) {
        // 1. Penser (Action immédiate / busy_wait)
        // printf("Philosophe %ld PENSE\n", id); // Décommentez pour déboguer
        busy_wait(); 

        // 2. Tenter de Manger (Acquisition des deux fourchettes)
        
        // --- Stratégie Anti-Deadlock : Asymétrie ---
        if (id == N_PHILOSOPHES - 1) { // Le dernier philosophe (N-1) commence par la droite
            pthread_mutex_lock(&forks[fork_right]);
            pthread_mutex_lock(&forks[fork_left]);
        } else { // Tous les autres commencent par la gauche
            pthread_mutex_lock(&forks[fork_left]);
            pthread_mutex_lock(&forks[fork_right]);
        }
        // --- Fin de la Stratégie ---

        // 3. Manger (Section critique)
        // printf("Philosophe %ld MANGE\n", id); // Décommentez pour déboguer
        busy_wait(); // Manger est aussi une action immédiate (busy_wait)

        // 4. Reposer les fourchettes (Déverrouillage)
        pthread_mutex_unlock(&forks[fork_left]);
        pthread_mutex_unlock(&forks[fork_right]);
    }

    return NULL;
}

// --- Fonction Principale ---

int main(int argc, char *argv[]) {
    // Gestion des arguments en ligne de commande
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <Nombre_de_Philosophes (N)>\n", argv[0]);
        return 1;
    }

    // Récupération du nombre de philosophes et de fourchettes
    N_PHILOSOPHES = atoi(argv[1]);
    if (N_PHILOSOPHES < 2) {
        fprintf(stderr, "Erreur: Le nombre de philosophes doit être >= 2\n");
        return 1;
    }

    // Allocation de la mémoire pour les N threads et les N mutex (fourchettes)
    pthread_t *threads = malloc(N_PHILOSOPHES * sizeof(pthread_t));
    forks = malloc(N_PHILOSOPHES * sizeof(pthread_mutex_t));

    if (!threads || !forks) {
        perror("Erreur d'allocation mémoire");
        return 1;
    }

    // 1. Initialisation des N mutex (fourchettes)
    for (int i = 0; i < N_PHILOSOPHES; i++) {
        if (pthread_mutex_init(&forks[i], NULL) != 0) {
            perror("Erreur d'initialisation de mutex");
            return 1;
        }
    }

    // Mesure du temps de début
    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    // 2. Création des N threads (Philosophes)
    for (long i = 0; i < N_PHILOSOPHES; i++) {
        // Envoi de l'ID du philosophe (i) à la fonction du thread
        if (pthread_create(&threads[i], NULL, philosophe_action, (void*)i) != 0) {
            perror("Erreur de création de thread");
            return 1;
        }
    }

    // 3. Attente de la fin des N threads
    for (int i = 0; i < N_PHILOSOPHES; i++) {
        pthread_join(threads[i], NULL);
    }

    // Mesure du temps de fin
    clock_gettime(CLOCK_MONOTONIC, &fin);

    // 4. Destruction des N mutex
    for (int i = 0; i < N_PHILOSOPHES; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    // 5. Libération de la mémoire
    free(threads);
    free(forks);

    // 6. Affichage du temps d'exécution (Métrique clé pour le projet)
    double elapsed_sec = (fin.tv_sec - debut.tv_sec) + (fin.tv_nsec - debut.tv_nsec) / 1e9;
    
    // Pour faciliter la collecte des données par script (bash/python), on affiche le temps seul :
    printf("%.6f\n", elapsed_sec);

    return 0;
}