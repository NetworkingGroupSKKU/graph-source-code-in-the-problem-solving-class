/*
graph-base.cpp
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

#define MAXBUFFER	1024
#define NOT_A_NODE	0xFFFFFFFF
#define INVISIBLE	0x80000000
#define MAKE_INVISIBLE(n)	((n) |= INVISIBLE)
#define MAKE_VISIBLE(n)		((n) &= ~INVISIBLE)
#define IS_VISIBLE(n)		(((n) & INVISIBLE) != INVISIBLE)		// valid node id but not a hidden 
#define IS_INVISIBLE(n)		((n) < 0 && (n) != NOT_A_NODE)
#define NEDGES(n)			((g->nodes[n] == NOT_A_NODE) ? 0 : g->nodes[n] &= ~INVISIBLE)
#define SEPARATORS	" \t"	

using namespace std; 

#define DEBUG	8

// prototype for local functions
int degree(graph *g, node x); 
void insert_the_edge(graph *g, node x, node y);
void delete_the_edge(graph *g, node x, node y);
void hide_the_edge(graph *g, node u, node v); 
void show_the_edge(graph *g, node u, node v);
void set_the_edge_attr(graph *g, node u, node v, void *attr, int size);
int  edge_locate(graph *g, node u, node v); 


bool is_valid_node(graph *g, node u)
{
	return u < g->max_vertices && g->nodes[u] != NOT_A_NODE; 
} 



// functions for the entire graph
void init_graph(graph *g, int cnt_nodes, int cnt_degrees)
{
#if DEBUG >= 9
	printf("init_graph(graph)\n");
#endif
	assert(cnt_nodes < 0xEFFFFFFF); 
	node u; 
	g->max_vertices = cnt_nodes;
	g->max_degrees  = cnt_degrees; 
	g->directed = false; 
	g->read_node_attr = NULL; 
	g->read_edge_attr = NULL;
	g->write_node_attr = NULL;
	g->write_edge_attr = NULL;

	g->nodes = (int *) malloc(sizeof(int)*g->max_vertices);
	node *ep = (node *) malloc(sizeof(node) * g->max_vertices * g->max_degrees);
	g->edges = (node **) malloc(sizeof(node *) * g->max_vertices);
	for (u=0; u < (g->max_vertices); u++)
		g->edges[u] = &ep[u * g->max_degrees]; 

	g->node_attr = (void **) malloc(sizeof(void *) * g->max_vertices);
	void **ap = (void **) malloc(sizeof(void *) * g->max_vertices * g->max_degrees);
	g->edge_attr = (void ***) malloc(sizeof(void **) * g->max_vertices);
	for (u=0; u < (g->max_vertices); u++) 
		g->edge_attr[u] = &ap[u * g->max_degrees];

	for (u=0; u < (g->max_vertices); u++) {
		g->nodes[u] = NOT_A_NODE; 
		g->node_attr[u] = NULL; 
		for (unsigned int j=0; j < (g->max_degrees); j++) {
			g->edges[u][j] = NOT_A_NODE; 
			g->edge_attr[u][j] = NULL; 
		}
	}

}

void delete_graph(graph *g)
{
#if DEBUG >= 9
	printf("delete_graph(graph)\n");
#endif

	// to deallocate customized node attributes
	for (node u=0; u < (g->max_vertices); u++) {
		if (g->node_attr[u] != NULL) 
			free(g->node_attr[u]);
	}
	// to deallocate customized edge attributes
	for (node u=0; u < (g->max_vertices); u++) {
		for (unsigned int j=0; j < (g->max_degrees); j++) {
			if (g->edge_attr[u][j] != NULL) 
				free(g->edge_attr[u][j]); 
		}
	}

	free(g->edge_attr[0]);
	free(g->edge_attr);
	free(g->node_attr);

	free(g->edges[0]);
	free(g->edges);
	free(g->nodes);
}

#define PROMPT "GRAPH> "

void scan_graph(FILE *fp, graph *g)
{
#if DEBUG >= 9
	printf("scan_graph(FILE, graph)\n");
#endif
	node x, y;			/* vertices in edge (x,y) */
	char buffer[MAXBUFFER];
	printf("%s", PROMPT); 
	fgets(buffer, MAXBUFFER-1, fp); 
	while (sscanf(buffer, "%d %d", &x,&y) != EOF) {
		insert_edge(g,edge(x,y));
		printf("%s", PROMPT); 
		fgets(buffer, MAXBUFFER-1, fp); 
	}
}

