
#ifndef DSLIB_H
#define DSLIB_H

#define MAXBUFFER	100

struct pqueue {
	int max, size;
	void **buffer;
	int count;  
	int (*comp)(void *a, void *b); 
};

void pq_init(pqueue *q, int size_of_item, int (*compfunc)(void *a, void *b), int max_cap=MAXBUFFER); 
bool pq_isempty(pqueue *q); 
bool pq_dequeue(pqueue *q, void *a); 
bool pq_enqueue(pqueue *q, void *a);

struct queue {
	int max, size; 
	void **buffer;
    int count;          /* number of queue elements */
    int first;          /* position of first element */
    int last;           /* position of last element */
};

void q_init(queue *q, int size_of_item, int max_cap=MAXBUFFER); 
void q_free(queue *q); 
bool q_isempty(queue *q); 
bool q_enqueue(queue *q, void *a); 
bool q_dequeue(queue *q, void *a); 

struct stack {
	int max, size;
	void **buffer;
	int count; 
};
void s_init(stack *s, int size_of_item, int max_cap=MAXBUFFER);
bool s_isempty(stack *s);
bool s_push(stack *s, void *a);
bool s_pop(stack *s, void *a); 

#endif