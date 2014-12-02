
/*	queue-base.c

	Dongsoo S. Kim

	Copyright(c) 2013, Dongsoo S. Kim

	Permission is granted for use in non-commerical applications
provided this copyright notice remains intact and unchanged.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dslib.h"

bool q_enqueue(queue *q, void *a) 
{
    if (q->count >= q->max) {
		fprintf(stderr, "Warning: queue overflow enqueue\n");
		return false;
	}
	q->buffer[q->last] = malloc(q->size); 
	memcpy(q->buffer[q->last], a, q->size);
	q->last = (q->last+1) % q->max;
	q->count++; 
	return true; 
}

bool q_dequeue(queue *q, void *a)
{
	if (q_isempty(q)) 
		return false; 
	memcpy(a, q->buffer[q->first], q->size);
	free(q->buffer[q->first]); 
	q->buffer[q->first] = NULL; 

	q->first = (q->first+1) % q->max;
	q->count--; 
	return true;
}

void q_init(queue *q, int size_of_item, int max_count)
{
	q->first = 0;
	q->last = 0;
	q->count = 0;
	q->size = size_of_item;
	q->max = max_count; 
	q->buffer = (void **) malloc(sizeof(void *) * q->max); 
	for (int i=0; i<q->max; i++)
		q->buffer[i] = NULL; 
}

void q_free(queue *q)
{
	for (int i=0; i<q->count; i++) {
		free(q->buffer[q->first]); 
		q->first = (q->first+1) % q->max;
	}
	free(q->buffer); 
}

bool q_isempty(queue *q)
{
	return q->count == 0;
}

