/*	
	graph-alg.h
	Header file for graph algorithms
	by Dongsoo S. Kim
	Copyright(c) 2014 by Dongsoo S. Kim

	Permission is granted for use in non-commerical applications
	provided this copyright notice remains intact and unchanged.
*/

#ifndef GRAPH_ALG_H
#define GRAPH_ALG_H
#include "graph.h"

bool is_connected(graph *g); 
int	 distance(graph *g, node start, node finish);			// hop distance, negative if no path
int	 diameter(graph *g); 
bool has_cycle(graph *g); 
int	 toposort(graph *g, node *sorted); 
void shortest_path(graph *g, node start, node *parent, double (*cost)(graph *g, edge e)); 
void min_span_tree(graph *g, edge *tree, double(*cost)(graph *g, edge e));
void dominating_set(graph *g, node *dom, node start_with = -1);
void independent_set(graph *g, node *ind, node start_with = -1); 
void eulerian_circuit(graph *g, edge *euler, node start_with = -1); 
void hamiltonian_circuit(graph *g, edge *ham, node start_with = -1);
void travel_sales_man(graph *g, edge *tsp, double (*cost)(graph *g, edge e)); 


#endif