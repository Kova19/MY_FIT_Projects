// Created by Martin Kovacik (xkovacm01) on 20.4.2024.
// proj2.c second project for IOS
// Tested on MacOS Sonoma and merlin.fit.vutbr.cz

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <limits.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include "proj2.h"

pid_t id = 0;
sem_t* writing;

void skibus(void){
    //Starting bus function
    srand(time(NULL) + rand() + id); //Seting up some randomizer
    usleep(rand() % TB);
    sem_wait(writing);
    fprintf(file, "%d: BUS: started\n", A[0]++);
    sem_post(writing);
    int help = 0;
    usleep(rand() % TB);

    while (num_skiers[0] != 0) 
    {   //Bus will be riding until every skier went skiing
        for (int i = 1; i <= Z; i++)
        { // for cykle for each bus stop
            if (arrived[i] == 0)
            {
                usleep(rand() % TB);
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i);
                sem_post(writing);
                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                if (i == Z)
                {
                    help = 1;
                    break;
                }
                else{
                    continue;
                }
            }
            switch (i)
            {
            case 1://Function for bus_stop
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i);  
                sem_post(writing);

                sem_post(zastavka1); // for all proces waiting on zastavka1
                sem_wait(allboard); // waiting for all skier on zastavka1

                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            case 2:
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i);
                sem_post(writing);

                sem_post(zastavka2);
                sem_wait(allboard);
                sem_wait(writing); 
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            case 3:
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i);   
                sem_post(writing);

                sem_post(zastavka3);
                sem_wait(allboard); 
                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            case 4:
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i);    
                sem_post(writing);

                sem_post(zastavka4);
                sem_wait(allboard); 
                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            case 5:
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i);  
                sem_post(writing);

                sem_post(zastavka5);
                sem_wait(allboard); 
                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            case 6:
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i);    
                sem_post(writing);

                sem_post(zastavka6);
                sem_wait(allboard); 
                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            case 7:
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i);    
                sem_post(writing);

                sem_post(zastavka7);
                sem_wait(allboard); 
                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            case 8:
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i);    
                sem_post(writing);

                sem_post(zastavka8);
                sem_wait(allboard); 
                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            case 9:
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i); 
                sem_post(writing);

                sem_post(zastavka9);
                sem_wait(allboard); 
                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            case 10:
                sem_wait(writing);
                fprintf(file, "%d: BUS: arrived to %d\n", A[0]++, i); 
                sem_post(writing);

                sem_post(zastavka10);
                sem_wait(allboard); 
                sem_wait(writing);
                fprintf(file, "%d: BUS: leaving %d\n", A[0]++, i);
                sem_post(writing);
                break;
            
            default:
                break;
            
            }
            //sem_post(multiplex);
        }
        sem_wait(writing);
        fprintf(file, "%d: BUS: arrived to final\n", A[0]++);
        sem_post(writing);
        usleep(rand() % TB);

        if (help == 1 && board[0] == 0)//This helps me with no skiier on bus stop
        {
            help = 0;
            sem_wait(writing);
            fprintf(file, "%d: BUS: arrived to final\n", A[0]++);
            sem_post(writing);
            sem_wait(writing);
            fprintf(file, "%d: BUS: leaving final\n", A[0]++);
            sem_post(writing);
            usleep(rand() % TB);
            continue;
        }
            
        sem_post(multiplex); //For all proces waiting to go skiing
        sem_wait(passagerSkier); //Waiting for last board skiier
        usleep(rand() % TB);
        sem_wait(writing);
        fprintf(file, "%d: BUS: leaving final\n", A[0]++);
        sem_post(writing);
        usleep(rand() % TB);
    }

    sem_post(mutex); //Say to main proces that we are finished
    sem_wait(writing);
    fprintf(file, "%d: BUS: finish\n", A[0]++);
    sem_post(writing);
    clean();
    exit(0);
}

