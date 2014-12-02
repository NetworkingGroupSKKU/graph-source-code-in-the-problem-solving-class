/*	
	test-connection.cpp
	demonstration program for "graph" abstract data type
	by Dongsoo S. Kim
	Copyright(c) 2013 by Dongsoo S. Kim

	Permission is granted for use in non-commerical applications
	provided this copyright notice remains intact and unchanged.
*/

#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <stdio.h>
#include <time.h>
#include "graph.h"
#include "graph_alg.h"
#define MAXSTR		256

char *filename[] = {
	"snet-10-20.gl", "snet-10-30.gl", "snet-10-40.gl", "snet-10-50.gl", "snet-10-60.gl", 
	"snet-15-20.gl", "snet-15-30.gl", "snet-15-40.gl", "snet-15-50.gl", "snet-15-60.gl",
	"snet-20-20.gl", "snet-20-30.gl", "snet-20-40.gl", "snet-20-50.gl", "snet-20-60.gl", 
	"snet-25-20.gl", "snet-25-30.gl", "snet-25-40.gl", "snet-25-50.gl", "snet-25-60.gl",
	"snet-30-20.gl", "snet-30-30.gl", "snet-30-40.gl", "snet-30-50.gl", "snet-30-60.gl",
	NULL
}; 

void test_connection()
{
	graph g; 
	bool result; 
	char name[MAXSTR]; 
	clock_t start, finish; 
	double duration; 

	start = clock(); 
	for (int i=0; filename[i] != NULL; i++)  {
		sprintf(name, "../../data/%s", filename[i]);
		init_graph(&g);
		if (read_graph(name, &g) > 0) {
			printf("%s (%d nodes, %d edges) ", filename[i], number_of_nodes(&g), number_of_edges(&g)); 
			result = is_connected(&g);
			printf("is %sconnected\n", (result ? "" : "not ")); 
		}
	}
	finish = clock(); 
	duration = (double)(finish-start)/CLOCKS_PER_SEC;
	printf("%.6f seconds\n", duration); 
}
