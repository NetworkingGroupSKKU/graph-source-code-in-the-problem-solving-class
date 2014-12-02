/*	
	test-graph2.cpp
	demonstration program for "graph" abstract data type
	by Dongsoo S. Kim
	Copyright(c) 2013 by Dongsoo S. Kim

	Permission is granted for use in non-commerical applications
	provided this copyright notice remains intact and unchanged.
*/

#include <string.h>
#include <stdlib.h>
#include <float.h>

#include "example.h"
#include "graph.h"
#include "graph_alg.h"

double get_weight(graph *g, edge e)
{
	double w; 
	if (get_edge_attr(g, e, &w, sizeof(double)))
		return w; 
	return DBL_MAX;
}

int graph_test(int argc, char *argv[])
{
	char *filename; 
	if (argc < 2) {
		printf("usage: %s <filename>\n", argv[0]);
		return 1;
	}
	filename = argv[1]; 

/*	example 1
	simply read a text file (gl format)
	and print it on screen 
*/
	graph g; 
	init_graph(&g); 
	read_graph(filename, &g); 
	print_graph(stdout, &g); 
	wait_for_key(); 

/*  example 2
	read a gl file with attribute
*/
	graph h; 
	init_graph(&h); 
	h.read_node_attr = read_position; 
	h.write_node_attr = write_position; 
	h.read_edge_attr = read_weight; 
	h.write_edge_attr = write_weight; 
	read_graph(filename, &h); 
	print_graph(stdout, &h); 
	wait_for_key(); 

/*  example 3
	modify the graph
*/
	double weight; 
	insert_edge(&h, edge(0, 8));
	weight = 3.14; 
	set_edge_attr(&h, edge(0, 8), &weight, sizeof(double));
	insert_node(&h, 20);
	insert_edge(&h, edge(6, 20)); 
	weight = 2.78; 
	set_edge_attr(&h, edge(6, 20), &weight, sizeof(double));
	insert_edge(&h, edge(8, 20)); 
	delete_edge(&h, edge(2, 4)); 
	print_graph(stdout, &h); 
	write_graph("output.txt", &h); 
	if (is_directed(&h))
		printf("graph <h> is directed\n");
	else
		printf("graph <h> is not directed\n");
	printf("graph <h> has %d nodes and %d edges\n", number_of_nodes(&h), number_of_edges(&h));
	check_node(&h, 5); 
	check_edge(&h, edge(5, 6));
	check_edge(&h, edge(5, 9)); 
	printf("delete node 5\n");
	delete_node(&h, 5); 
	printf("graph <h> has %d nodes and %d edges\n", number_of_nodes(&h), number_of_edges(&h));
	check_node(&h, 5); 
	check_edge(&h, edge(5, 6));
	check_edge(&h, edge(5, 9)); 
	printf("hide node 3\n");
	hide_node(&h, 3); 
	printf("graph <h> has %d nodes and %d edges\n", number_of_nodes(&h), number_of_edges(&h));
	printf("node 3 is %s\n", (is_hidden_node(&h,3) ? "hidden" : "visible")); 
	printf("node 7 is %s\n", (is_hidden_node(&h,7) ? "hidden" : "visible")); 

	printf("hide edge <9,2>\n"); 
	hide_edge(&h, edge(9, 2)); 
	printf("graph <h> has %d nodes and %d edges\n", number_of_nodes(&h), number_of_edges(&h));
	printf("edge <9,2> is %s\n", (is_hidden_edge(&h,edge(9,2)) ? "hidden" : "visible")); 
	printf("edge <2,9> is %s\n", (is_hidden_edge(&h,edge(2,9)) ? "hidden" : "visible")); 
	print_graph(stdout, &h); 

	printf("show all edges\n"); 
	show_all_edges(&h); 
	printf("graph <h> has %d nodes and %d edges\n", number_of_nodes(&h), number_of_edges(&h));
	print_graph(stdout, &h); 
	return 0; 
}

int main_x(int argc, char *argv[])
{
	// graph_test(argc, argv);
	return 0; 
}
