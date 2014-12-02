#include "graph_alg.h"
#include <stdlib.h>
#include <float.h>

void shortest_pathx(graph *g, node start, node *parent, double (*cost)(graph *g, edge e))
{
	bool   *intree;	/* is vertex in the tree yet? */
	double *distance; /* vertex distance from start */
	node	u;			/* current vertex to process */
	edge	e;
	double	weight;			/* edge weight */
	double  dist;			/* shortest current distance */

	intree = (bool *) malloc(sizeof(bool) * g->max_vertices);
	distance = (double *) malloc(sizeof(double) * g->max_vertices);

	for (u=0; u < g->max_vertices; u++) {
		intree[u] = false; 
		distance[u] = DBL_MAX;
		parent[u] = -1;
	}

	distance[start] = 0.0;

	while (intree[start] == false) {
		intree[start] = true;
		forall_out_edges(e, start, g) {
			weight = cost(g, e);
			if (distance[e.target] > (distance[start]+weight)) {
				distance[e.target] = distance[start]+weight;
				parent[e.target] = start;
			}
		}

		dist = DBL_MAX;
		forall_nodes(u,g)
			if ((intree[u]==false) && (dist > distance[u])) {
				dist = distance[u];
				start = u;
			}
	}

	free(intree);
	free(distance); 
}