#ifndef SEM_LIB
#define SEM_LIB

#include <semaphore.h>

#endif

#ifndef SEM_H
#define SEM_H

sem_t * initJobSem(int WORKERS);

void freeSem(sem_t * jobs_sem);

#endif