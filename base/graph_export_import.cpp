/*
  graph_export_import.cpp
  base function for "graph" abstract data type
  by Dongsoo S. Kim
  Copyright(c) 2013 by Dongsoo S. Kim
  
  Permission is granted for use in non-commerical applications
  provided this copyright notice remains intact and unchanged.
*/

#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include "graph.h"

int export_gml_graph(FILE *fp, graph *g)
{
	node u; 
	edge e; 

	fprintf(fp, "graph [\n");
	fprintf(fp, "directed %d\n", (is_directed(g) ? 1 : 0)); 
	forall_nodes(u, g)
		fprintf(fp, "node [ id %d ]\n", u);

	forall_edges(e, g)
		fprintf(fp, "edge [ source %d target %d ]\n", e.source, e.target);

	fprintf(fp, "]\n"); 
	return 0; 
}

int export_graph(const char *filename, graph *g, char *type)
{
	FILE *file = fopen(filename, "w");

	if (strcmp(type, "gml") == 0)
		export_gml_graph(file, g); 

	fclose(file); 
	return 0; 
}

int import_graph(const char *filename, graph *g, char *type)
{
	return 0; 
}

