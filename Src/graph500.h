#ifndef GRAPH500_HEADER_
#define GRAPH500_HEADER_

#define NAME "Graph500 sequential list"
#define VERSION 0

#include "generator/graph_generator.h"

int create_graph_from_edgelist (struct packed_edge*, int64_t);
int make_bfs_tree (int64_t*, int64_t*, int64_t);
void destroy_graph (void);

#endif /*GRAPH500_HEADER_*/
