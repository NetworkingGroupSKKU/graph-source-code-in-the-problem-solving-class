#include <stdio.h>
#include "graph.h"
#include "example.h"

void example01_io()
{
	printf("- Example for basic graph functions\n"); 
    graph g; 
    init_graph(&g); 
    insert_node(&g, 0); 
    insert_node(&g, 1); 
    insert_node(&g, 2); 
    insert_edge(&g, edge(0, 1)); 
    insert_edge(&g, edge(1, 2)); 
    print_graph(stdout, &g); 
	delete_graph(&g); 
    
	printf("* Example for basic graph IO\n"); 

	char *filename = "..\\data\\snet-10-20.gl"; 
    init_graph(&g); 		
	if (read_graph(filename, &g) > 0) { 
		printf("** reading a file [%s]\n", filename); 
		print_graph(stdout, &g); 
		printf("** insert_edge(&g, 0, 3);\ninsert_node(&g, 10); \ninsert_node(&g, 11); \ninsert_edge(&g, 10, 11); \ndelete_edge(&g, 1, 3);\ndelete_node(&g, 7);\n");
		insert_edge(&g, edge(0, 3)); 
		insert_node(&g, 10); 
		insert_node(&g, 11); 
		insert_edge(&g, edge(10, 11)); 
		delete_edge(&g, edge(1, 3));
		delete_node(&g, 7); 
		printf("** after inserting and deleting\n"); 
		print_graph(stdout, &g); 
	}
	delete_graph(&g); 
}


void example02_unformatted_io()
{
	printf("* Example for basic graph unformatted IO\n"); 
	graph g; 
	init_graph(&g);
	scan_graph(stdin, &g);
	print_graph(stdout, &g);
	delete_graph(&g);
}

void example01_macros()
{
	graph g; 
	init_graph(&g); 
	read_graph("..\\data\\snet-10-20.gl", &g); 
	printf("Graph 1\n");
	insert_edge(&g, edge(0, 3)); 
	insert_node(&g, 10); 
	insert_node(&g, 11); 
	insert_edge(&g, edge(10, 11)); 
	delete_edge(&g, edge(1, 3));
	delete_node(&g, 7); 
	printf("Graph 2\n");
	print_graph(stdout, &g); 

	edge e; 
	node u = 3; 
	node n; 


	printf("All edges\n");
	forall_edges(e,&g) 
		printf("(%d,%d)\n", e.source, e.target); 

	printf("All adjacent nodes of 3\n");
	forall_adj_nodes(n,u,&g)
		printf("%d\n", n);

	printf("node %d is %s\n", u, is_hidden_node(&g, u)? "hidden":"visible"); 
	hide_node(&g, u);
	printf("node %d is %s\n", u, is_hidden_node(&g, u)? "hidden":"visible"); 

	delete_graph(&g);
}


void test_basic()
{
	graph g; 
	init_graph(&g); 
	char *filename = "..\\data\\snet-10-20.gl"; 

	read_graph(filename, &g); 
	printf("-------- Example for basic graph functions --------\n"); 
	printf("reading a file [%s]\n", filename); 
	print_graph(stdout, &g); 
	printf("insert_edge(&g, 0, 3);\ninsert_node(&g, 10); \ninsert_node(&g, 11); \ninsert_edge(&g, 10, 11); \ndelete_edge(&g, 1, 3);\ndelete_node(&g, 7);\n");
	insert_edge(&g, edge(0, 3)); 
	insert_node(&g, 10); 
	insert_node(&g, 11); 
	insert_edge(&g, edge(10, 11)); 
	delete_edge(&g, edge(1, 3));
	delete_node(&g, 7); 
	printf("------- after inserting and deleting\n"); 
	print_graph(stdout, &g); 

	edge e; 
	node u = 3; 
	node n; 
	printf("- forall_edges()\n");
	forall_edges(e,&g) 
		printf("(%d,%d)\n", e.source, e.target); 

	printf("- forall_adj_nodes(n,%d,g)\n", u);
	forall_adj_nodes(n,u,&g)
		printf("%d ", n);
	printf("\n"); 

	printf("- Scan the graph.\nEnter two numbers (nodes) for each line.\nHit the ENTER key to finish. \n");
	scan_graph(stdin, &g);
	print_graph(stdout, &g); 
	printf("- adj nodes of 3 are ");
	forall_adj_nodes(n, 3, &g) {
		printf("%d ", n); 
	}

	printf("\n- forall nodes are "); 
	forall_nodes(n, &g) {
		printf("%d ", n); 
	}

	printf("\n- forall edges are "); 

	forall_edges(e, &g) {
		printf("(%d,%d) ", e.source, e.target); 
	}
	printf("\n"); 
}

#define HLINE "============================="


struct test_suites {
	void (*func)();
	char *name; 
};

test_suites suites[] = {
//	{example01_io,				"example01_io"}, 
//	{example02_unformatted_io,  "example02_unformatted_io"},
//	{example01_macros,			"example01_macros"},
//	{test_basic,				"test_basic"}, 
//	{city_tour,					"city_tour"},
	{test_connection,			"test_connection"}, 
	{NULL, NULL}
};


int main(int argc, char *argv[])
{
	for (int i = 0; suites[i].func != NULL; i++) {
		printf("%s %s %s\n", HLINE, suites[i].name, HLINE); 
		suites[i].func(); 
	};
}

