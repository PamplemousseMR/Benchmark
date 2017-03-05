#ifndef GRAPH500_HEADER_
#define GRAPH500_HEADER_

#define NAME "Graph500 sequential list"
#define VERSION 0

#include "../compat.h"
#include "../graph_struct.h"
#include "../xalloc.h"

class BFS
{

	public:

		static int create_graph_from_edgelist(struct packed_edge*, int64_t);
		static int make_bfs_tree(int64_t*, int64_t*, int64_t);
		static void destroy_graph(void);

};

#endif /*GRAPH500_HEADER_*/
