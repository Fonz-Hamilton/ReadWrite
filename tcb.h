#include <ucontext.h>
#include <string.h>

typedef struct TCB_t {					
	struct TCB_t *next;
	struct TCB_t *prev;
	int id;
	int item;
	ucontext_t context;
} TCB_t;

// arguments (Pointer to othe function, to be executed), (pointer to the thread stack), (size of the stack)
void init_TCB (TCB_t *tcb, void *function, void *stackP, int stack_size, int threadID, int numOfItems) {

	memset(tcb, '\0', sizeof(TCB_t)); 	// wash, rinse
	getcontext(&tcb->context);			// have to get parent context

	tcb->id = threadID;					// set the tcb id to the threadID passed from main
	tcb->item = numOfItems;				// set the tcb item to numOfItems passed from main (n)

	tcb->context.uc_stack.ss_sp = stackP;
	tcb->context.uc_stack.ss_size = (size_t) stack_size;

	
	makecontext(&tcb->context, function, 2, threadID, numOfItems);
}
