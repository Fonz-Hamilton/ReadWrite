/*
** Fonz Hamilton
** CSE 330
** Assignment 4
*/

#include <stdio.h>
#include "sem.h"

void reader();
void writer();
void debugMain();

int buffSize = 0;		// buffer size
int numProd = 0;		// number of producers
int numCons = 0;		// number of consumers
//int n = 0;				// number of items producers/consumers make/consume

//for assignment 4
int sp = 0;				// global variable i set to 0 initially *I refuse to use "i"*
int n = 2;				// how many reads and writes
int numRead = 0;		// number of reader
int numWrite = 0;		// number of writers	
int rc = 0;				// read counter
int wc = 0;				// write counter
int rwc = 0;			// read wait count
int wwc = 0;			// write wait count

int *buffer;			// buffer array to store items
int in = 0;				// the index the producers can write in to next
int out = 0;			// the index the consumers can read from next

int numThreads = 0;		// numProd + numCons (R + W)

struct semaphore *R_Sem;	// for reader
struct semaphore *W_Sem;	// For writer

int main(int argc, char *argv[]) {
	
	scanf("%d,%d", &numRead, &numWrite);

	RunQ = NewItem();				// declare RunQ
    InitQueue(&RunQ);				// initialize RunQ

	//buffer = malloc(buffSize *sizeof(int));


    // make semaphores  
    R_Sem = (struct semaphore *)malloc(sizeof(struct semaphore));	// for producer
    W_Sem = (struct semaphore *)malloc(sizeof(struct semaphore));	// for consumer

	// initialize semaphore queues
	R_Sem->queue = NewItem();
	W_Sem->queue = NewItem();

	// initialize semaphores
    InitSem(R_Sem, 0);			// for reader
    InitSem(W_Sem, 0);			// for writer

	// pass in the arguments and makes consumer/producer based on positive or negative number
	numThreads = numRead + numWrite;				// R + W
	for (int i = 0; i < numThreads; i++ ) {
		int inputID;
		scanf("%d", &inputID);
		if (inputID >= 0) {
			start_thread(reader, inputID, n);	// pass id and how many items can be read/written
		}

		else {
			start_thread(writer, inputID, n);
		}
		
	}

//	fflush(stdout);					// flush the input stream just in case

	run();
	return 0;

}

void reader(int id, int itemsToProduce) {

	if (wwc > 0 || wc > 0) {
		rwc++;
		P(R_Sem, id);
		rwc--;
	}
	rc++;

			
	printf("\n This is the %d th reader reading value i = %d for the first time \n", id, sp);
	yield();
			
	printf("\n This is the %d th reader reading value i = %d for the second time \n", id, sp);
		
	rc--;
	if (rc == 0 && wwc > 0) {
		V(W_Sem);
	}


	// get rid of the thread in the queue since its done
	struct TCB_t *deleted = DelQueue(&RunQ);				// testing 
    swapcontext(&(deleted->context), &(RunQ->context));
//	debugMain();
//	exit(0);
//	yield();

		
		
}

void writer(int id, int itemsToConsume) {
//	printf("\n\nrwc at writer %d: %d", id, rwc);
	if (rc > 0 || wc > 0  || rwc > 0 || wwc > 0) {
		wwc++;
		P(W_Sem, id);		// if scratchpad is full then will get stuck in P
		wwc--;
	}
	
	wc++;

	// enter scratchPad

	sp++;
	printf("\n This is the %d th writer writing value i = %d \n", id * -1, sp);
	yield();		

	printf("\n This is the %d th writer verifying value i = %d \n", id * -1, sp);	

			
	wc--;
//	printf("\n\nrwc before for loop: %d", rwc);
	if (rwc > 0) {
		for (int i = 0; i < rwc; i++) {
			//debugMain();
			V(R_Sem);
		}
	}
	else if (wwc > 0) {
		V(W_Sem);
	}
		

//	printf("\n\nthis is the end\n");
	struct TCB_t *deleted = DelQueue(&RunQ);
	swapcontext(&(deleted->context), &(RunQ->context));     // testing
//	yield();
	
}

void debugMain() {
	
	TCB_t *temp = RunQ;
    printf("\nthe current RunQ\n");
    int k = 0;
    int tootles = temp->id;
    int count = 0;
    while (k < 20) {
        if (temp->id == tootles && count > 1) {
        //  printf("%d ", temp->id);
        //  temp = temp->next;
        //  count++;

        }
        else {
            printf("%d ", temp->id);
            temp = temp->next;
            count++;
        }
        k++;
    }
    printf("\n");


    TCB_t *temp1 = R_Sem->queue;
    printf("\nthe current R_Sem->queue\n");
    int noodles = 0;
    if (temp1 != NULL) {
        noodles = temp1->id;
        count = 0;
    }
    k = 0;
    while (k < 20) {
        if (temp1 != NULL) {
            if (temp1->id == noodles && count >= 1) {
            //  printf("%d ", temp1->id);
            //  temp1 = temp1->next;
            }

            else {
                printf("%d ", temp1->id);
                temp1 = temp1->next;
                count++;
            }

        }
        k++;
    }
    printf("\n");


	TCB_t *temp2 = W_Sem->queue;
    printf("\nthe current W_Sem->queue\n");
    int poodles = 0;
    if (temp2 != NULL) {
        poodles = temp2->id;
        count = 0;
    }
    k = 0;
    while (k < 20) {
        if (temp2 != NULL) {
            if (temp2->id == poodles && count >= 1) {
            //  printf("%d ", temp1->id);
            //  temp2 = temp2->next;
            }

            else {
                printf("%d ", temp2->id);
                temp2 = temp2->next;
                count++;
            }

        }
        k++;
    }
    printf("\n");
}
