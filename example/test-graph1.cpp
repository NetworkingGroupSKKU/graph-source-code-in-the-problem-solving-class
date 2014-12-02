// graph-test.cpp
// dskim61@skku.edu
//
#include <direct.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include "graph.h"
#include "graph_alg.h"
#include "dslib.h"
#include "example.h"

int test1(int argc, char *argv[])
{
	graph g; 
	FILE *fp;
	char *filename = "test.txt"; 

	fp = fopen(filename, "r"); 
	if (fp == NULL) {
		fprintf(stderr, "File not found <%s>\n", filename); 
		return 1;
	}
	scan_graph(fp, &g); 
	print_graph(stdout, &g); 
	printf("======\n"); 
	write_graph("test.gl", &g); 

	delete_edge(&g, edge(2, 5));
	print_graph(stdout, &g); 
	printf("======\n"); 

	make_directed(&g); 
	print_graph(stdout, &g); 
	printf("======\n"); 

	delete_node(&g, 5); 
	print_graph(stdout, &g); 
	printf("======\n"); 

	return 1; 
}

int test2(int argc, char *argv[])
{
	graph g; 
	read_graph("test.gl", &g); 
	print_graph(stdout, &g); 
	printf("graph has %d nodes and %d edges\n", number_of_nodes(&g), number_of_edges(&g));

	return 1; 
}

int generate_snet(int ncnt, int dist)
{
	const int nmax = 100; 
	struct pos {
		double x, y;
	};
	int i, j; 
	pos p[nmax]; 
	char filename[100]; 
	double cost; 
	sprintf(filename, "snet-%d-%d.gl", ncnt, dist); 
	FILE *fp = fopen(filename, "w");
	fprintf(fp, "D 0\n");
	for (i=0; i<ncnt; i++) {
		p[i].x = 100.0 * rand() / RAND_MAX; 
		p[i].y = 100.0 * rand() / RAND_MAX; 
		fprintf(fp, "N %d %.2lf %.2lf\n", i, p[i].x, p[i].y);
	}

	for (i=0; i<ncnt-1; i++)
		for (j=i+1; j<ncnt; j++) {
			cost = sqrt((p[i].x-p[j].x)*(p[i].x-p[j].x)+(p[i].y-p[j].y)*(p[i].y-p[j].y)); 
			if ( cost <= dist)
				fprintf(fp, "E %d %d %.2lf\n", i, j, cost); 
		}

	fclose(fp); 
	return 1; 
}

int test3_drive()
{
	int dist; 
	int n; 
	for (n=10; n<=30; n += 5)
		for (dist=20; dist<=60; dist += 10)
			generate_snet(n, dist);

	return 0; 
}

void connect_test(char *filename)
{
	graph g; 
	read_graph(filename, &g); 
	print_graph(stdout, &g);
	if (is_connected(&g))
		printf("Graph [%s] is connected\n", filename); 
	else 
		printf("Graph [%s] is not connected\n", filename); 
}

void queue_test()
{
	int x; 
	queue q;

	q_init(&q, sizeof(int)); 
	x = 3; 
	while (q_enqueue(&q, &x)) 
		x++; 

	while (!q_isempty(&q)) {
//		q_print(stdout, &q); 
		q_dequeue(&q, &x);
	}
}

void attr_test()
{
	graph g; 
	init_graph(&g); 
	g.read_edge_attr = read_weight; 
	g.write_edge_attr = write_weight;
	g.read_node_attr = read_position;
	g.write_node_attr = write_position;

	read_graph("snet-10-5.gl", &g);
	print_graph(stdout, &g);
	printf("=================\n"); 
	write_graph("output.txt", &g); 
}

void attr_test2()
{
	graph g; 
	double w;
	double weigth; 
	init_graph(&g);
	insert_edge(&g, edge(1, 2)); 
	w = 3.5; 
	set_edge_attr(&g, edge(1, 2), &w, sizeof(double)); 
	get_edge_attr(&g, edge(1, 2), &weigth, sizeof(double));

	print_graph(stdout, &g);
	printf("weight on 1 - 2 = %lf\n", w); 
}

int main_test1(int argc, char *argv[])
{
	// generate_snet(16, 30); 
	// test3_drive(); 
	// queue_test(); 	
	// connect_test("snet-10-5.gl"); 
	// connect_test("snet-10-10.gl"); 
	// attr_test();
	return 0;
}