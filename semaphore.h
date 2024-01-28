#include <sys/types.h>
// #include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Basic struct
struct sembuf sem_operation;

// Init semaphore
int initialize_semaphore(int key, int num_sems) {
    int semaphore_id = semget(key, num_sems, IPC_CREAT | 0666);
    if (semaphore_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    return semaphore_id;
}

// Set value in semaphore
void set_semaphore_value(int semaphore_id, int sem_num, int value) {
    union semun {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } arg;

    arg.val = value;
    if (semctl(semaphore_id, sem_num, SETVAL, arg) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }
}

// Get value from semaphore
int get_semaphore_value(int semaphore_id, int sem_num) {
    int value = semctl(semaphore_id, sem_num, GETVAL);
    if (value == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    return value;
}

// Operation P (waiting) on semaphore
void semaphore_wait(int semaphore_id, int sem_num) {
    sem_operation.sem_num = sem_num;
    sem_operation.sem_op = -1;
    sem_operation.sem_flg = 0;

    if (semop(semaphore_id, &sem_operation, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

// Operation V (signalize) on semaphore
void semaphore_signal(int semaphore_id, int sem_num) {
    sem_operation.sem_num = sem_num;
    sem_operation.sem_op = 1;
    sem_operation.sem_flg = 0;

    if (semop(semaphore_id, &sem_operation, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}

void remove_semaphore(int semaphore_id) {
    if (semctl(semaphore_id, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }
}