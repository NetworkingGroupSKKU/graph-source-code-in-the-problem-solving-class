#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "dslib.h"
#include "graph_alg.h"

int toposort(graph *g, node sorted[])
{
	int *indegree;
	queue zeroin; 
	node u, x; 
	edge e; 
	int cnt; 

	// compute indegree for each node
	indegree = (int *) malloc(g->max_vertices*sizeof(int)); 

	forall_nodes(u,  g)
		indegree[u] = 0; 

	forall_edges(e,g)
		indegree[e.target]++;

	q_init(&zeroin, sizeof(int)); 

	forall_nodes(u,g)
		if (indegree[u] == 0)
			q_enqueue(&zeroin, &u); 

	cnt = 0; 
	while (!q_isempty(&zeroin)) {
		q_dequeue(&zeroin, &u);
		sorted[cnt++] = u; 
		forall_nodes(x, g) {
			if (exist_edge(g,edge(u,x))) {
				indegree[x]--; 
				if (indegree[x] == 0)
					q_enqueue(&zeroin, &x);
			}
		}
	}

	free(indegree); 
	if (cnt != number_of_nodes(g))
		printf("not a dag -- only %d vertices found\n", cnt); 
	return cnt; 
}

/*
void main_toposort()
{
	graph g; 
	init_graph(&g); 
	make_directed(&g);
	insert_edge(&g, edge(0,1));
	insert_edge(&g, edge(0,5));
	insert_edge(&g, edge(1,3));
	insert_edge(&g, edge(2,0));
	insert_edge(&g, edge(2,1));
	insert_edge(&g, edge(2,4));
	insert_edge(&g, edge(3,4));
	insert_edge(&g, edge(5,4));

	printf("------- Example for topology sort ---------\n"); 
	print_graph(stdout, &g); 
	node topsorted[MAXV]; 
	int cnt; 
	cnt = toposort(&g, topsorted);
	if (cnt > 0) {
		printf("%d", topsorted[0]);
		for (int i=1; i<cnt; i++) 
			printf(" -> %d", topsorted[i]);
	}
	printf("\n"); 
}

*/