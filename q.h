#include <stdlib.h>
#include "tcb.h"

struct TCB_t *NewItem();								// returns a pointer to a new q-element
void InitQueue(struct TCB_t **newHead);					// creates an empty queue, pointed to by the 
														// variable head

void AddQueue(struct TCB_t **head, struct TCB_t *item);	//adds a queue item, pointed to by "item"
														// in the queue

struct TCB_t *DelQueue(struct TCB_t **head);			// deletes an item from head and returns
														// a pointer to the deleted item

void RotateQ(struct TCB_t **head);						// Moves header pointer to the next element
														// in queue. this is equivalent 
														// AddQ(&head, DeleteQ(&head)) but is simpler
														// to use and more fficient to implement

//int counter;
struct TCB_t *first, *last;								// tcb called first and tcb called last
														// first and last point to TCBs

TCB_t *NewItem() {
	TCB_t *q;
	q = (TCB_t *)malloc(sizeof(TCB_t));
	return q;
}

void InitQueue(struct TCB_t **newHead) {
	*newHead = NULL;
	//counter = 0;

}

void AddQueue(struct TCB_t **head, struct TCB_t *item) {
	
	if (*head == NULL) {
		*head = item;
		(*head)->next = *head;
		(*head)->prev = *head;

		first = *head;
		last = (*head)->prev;
	}

	else {
	
		(*head)->prev->next = item;
		item->prev = (*head)->prev;
		(*head)->prev = item;

		//last->next = item;
		//item->prev = last;
		//last = item;

	}
	(*head)->prev = item;		// redundant?
	item->next = (*head);

	//first->prev = last;
	//last->next = first;
	//counter++;
}

struct TCB_t *DelQueue(struct TCB_t **head) {
	struct TCB_t *temp;

	
	temp = *head;
	if (*head != NULL) {
		if ((*head)->next != *head) {
			*head = (*head)->next;
			(*head)->prev = temp->prev;
			(*head)->prev->next = (*head);
			//first  = *head;
			//first->prev = last;
			//last->next = first;
		}
		else {
			*head = NULL;
		}
	}
	else {
		*head = NULL;
	}
	//temp->next = NULL;
	//temp->prev = NULL;

	//counter++;
	return temp;
}

void RotateQ(struct TCB_t **head) {
	AddQueue(head, DelQueue(head));

}