void print_graph(FILE *fp, graph *g, bool data)
{
#if DEBUG >= 9
	printf("print_graph(FILE, graph)\n");
#endif
	char buffer[MAXBUFFER];
	node n; 
	int v; 
	bool first; 
	forall_nodes(n,g) {
		fprintf(fp, "%d",n);
		if (data && g->write_node_attr != NULL)  {
			strcpy(buffer, "");
			g->write_node_attr(g, n, buffer);
			fprintf(fp, "(%s) ", buffer); 
		}
		fprintf(fp, " %s ", (g->directed ? "->" : "--"));
		first = true; 
		forall_adj_nodes(v,n,g) {
			if (!first) fprintf(fp, ",");
			first = false; 
			fprintf(fp, "%d", v);
			if (data && g->write_edge_attr != NULL)  {
				strcpy(buffer, "");
				g->write_edge_attr(g, edge(n, v), buffer);
				fprintf(fp, "(%s)", buffer); 
			}
		}
		fprintf(fp, "\n");
	}
}

int read_graph(const char *filename, graph *g)
{
#if DEBUG >= 9
	printf("read_graph(\"%s\", graph)\n", filename);
#endif
	FILE *fp;
	char buffer[MAXBUFFER];
	char *buf, *bp;
	node u, v;
	int nlines; 

	fp = fopen(filename, "r"); 
	if (fp == NULL) {
		fprintf(stderr, "File not found <%s>\n", filename); 
		return 0;
	}

	nlines = 0; 

	while (fgets(buffer, MAXBUFFER-1, fp) != NULL) {
		nlines++; 

		buf = buffer;
		while (isspace(*buf)) buf++;	// left time
		if (strlen(buf) == 0)			// skip blank line
			continue; 
		bp = buffer+strlen(buffer)-1;
		while (isspace(*bp) || *bp == '\n' || *bp == '\r' ) {			// right trim
			*bp = '\0';
			bp--;
		}

		bp = strtok(buf, SEPARATORS); 
		switch (toupper(*bp)) {
		case 'D': 
			bp = strtok(NULL, SEPARATORS); 
			if (atoi(bp))
				make_directed(g);
			break; 
		case 'N':
			bp = strtok(NULL, SEPARATORS); 
			u = atoi(bp);
			insert_node(g, u);
			if (g->read_node_attr != NULL) 
				g->read_node_attr(g, u, bp+strlen(bp)+1); 
			break;
		case 'E': 
			bp = strtok(NULL, SEPARATORS); u = atoi(bp);
			bp = strtok(NULL, SEPARATORS); v = atoi(bp);
			insert_edge(g, edge(u, v)); 
			if (g->read_edge_attr != NULL) 
				g->read_edge_attr(g, edge(u,v), bp+strlen(bp)+1); 
			break; 
		case '#':
			// comments.  ignore the line
			break; 
		}
	}

	fclose(fp); 
	return nlines; 
}

