#ifndef __BFS__
#define __BFS__

#include "../edge_struct.h"	/*	packed_edge	*/

int create_graph_from_edgelist (struct packed_edge*, int64_t);
int make_bfs_tree (int64_t*, int64_t*, int64_t);
void destroy_graph (void);

#endif
