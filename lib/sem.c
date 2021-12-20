#include "../include/sem.h"
#include "../include/utils.h"


sem_t * initJobSem(int WORKERS){
    sem_t * jobs_sem = (sem_t *)malloc(sizeof(sem_t));
    sem_init(jobs_sem, 0, WORKERS);
    return jobs_sem;
}

void freeSem(sem_t * sem) {
    sem_destroy(sem);
    free(sem);
}