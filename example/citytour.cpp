/*	
	citytour.cc
	demonstration program for "graph" abstract data type
	by Dongsoo S. Kim
	Copyright(c) 2014 by Dongsoo S. Kim

	Permission is granted for use in non-commerical applications
	provided this copyright notice remains intact and unchanged.
*/

#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "graph.h"
#define MAXLEN  128

typedef char CityName[MAXLEN]; 

bool read_cityname(graph *g, node u, char *b)
{
  CityName cn; 
  strcpy(cn, b);
  set_node_attr(g, u, cn, sizeof(CityName));
  return true; 
}

bool write_cityname(graph *g, node u, char *b)
{
  CityName cn;
  if (get_node_attr(g, u, &cn, sizeof(CityName)))
    sprintf(b, "%s", cn);
  return true; 
}


bool read_distance(graph *g, edge e, char *b)
{
  double w; 
  b = strtok(b, " "); 
  if (b == NULL)
    return false;
  w = atof(b); 
  set_edge_attr(g, e, &w, sizeof(double)); 
  return true; 
}

bool write_distance(graph *g, edge e, char *b)
{
  double w; 
  if (get_edge_attr(g, e, &w, sizeof(double)))
    sprintf(b, "%.2lf", w); 
  return true; 
}

double get_distance(graph *g, edge e)
{
  double w; 
  if (get_edge_attr(g, e, &w, sizeof(double)))
    return w; 
  return DBL_MAX;
}

#define FILENAME "../../data/italy.gl"

int mst_kruskal(graph *g, edge *elist, double (*cost)(graph *g, edge e))
{
  edge *edges;
  edge e; 
  int  k; 

  edges = (edge *) malloc(sizeof(edge) * number_of_edges(g));
  k = 0; 
  forall_edges(e,g) {
    edges[k++] = e;
  }
  
  for (k=0; k<number_of_edges(g); k++) {
    if (k % 6 == 0) 
      printf("\n");
    printf("%f ", get_distance(g,edges[k]));
  }
  printf("\n");
  return 0; 

}

void city_tour()
{
  graph h; 
  edge *elist; 
  int nedge; 
  init_graph(&h); 
  h.read_node_attr = read_cityname;
  h.write_node_attr = write_cityname;
  h.read_edge_attr = read_distance;
  h.write_edge_attr = write_distance;

  if (read_graph(FILENAME, &h)) {
	print_graph(stdout, &h); 
	elist = (edge *) malloc(sizeof(edge) * (number_of_nodes(&h) - 1));
	nedge = mst_kruskal(&h, elist, get_distance);
  }
}

