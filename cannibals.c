#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "semaphore.h"
#include <time.h>

#define A2B 0
#define B2A 1 
#define C2D 2
#define D2C 3

void cooker(int *portions, int no_refill) {
    /* DO COOKING STUFF */
    *portions = no_refill;
    printf("Cooker do cooking thing, %d\n", *portions);
}

void cannibal(int *portions) {
    /* EAT PORTION */
    printf("Cannibal eating portion no. %d\n", *portions);
    (*portions)--;
}

int main() {

    srand(time(NULL));

    int no_cannibals = 10;
    int pid = 0, status;

    // Utwórz obszar pamięci współdzielonej
    key_t key = ftok("shmfile", 'R');
    int shmid = shmget(key, sizeof(int), 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Przyłącz pamięć współdzieloną do procesu
    int *porcja = (int*) shmat(shmid, NULL, 0);
    *porcja = 0;
    if (porcja == (int*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // SEMAPHORES
    key_t sem_key = ftok("semafor", 'c');
    if (sem_key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Utwórz semafor
    int semaphore_id = initialize_semaphore(sem_key, 4);

    // Inicjalizuj semafory
    set_semaphore_value(semaphore_id, A2B, 1);
    set_semaphore_value(semaphore_id, B2A, 0);
    set_semaphore_value(semaphore_id, C2D, 0);
    set_semaphore_value(semaphore_id, D2C, 0);
    // SEMAPHORES //

    for (int i = 0; i < no_cannibals; i++) {
        pid = fork();
        usleep(1000*(rand() % 10) + 1);

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            for (int i = 0; 1 == 1; i++) {
                if (*porcja <= 0) {
                    /* CALL COOKER */
                    if (get_semaphore_value(semaphore_id, D2C) != 1) {
                        printf("CALL COOKER\n");
                        set_semaphore_value(semaphore_id, D2C, 1);
                    }
                    semaphore_wait(semaphore_id, C2D);
                } else {
                    semaphore_wait(semaphore_id, A2B);
                    semaphore_signal(semaphore_id, B2A);

                    cannibal(porcja);

                    semaphore_wait(semaphore_id, B2A);
                    semaphore_signal(semaphore_id, A2B);
                }

                usleep(100000*(rand() % 10) + 1);  // Add a 100ms delay
            }

            // Operacje w sekcji krytycznej dla procesu A
            exit(0);
        }
    }


    int no_refill = 7;
    for (int refill = 0; refill < no_refill; refill++) {
        semaphore_wait(semaphore_id, D2C);
        if (refill == no_refill - 1) {
            no_refill = -1;
        }
        cooker(porcja, no_refill);
        set_semaphore_value(semaphore_id, D2C, 0);
        semaphore_signal(semaphore_id, C2D);
        usleep(10000*(rand() % 10) + 1);  // Add a 100ms delay
    }

    // Odłącz pamięć współdzieloną
    shmdt(porcja);

    // Usuń obszar pamięci współdzielonej
    shmctl(shmid, IPC_RMID, NULL);

    /* COOKER ENDING PARTY, THERE IS NO LEFT FROM MISSIONARY */
    for (int i = 0; i < no_cannibals; i++) {
        printf("Waiting for cannibals to leave %d/%d \n", (no_cannibals-i), no_cannibals);
        pid = wait(&status);
        printf("PID = %d, status = %d\n", pid, status);
    }
    printf("END PARTY\n");

    return 0;
}
