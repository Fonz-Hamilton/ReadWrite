#include <stdio.h>
#include "threads.h"

struct semaphore {
	int value;
	struct TCB_t *queue;

};

void debug();
//int val;

void InitSem(struct semaphore *s, int value) {
	s->value = value;

	InitQueue(&s->queue);
}

struct semaphore *debugS;

void P(struct semaphore *s, int threadID) {
	//s->value--;					// decrement the semaphore; if value is 0 or less than zero then
									// blocks the process in the queue associated with the semaphore

//	printf("\nThe value of s->value at the top: %d\n", s->value);
	
	if (s->value > 0) {
		 
		s->value--;
	}

	else {

		while (s->value <= 0) {			// loops until the s->value changes
	/*
			if (threadID >= 0) {
				printf("\n Producer %d is waiting \n", threadID);
			}
			else {
				printf("\n Consumer %d is waiting \n", threadID * -1);
			}
	*/
			struct TCB_t *currentThread = DelQueue(&RunQ);  // take the tcb from delQueue and make it current
        	AddQueue(&(s->queue), currentThread);   		// add it to the end of semaphore queue
     	
		/*
			printf("\nin P\n");
			debugS = s;
			debug();
			printf("\nend of P\n");
		*/
			if ( RunQ == NULL) {
//				printf("runq is Null ############\n");
				exit(0);
			}

//	  		printf("\nthe currentThread:  %d\n", currentThread->id);
//			printf("\nthe front of q:  %d\n", s->queue->id);
//			yield();
//			printf("\n\ns->value: %d\n", s->value);
        	swapcontext(&(currentThread->context), &(RunQ->context)); // go to next context
//			yield();
		}
		s->value--;		


	}
	
	
//	debugS = s;
//	debug();
}

void V(struct semaphore *s) {
	s->value++;				// increment semaphore; if the value is 0 or negative, then takes a tcb
							// out of the semaphore queue and puts it into the run queue


	if (s->value <= 0 || s->queue != NULL) {
//	if (s->value <= 0 ) {

		/*
		printf("\nV debug before delqueue\n");
        debugS = s;
        debug();
        printf("\nend of V before delqueue\n");
		*/
		

		struct TCB_t *currentThread = DelQueue(&(s->queue));

		/*
		printf("\nV debug after delqueue\n"); 
        debugS = s;
        debug();
        printf("\nend of V after delqueue\n");
		*/


		AddQueue(&RunQ, currentThread);	// delete from s queue and add it back to the RunQ
	//	printf("\ndoes this happen?\n");
		//swapcontext(&(currentThread->context), &(RunQ->context));

		/*
		printf("\nV debug after Addqueue\n");	
		debugS = s;
		debug();
		printf("\nend of V after Addqueue\n");
		*/		
	}
	
//	yield();
}

void debug() {

	//	printf("\nThe value of s->value: %d\n", s->value);

	
	TCB_t *temp = RunQ;
	printf("\nthe current RunQ\n");
	int k = 0;
	int tootles = temp->id;
	int count = 0;
	while (k < 6) {
		if (temp->id == tootles && count > 1) {
		//	printf("%d ", temp->id);
		//	temp = temp->next;
		//	count++;

		}
		else {
			printf("%d ", temp->id);
            temp = temp->next;
            count++;
		}
		k++;
	}
	printf("\n");


	TCB_t *temp1 = debugS->queue;
    printf("\nthe current s->queue\n");
	int noodles = 0;
	if (temp1 != NULL) {
		noodles = temp1->id;
		count = 0;
	}
    k = 0;
    while (k < 6) {
		if (temp1 != NULL) {
			if (temp1->id == noodles && count >= 1) {
        	//	printf("%d ", temp1->id);
        	//	temp1 = temp1->next;
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
}
