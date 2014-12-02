/*	
graph.h
Header file for "graph" abstract data type
by Dongsoo S. Kim
Copyright(c) 2013 by Dongsoo S. Kim

Permission is granted for use in non-commerical applications
provided this copyright notice remains intact and unchanged.
*/

#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>

#define DEFAULT_MAXV		100
#define DEFAULT_MAXDEGREE	50

typedef unsigned int node;

struct edge {
	node source;
	node target;
	edge(node s=0, node t=0)	{source = s; target=t;}
}; 

struct graph {
	bool	directed; 
	unsigned int max_vertices; 
	unsigned int max_degrees; 
	int 	*nodes; 
	node 	**edges;
	void **node_attr; 
	void ***edge_attr; 
	bool (*read_node_attr)(graph *g, node u, char *b);
	bool (*read_edge_attr)(graph *g, edge e, char *b);
	bool (*write_node_attr)(graph *g, node u, char *b);
	bool (*write_edge_attr)(graph *g, edge e, char *b);
}; 

// graph-base.cpp or graph-base.obj
// functions for the entire graph
void init_graph(graph *g, int cnt_nodes=DEFAULT_MAXV, int cnt_degrees=DEFAULT_MAXDEGREE);
void delete_graph(graph *g); 

void scan_graph(FILE *fp, graph *g);
void print_graph(FILE *fp, graph *g, bool data=false);

int read_graph(const char *filename, graph *g);
int write_graph(const char *filename, graph *g);

void make_directed(graph *g);
void make_undirected(graph *g);
bool is_directed(graph *g); 

// functions for a node
bool exist_node(graph *g, node u); 
void insert_node(graph *g, node u); 
void delete_node(graph *g, node u); 
void hide_node(graph *g, node u);
void show_node(graph *g, node u); 
void show_all_nodes(graph *g); 
bool is_hidden_node(graph *g, node u); 
bool set_node_attr(graph *g, node u, void *attr, int size); 
bool get_node_attr(graph *g, node u, void *attr, int size); 
int number_of_nodes(graph *g);
int adj_nodes(graph *g, node u, node *adj); 

// function for an edge by giving either a pair of nodes or an edge struct
bool exist_edge(graph *g, edge e); 
void insert_edge(graph *g, edge e);
void delete_edge(graph *g, edge e);
void hide_edge(graph *g, edge e); 
void show_edge(graph *g, edge e); 
void show_all_edges(graph *g); 
bool is_hidden_edge(graph *g, edge e); 
bool set_edge_attr(graph *g, edge e, void *attr, int size); 
bool get_edge_attr(graph *g, edge e, void *attr, int size); 
int number_of_edges(graph *g);

bool is_visible(int n);
bool is_valid(int n);

// macros for convenience to handle an iteration
#define forall_nodes(n,g)	\
	for((n)=0; (n) < (g)->max_vertices; (n)++) if(exist_node(g,n)) 

#define forall_adj_nodes(n,u,g)	\
	if (exist_node(g,u))	\
		for (int lidx=0; lidx < (g)->nodes[u], is_valid((n)=(g)->edges[u][lidx]); lidx++)	\
			if (is_visible(n)) 

#define forall_edges(e,g)	\
	for (node lnode=0; lnode < ((g)->max_vertices); lnode++)		\
		if(exist_node(g,  ((e).source = lnode)))		\
			for (int lidx=0; lidx < (g)->nodes[lnode], is_valid((e).target=(g)->edges[lnode][lidx]); lidx++)		\
				if (is_visible(e.target) && (is_directed(g) || e.source < e.target)) 

#define forall_out_edges(e,n,g) \
	if (exist_node(g,((e).source = n)))			\
		for (int lidx=0; lidx < (g)->nodes[n]; lidx++)	\
			if (is_visible((e).target = (g)->edges[n][lidx])) 

#define forall_in_edges(e,n,g)	\
	for (int lnode=0; lnode<(g)->max_vertices; lnode++)  \
		for (int ledge=0; ledge<(g)->nodes[lnode]; ledge++)			\
			if (is_visible((e).target=(g)->edges[(e).source=lnode][ledge]) && (e).target == (n))

#endif