int write_graph(const char *filename, graph *g)
{
#if DEBUG >= 9
	printf("write_graph(\"%s\", graph)\n", filename);
#endif
	FILE *fp;
	int cnt; 
	node u, v; 
	edge e; 
	char buffer[MAXBUFFER]; 

	fp = fopen(filename, "w"); 
	if (fp == NULL) {
		fprintf(stderr, "File not found <%s>\n", filename); 
		return 0;
	}

	cnt = 1; 
	fprintf(fp, "D %c\n", (g->directed ? '1' : '0'));
	forall_nodes(u,g) {
		fprintf(fp, "N %d", u);
		if (g->write_node_attr!=NULL) {
			strcpy(buffer, "");
			g->write_node_attr(g, u, buffer); 
			fprintf(fp, " %s", buffer);
		}
		fprintf(fp, "\n"); 
		cnt++;
	}

	forall_edges(e,g) {
		fprintf(fp, "E %d %d", e.source, e.target); 
		if (g->write_edge_attr!=NULL) {
			strcpy(buffer, "");
			g->write_edge_attr(g, e, buffer); 
			fprintf(fp, " %s", buffer);
		}
		fprintf(fp, "\n"); 
		cnt++;
	}

	fclose(fp);
	return cnt; 
}

void make_directed(graph *g)
{
#if DEBUG >= 9
	printf("make_directed(graph)\n");
#endif
	g->directed = true; 
}

void make_undirected(graph *g)
{
#if DEBUG >= 9
	printf("make_undirected(graph)\n");
#endif
	g->directed = false; 
}

bool is_directed(graph *g)
{
#if DEBUG >= 9
	printf("is_directed(graph)\n");
#endif
	return g->directed;
}

// functions for a node
int adj_nodes(graph *g, node u, node *adj)
{
	int cnt; 
	node v; 

	if (!exist_node(g,u))
		return 0; 
	cnt = 0; 
	forall_adj_nodes(v,u,g)
		adj[cnt++] = v;

	return cnt; 
}

bool exist_node(graph *g, node u)
{
#if DEBUG >= 9
	printf("exist_node(graph, node)\n");
#endif
	return u < g->max_vertices && IS_VISIBLE(g->nodes[u]); 
}

void insert_node(graph *g, node u)
{
#if DEBUG >= 9
	printf("insert_node(graph,%d)\n", u);
#endif

	if (u >= g->max_vertices) {
		fprintf(stderr, "Warning: insert_node(%d) exceeds max vertices\n", u);
		return; 
	}	
	if (!exist_node(g,u)) {
		g->nodes[u] = 0; 
	}
}

void delete_node(graph *g, node u)
{
	// delete a visible node only
#if DEBUG >= 9
	printf("delete_node(graph,%d)\n", u);
#endif

	unsigned int i, k; 

	if (!exist_node(g,u))
		return; 

	forall_nodes(i,g) 
		delete_the_edge(g, i, u); 

	for (k=0; k<g->max_degrees; k++)  {
		if (g->edge_attr[u][k] != NULL) 
			free(g->edge_attr[u][k]); 
		g->edge_attr[u][k] = NULL; 
		g->edges[u][k] = NOT_A_NODE; 
	}

	if (g->node_attr[u])
		free(g->node_attr[u]);
	g->node_attr[u] = NULL;
	g->nodes[u] = NOT_A_NODE; 
}

void hide_node(graph *g, node u)
{
	edge e; 
	node v; 

	if (g->nodes[u] < 0)		// the node is either invalid or hidden already
		return; 

	forall_adj_nodes(v, u, g) 
		hide_edge(g, edge(u,v)); 

	MAKE_INVISIBLE(g->nodes[u]);
}

void show_node(graph *g, node u)
{
	if (IS_INVISIBLE(g->nodes[u]))
		MAKE_VISIBLE(g->nodes[u]); 
}

void show_all_nodes(graph *g)
{
	node u; 
	for (u=0; u<g->max_vertices; u++) {
		if (IS_INVISIBLE(g->nodes[u]))
			MAKE_VISIBLE(g->nodes[u]); 
	}

}

bool is_hidden_node(graph *g, node u)
{
	if (!is_valid_node(g,u))
		return false; 
	return !IS_VISIBLE(g->nodes[u]); 
}

