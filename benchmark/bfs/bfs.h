#pragma once

#include "../edge_struct.h"	/*	packed_edge	*/

/*	=============== Functions ===============	*/

int create_graph_from_edgelist (struct packed_edge*, int64_t);
int make_bfs_tree (int64_t*, int64_t*, int64_t);
void destroy_graph (void);

#pragma once
