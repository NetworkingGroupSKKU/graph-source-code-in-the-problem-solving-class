#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dslib.h"

void s_init(stack *s, int size_of_item, int max_cap)
{
	s->max = max_cap;
	s->size = size_of_item;
	s->count = 0; 
	s->buffer = new void*[s->max];
}

bool s_isempty(stack *s)
{
	return s->count == 0; 
}

bool s_push(stack *s, void *a)
{
	if (s->count == s->max) {
		fprintf(stderr, "stack overflow\n");
		return false;
	}
	s->buffer[s->count] = malloc(s->size); 
	memcpy(s->buffer[s->count], a, s->size); 
	s->count++; 
	return true; 
}

bool s_pop(stack *s, void *a)
{
	if (s_isempty(s)) 
		return false; 
	s->count--;
	memcpy(a, s->buffer[s->count], s->size); 
	return true; 
}