bool set_node_attr(graph *g, node u, void *attr, int size)
{
	if (exist_node(g,u)) {
		if (g->node_attr[u] == NULL)
			g->node_attr[u] = malloc(size); 
		memcpy(g->node_attr[u], attr, size); 
		return true; 
	}
	return false; 
}

bool get_node_attr(graph *g, node u, void *attr, int size)
{
	if (exist_node(g,u) && g->node_attr[u] != NULL) {
		memcpy(attr, g->node_attr[u], size); 
		return true; 
	}
	return false; 
}

int number_of_nodes(graph *g)
{
	int cnt;
	node u; 
	cnt = 0; 
	forall_nodes(u,g) 
		cnt++; 
	return cnt; 
}

// function for an edge
bool exist_edge(graph *g, edge e)
{
#if DEBUG >= 9
	printf("exist_edge(graph, edge)\n");
#endif
	node n; 
	if (exist_node(g, e.source) && exist_node(g, e.target)) {
		forall_adj_nodes(n, e.source ,g) {
			if (n == e.target)
				return true; 
		}
	}
	return false; 
}

void insert_edge(graph *g, edge e)
{
#if DEBUG >= 9
	printf("insert_edge(graph,%d,%d)\n", x, y);
#endif

	insert_node(g, e.source);
	insert_node(g, e.target); 

	insert_the_edge(g, e.source, e.target); 
	if (!g->directed)
		insert_the_edge(g, e.target, e.source); 
}

void delete_edge(graph *g, edge e)
{
#if DEBUG >= 9
	printf("delete_edge(graph,%d,%d)\n", e.source, e.target);
#endif
	delete_the_edge(g, e.source, e.target); 
	if (!g->directed)
		delete_the_edge(g, e.target, e.source); 
}

void hide_edge(graph *g, edge e)
{
	hide_the_edge(g, e.source, e.target); 
	if (!g->directed)
		hide_the_edge(g, e.target, e.source); 
}

void show_edge(graph *g, edge e)
{
	show_the_edge(g, e.source, e.target); 
	if (!g->directed)
		show_the_edge(g, e.target, e.source); 
}

void show_all_edges(graph *g)
{
	int j; 
	node u; 
	show_all_nodes(g); 
	forall_nodes(u,g) {
		for (j=0; j<NEDGES(u), g->edges[u][j] != NOT_A_NODE; j++)
			MAKE_VISIBLE(g->edges[u][j]); 
	}
}

bool is_hidden_edge(graph *g, edge e)
{
	int i; 

	if (!is_valid_node(g,e.source) || !is_valid_node(g,e.target))	// the nodes themselvs are not valid
		return false;

	if ((i = edge_locate(g, e.source, e.target)) < 0)			// the edge is not valid
		return false; 

	if (g->edges[e.source][i] == e.target)						// the edge is visible 
		return false;

	return true; 

}

bool set_edge_attr(graph *g, edge e, void *attr, int size)
{
	if (!exist_node(g, e.source) || !exist_node(g,e.target))
		return false; 
	set_the_edge_attr(g, e.source, e.target, attr, size); 
	if (!g->directed)
		set_the_edge_attr(g, e.target, e.source, attr, size); 
	return true; 
}

bool get_edge_attr(graph *g, edge e, void *attr, int size)
{
	int eloc;
	if (!exist_node(g, e.source) || !exist_node(g, e.target))
		return false; 

	eloc = edge_locate(g,e.source,e.target); 

	if (g->edge_attr[e.source][eloc]) {
		memcpy(attr, g->edge_attr[e.source][eloc], size); 
		return true;
	}

	return false;
}

int number_of_edges(graph *g)
{
	int cnt; 
	node u, v; 
	cnt = 0;
	forall_nodes(u,g) {
		forall_adj_nodes(v, u, g) {
			cnt++;
		}
	}
	if (!g->directed)
		cnt = cnt / 2; 
	return cnt; 
}

