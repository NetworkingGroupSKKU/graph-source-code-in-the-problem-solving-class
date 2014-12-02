#ifndef EXAMPLE_H
#define EXAMPLE_H

#include "graph.h"

struct pos {
	double x, y;
}; 

bool read_position(graph *g, node u, char *b); 
bool write_position(graph *g, node u, char *b); 
bool read_weight(graph *g, edge e, char *b); 
bool write_weight(graph *g, edge e, char *b); 

int wait_for_key(); 

void check_node(graph *g, node u); 
void check_edge(graph *g, edge e); 

void city_tour(); 
void test_connection();
#endif