// priqueue.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dslib.h"
#define parent(n)		(((n)-1)/2)
#define left(n)			(2*(n)+1)
#define right(n)		(2*((n)+1))


void heapify(pqueue *q, int n)
{
	int l, r;
	int largest; 
	l = left(n);
	r = right(n); 
	if (l <= q->count && q->comp(q->buffer[l], q->buffer[n]) > 0) 
		largest = l;
	else
		largest = n; 
	if (r < q->count && q->comp(q->buffer[r], q->buffer[largest]) > 0)
		largest = r; 
	if (largest != n) {
		void *t = q->buffer[n];
		q->buffer[n] = q->buffer[largest];
		q->buffer[largest] = t;
		heapify(q, largest);
	}
}

void pq_init(pqueue *q, int size_of_item, int (*compfunc)(void *a, void *b), int max_cap)
{
	q->max = max_cap;
	q->size = size_of_item;
	q->count = 0; 
	q->comp = compfunc; 
	q->buffer = new void*[q->max];
}

bool pq_isempty(pqueue *q)
{
	return q->count == 0; 
}

bool pq_dequeue(pqueue *q, void *a)
{
	if (pq_isempty(q))
		return false; 
	memcpy(a, q->buffer[0], q->size); 
	q->count--; 
	q->buffer[0] = q->buffer[q->count];
	heapify(q, 0); 
	return true; 
}

bool pq_enqueue(pqueue *q, void *a)
{
	int i;
	if (q->count == q->max) {
		fprintf(stderr, "queue overflow\n");
		return false;
	}
	i = q->count;
	q->buffer[i] = malloc(q->size); 
	memcpy(q->buffer[i], a, q->size); 
	while (i>0 && q->comp(q->buffer[parent(i)], q->buffer[i]) < 0) {
		void *t = q->buffer[i];
		q->buffer[i] = q->buffer[parent(i)];
		q->buffer[parent(i)] = t;
		i = parent(i);
	}
	q->count++; 
	return true; 
}