/*
bool exist_edge(graph *g, edge e)
{
	return exist_edge(g, e.source, e.target);
}

void insert_edge(graph *g, edge e)
{
	insert_edge(g, e.source, e.target); 
}

void delete_edge(graph *g, edge e)
{
	delete_edge(g, e.source, e.target); 
}

void hide_edge(graph *g, edge e)
{
	hide_edge(g, e.source, e.target); 
}

void show_edge(graph *g, edge e) 
{
	show_edge(g, e.source, e.target); 
}

bool is_hidden_edge(graph *g, edge e)
{
	return is_hidden_edge(g, e.source, e.target); 
}

bool set_edge_attr(graph *g, edge e, void *attr, int size)
{
	return set_edge_attr(g, e.source, e.target, attr, size);
}

bool get_edge_attr(graph *g, edge e, void *attr, int size)
{
	return get_edge_attr(g, e.source, e.target, attr, size);
}
*/

/*********************************************************************** 
local functions
***********************************************************************/
int degree(graph *g, node x) 
{
	int n; 
	node u; 
	if (!exist_node(g,x))
		return 0; 
	n = 0; 
	forall_adj_nodes(u, x, g)
		n++; 
	return n;
}

void insert_the_edge(graph *g, node x, node y)
{
	int nedges; 
	int eloc; 
	if (!exist_node(g, x) || !exist_node(g,y))
		return; 

	eloc = edge_locate(g,x,y); 
	if (eloc >= 0)		// already exist
		return; 
	nedges = NEDGES(x); 
	if (nedges==g->max_degrees) {
		fprintf(stderr, "Warning: insertion(%d,%d) exceeds max degree\n",x,y);
		return; 
	}
	g->edges[x][nedges] = y;
	g->nodes[x]++; 
}

void delete_the_edge(graph *g, node x, node y)
{
	int i;				/* counter */
	int deg; 

	if (!exist_node(g, x) || !exist_node(g,y))
		return; 

	deg = g->nodes[x]; 

	for (i=0; i<deg; i++) 
		while (g->edges[x][i] == y) {
			deg--; 
			g->nodes[x]--; 
			g->edges[x][i] = g->edges[x][deg];
			g->edges[x][deg] = NOT_A_NODE; 
			if (g->edge_attr[x][i])
				free(g->edge_attr[x][i]);
			g->edge_attr[x][i] = g->edge_attr[x][deg];
			g->edge_attr[x][deg] = NULL;
		}
}

void hide_the_edge(graph *g, node u, node v)
{
	if (!exist_node(g, u) || !exist_node(g,v))
		return; 
	int eloc; 
	eloc = edge_locate(g,u,v); 
	MAKE_INVISIBLE(g->edges[u][eloc]); 
}

void show_the_edge(graph *g, node u, node v)
{
	if (!exist_node(g, u) || !exist_node(g,v))
		return; 
	int eloc; 
	eloc = edge_locate(g,u, (v & ~INVISIBLE));
	MAKE_VISIBLE(g->edges[u][eloc]); 
}

void set_the_edge_attr(graph *g, node u, node v, void *attr, int size)
{
	int eloc; 
	if (!exist_node(g, u) || !exist_node(g,v))
		return; 
	eloc = edge_locate(g,u,v); 
	if (eloc >= 0) {
		if (g->edge_attr[u][eloc] == NULL)
			g->edge_attr[u][eloc] = malloc(size); 
		memcpy(g->edge_attr[u][eloc], attr, size); 
	}
}

int edge_locate(graph *g, node u, node v)
{
	if (!is_valid_node(g,u) || !is_valid_node(g,v))
		return -1; 

	for (unsigned int i=0; i<g->max_degrees; i++)
		if ((g->edges[u][i] & ~INVISIBLE) == v)
			return i;
	return -1; 
}

bool is_visible(int n)
{
	return IS_VISIBLE(n);
}

bool is_valid(int n)
{
	return n != NOT_A_NODE;
}
