#ifndef PROJ2_H_
#define PROJ2_H_


//Define arguments

int L, Z, K, TL, TB;



// Init file
FILE* file;

// Shared memory intiger
int uniqueID;

// Semaphores

sem_t* mutex;
sem_t* allboard;
sem_t* bus;
sem_t* multiplex;
sem_t* passagerSkier;

sem_t* zastavka1, *zastavka2, *zastavka3, *zastavka4, *zastavka5, *zastavka6, *zastavka7, *zastavka8, *zastavka9, *zastavka10;


//Functions

void skier(void);
void clean(void);
void skibus(void);

//Shared memory arrays
int *A;
int* arrived;
int* board;
int* riders;
int* num_skiers;

#endif // PROJ2_H_