void skier(void){
    srand(time(NULL) + id + uniqueID);
    
    sem_wait(writing);
    fprintf(file, "%d: L %d: started\n", A[0]++, uniqueID);
    sem_post(writing);
    usleep(rand() % TL + 1);

    int bus_stop = rand() % Z + 1; // Select random bus stop

    switch (bus_stop)
    {
    case 1:
        sem_wait(writing);
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++; //Say that some one is waiting on bust stop X
        while (1)//This cycle helps with full bus
        {
            sem_wait(zastavka1);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--; // If skiier board then decrement arrived on X
        board[0]++; // increment board skiiers
        if (arrived[bus_stop] != 0) // if he is not last then say other proces that they can go
        {
            sem_post(zastavka1);
        }

        break;
    case 2:
        sem_wait(writing);
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++;
        while (1)
        {
            sem_wait(zastavka2);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--;
        board[0]++;
        if (arrived[bus_stop] != 0)
        {
            sem_post(zastavka2);
        }
        
        break;
    case 3:
        sem_wait(writing);
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++;
        while (1)
        {
            sem_wait(zastavka3);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--;
        board[0]++;
        if (arrived[bus_stop] != 0)
        {
            sem_post(zastavka3);
        }
        
        break;
    case 4:
        sem_wait(writing);
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++;
        while (1)
        {
            sem_wait(zastavka4);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--;
        board[0]++;
        if (arrived[bus_stop] != 0)
        {
            sem_post(zastavka4);
        }
        
        break;
    case 5:
        sem_wait(writing);
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++;
        while (1)
        {
            sem_wait(zastavka5);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--;
        board[0]++;
        if (arrived[bus_stop] != 0)
        {
            sem_post(zastavka5);
        }
        
        break;
    case 6:
        sem_wait(writing);
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++;
        while (1)
        {
            sem_wait(zastavka6);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--;
        board[0]++;
        if (arrived[bus_stop] != 0)
        {
            sem_post(zastavka6);
        }
        
        break;
    case 7:
        sem_wait(writing);
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++;
        while (1)
        {
            sem_wait(zastavka7);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--;
        board[0]++;
        if (arrived[bus_stop] != 0)
        {
            sem_post(zastavka7);
        }
        
        break;
    case 8:
        sem_wait(writing);    
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++;
        while (1)
        {
            sem_wait(zastavka8);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--;
        board[0]++;
        if (arrived[bus_stop] != 0)
        {
            sem_post(zastavka8);
        }
        
        break;
    case 9:
        sem_wait(writing);
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++;
        while (1)
        {
            sem_wait(zastavka9);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--;
        board[0]++;
        if (arrived[bus_stop] != 0)
        {
            sem_post(zastavka9);
        }
        
        break;
    case 10:
        sem_wait(writing);
        fprintf(file, "%d: L %d: arrived to %d\n", A[0]++, uniqueID, bus_stop);
        sem_post(writing);
        arrived[bus_stop]++;
        while (1)
        {
            sem_wait(zastavka10);
            if (board[0] == K)
            {
                sem_post(allboard);
            }
            else{
                break;
            }
        }
        sem_wait(writing);
        fprintf(file, "%d: L %d: boarding\n", A[0]++, uniqueID);
        sem_post(writing);
        arrived[bus_stop]--;
        board[0]++;
        if (arrived[bus_stop] != 0)
        {
            sem_post(zastavka10);
        }
        
        break;
    
    default:
        break;
    }

    if (arrived[bus_stop] == 0)
    {
        sem_post(allboard); // If this is last skiier, then say to bus that he can go
    }
    
    sem_wait(multiplex); // Now wait until final bus stop
    sem_wait(writing);
    fprintf(file, "%d: L %d: going to ski\n", A[0]++, uniqueID);
    sem_post(writing);
    board[0]--;
    num_skiers[0]--;

    sem_post(multiplex); //Tell others to get out of bus 

    if (board[0] == 0) // If bus is empty
    {
        sem_post(passagerSkier); //Say it to bus that he can go
        sem_wait(multiplex);
    }
    

    clean();
    exit(0);
}

void clean(void){ // just cleaning function
    
    //Close all semaphores
    sem_close(mutex);
    sem_close(multiplex);
    sem_close(bus);
    sem_close(allboard);
    sem_close(passagerSkier);

    sem_close(zastavka1);
    sem_close(zastavka2);
    sem_close(zastavka3);
    sem_close(zastavka4);
    sem_close(zastavka5);
    sem_close(zastavka6);
    sem_close(zastavka7);
    sem_close(zastavka8);
    sem_close(zastavka9);
    sem_close(zastavka10);

    sem_close(writing);

    //Unlink semaphores
    sem_unlink("/xkovacm01_mutex");
    sem_unlink("/xkovacm01_multiplex");
    sem_unlink("/xkovacm01_bus");
    sem_unlink("/xkovacm01_AllBoard");
    sem_unlink("/xkovacm01_Passager");

    sem_unlink("/xkovacm01_Writing");

    //Unlinck bus stops
    sem_unlink("/xkovacm01_zastavky1");
    sem_unlink("/xkovacm01_zastavky2");
    sem_unlink("/xkovacm01_zastavky3");
    sem_unlink("/xkovacm01_zastavky4");
    sem_unlink("/xkovacm01_zastavky5");
    sem_unlink("/xkovacm01_zastavky6");
    sem_unlink("/xkovacm01_zastavky7");
    sem_unlink("/xkovacm01_zastavky8");
    sem_unlink("/xkovacm01_zastavky9");
    sem_unlink("/xkovacm01_zastavky10");

    //clean shared memory
    munmap(writing, sizeof(sem_t));
    munmap(arrived, sizeof(int));
    munmap(board, sizeof(int));
    munmap(num_skiers, sizeof(int));
    munmap(A, sizeof(int));

    //clean other semaphores
    munmap(mutex,sizeof(sem_t));
    munmap(multiplex,sizeof(sem_t));
    munmap(bus,sizeof(sem_t));
    munmap(allboard,sizeof(sem_t));
    munmap(passagerSkier,sizeof(sem_t));

    //Clean shared bustops
    munmap(zastavka1,sizeof(sem_t));
    munmap(zastavka2,sizeof(sem_t));
    munmap(zastavka3,sizeof(sem_t));
    munmap(zastavka4,sizeof(sem_t));
    munmap(zastavka5,sizeof(sem_t));
    munmap(zastavka6,sizeof(sem_t));
    munmap(zastavka7,sizeof(sem_t));
    munmap(zastavka8,sizeof(sem_t));
    munmap(zastavka9,sizeof(sem_t));
    munmap(zastavka10,sizeof(sem_t));
        
}


int main(int argc, char *argv[]){

    //Parse data then check them if they are okey
	L = atoi(argv[1]);
	Z = atoi(argv[2]);
	K = atoi(argv[3]);
	TL = atoi(argv[4]);
	TB = atoi(argv[5]);


	if(argc != 6){
		fprintf(stderr,"Incorrect number of arguments!\n");
		exit(1);
	}

    if (L < 0 || L >= 20000)
    {
        fprintf(stderr, "Too many skiers!\n");
        exit(1);
    }
    if (Z <= 0 || Z > 10)
    {
        fprintf(stderr, "Wrong number of bus stops!\n");
        exit(1);
    }
    if (K < 10 || K > 100)
    {
        fprintf(stderr, "Wrong bus capacity!\n");
        exit(1);
    }

    if (TL < 0 || TL > 10000)
    {
        fprintf(stderr, "Wrong waiting time for skiers!\n");
        exit(1);
    }
    if (TB  < 0 || TB > 1000) 
    {
        fprintf(stderr, "Wrong time between 2 bus stops!\n");
        exit(1);
    }

    //After check data then open file ...

    if ((file = fopen("./proj2.out","w"))== NULL){
        fprintf(stderr, "File failed to open!\n");
        exit(1);
    }    
    //set buffer to NULL 
    fflush(file);
    setbuf(file, NULL);

    
    srand(time(NULL));
    
    //Create semaphores
    mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    mutex = sem_open("/xkovacm01_mutex", O_CREAT, 0666, 1);
    multiplex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    multiplex = sem_open("/xkovacm01_multiplex", O_CREAT, 0666, 0);
    bus = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    bus = sem_open("/xkovacm01_bus", O_CREAT, 0666, 0);
    allboard = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    allboard = sem_open("/xkovacm01_AllBoard", O_CREAT, 0666, 0);
    passagerSkier = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    passagerSkier = sem_open("/xkovacm01_Passager", O_CREAT, 0666, 0);

    writing = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    writing = sem_open("/xkovacm01_Writing", O_CREAT, 0666, 1);


    //Create semaphores for busstops
    zastavka1 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka1 = sem_open("/xkovacm01_zastavky1", O_CREAT, 0666, 0);
    zastavka2 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka2 = sem_open("/xkovacm01_zastavky2", O_CREAT, 0666, 0);
    zastavka3 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka3 = sem_open("/xkovacm01_zastavky3", O_CREAT, 0666, 0);
    zastavka4 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka4 = sem_open("/xkovacm01_zastavky4", O_CREAT, 0666, 0);
    zastavka5 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka5 = sem_open("/xkovacm01_zastavky5", O_CREAT, 0666, 0);
    zastavka6 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka6 = sem_open("/xkovacm01_zastavky6", O_CREAT, 0666, 0);
    zastavka7 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka7 = sem_open("/xkovacm01_zastavky7", O_CREAT, 0666, 0);
    zastavka8 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka8 = sem_open("/xkovacm01_zastavky8", O_CREAT, 0666, 0);
    zastavka9 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka9 = sem_open("/xkovacm01_zastavky9", O_CREAT, 0666, 0);
    zastavka10 = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    zastavka10 = sem_open("/xkovacm01_zastavky10", O_CREAT, 0666, 0);
    
    //Create shared memory ints 
    A = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    arrived = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    board = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);
    num_skiers = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1 ,0);

    //set all int to values which i need
    board[0] = 0;
    A[0] = 1;
    uniqueID = 0;
    num_skiers[0] = L;

    //Do some forking stuff
    for (int i = 0; i < L+1; i++)
    {
        id = fork();

        if (id == -1)
        {
            fprintf(stderr, "Something went wrong with forking!\n");
            clean();
        }
        if (id == 0)
        {
            if (i == 0)
            {
                skibus();
            }
            else{
                uniqueID = i;
                skier();
            }
        }
    }

    //Wait until bus finish
    sem_wait(mutex);

    clean();
    if(fclose(file) == EOF){
        fprintf(stderr,"Error while closing file!\n");
        exit(1);
    }
    return 0;
}
