#include "q.h"


TCB_t *RunQ;	// global header pointer

// allocate a stack (via malloc) of a certain size (choose 8192)
// allocate a TCB (via malloc)
// call init_TCB with appropriate arguments
// call addQ to add this TCB into the “RunQ” which is a global header pointer

void start_thread(void(*function)(int, int), int id, int items) {
	void *stack = malloc(8192);
	TCB_t *tcb = NewItem();
	init_TCB(tcb, function, stack, 8192, id, items);
	AddQueue(&RunQ, tcb);

}

void run() {
	ucontext_t parent;			// get a place to store the main context, for faking

	getcontext(&parent);		// magic sauce(???)

	swapcontext(&parent, &(RunQ->context));	// start the first thread
}

void yield() {
	// rotate the RunQ
	// this is how you cycle through the queue
	TCB_t *currentThread;
	TCB_t *prevThread;

	prevThread = RunQ;
	RotateQ(&RunQ);
	currentThread = RunQ;

	// swap the context, from previous thread to the thread pointed to by RunQ
	swapcontext(&(prevThread->context), &(currentThread->context));
}

