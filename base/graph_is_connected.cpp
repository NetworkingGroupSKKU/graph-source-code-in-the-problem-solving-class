/*	
	graph_is_connected.cpp
	test if a graph is connected or not
	by Dongsoo S. Kim
	Copyright(c) 2013 by Dongsoo S. Kim

	Permission is granted for use in non-commerical applications
	provided this copyright notice remains intact and unchanged.
*/

#include <stdlib.h>
#include "graph_alg.h"
#include "dslib.h"

void save_visibles(graph *g, node *nodes, edge *edges)
{
	int i; 
	node n; 
	edge e; 

	i = 0; 
	forall_nodes(n, g)
		nodes[i++] = n; 
	nodes[i] = -1; 

	i = 0; 
	forall_edges(e, g) 
		edges[i++] = e; 
	edges[i].source = -1; 
}

void show_visibles(graph *g, node *nodes, edge *edges)
{
	int i; 
	for (i=0; nodes[i] > 0; i++)
		show_node(g, nodes[i]);
	for (i=0; edges[i].source > 0; i++) 
		show_edge(g, edges[i]);
}

bool is_connected(graph *g)
{
	node u; 
	queue que; 
	int n; 
	bool ret; 

	node *visible_nodes;
	edge *visible_edges; 
	
	for (u=0; u< g->max_vertices, !exist_node(g,u); u++)
		;

	if (!exist_node(g,u))
		return false; 

	visible_nodes = (node *) malloc(sizeof(node) * (number_of_nodes(g) + 1));
	visible_edges = (edge *) malloc(sizeof(edge) * (number_of_edges(g) * (is_directed(g) ? 1 : 2) + 1)); 

	save_visibles(g, visible_nodes, visible_edges); 

	// now, u is the first valid node in the graph
	q_init(&que, sizeof(node), number_of_edges(g)*2);

	q_enqueue(&que, &u); 

	while (!q_isempty(&que)) {
		if (q_dequeue(&que, &u) && !is_hidden_node(g, u)) { 
			forall_adj_nodes(n, u, g) {
				q_enqueue(&que, &n);
			}
			hide_node(g, u);
		}
	}

	ret = number_of_nodes(g) == 0;

	show_visibles(g, visible_nodes, visible_edges); 

	q_free(&que); 
	free(visible_nodes);
	free(visible_edges); 

	return ret; 
}