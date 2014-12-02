/*	
	test-digraph.cpp
	demonstration program for "graph" abstract data type
	by Dongsoo S. Kim
	Copyright(c) 2013 by Dongsoo S. Kim

	Permission is granted for use in non-commerical applications
	provided this copyright notice remains intact and unchanged.
*/

#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "graph.h"

int digraph_main(int argc, char *argv[])
{
	graph g; 
	init_graph(&g); 
	make_directed(&g);
	insert_edge(&g, edge(0,1)); 
	insert_edge(&g, edge(0,2)); 
	insert_edge(&g, edge(1,2));
	insert_edge(&g, edge(1,3));
	insert_edge(&g, edge(2,3));

	print_graph(stdout, &g); 
	printf("-------------\n");

	edge e;
	forall_out_edges(e,0,&g) {
	  printf("%d -> %d\n", e.source, e.target);
	}

	printf("-------------\n");
	forall_in_edges(e,2,&g) {
	  printf("%d -> %d\n", e.source, e.target);
	}
	return 0;
}
