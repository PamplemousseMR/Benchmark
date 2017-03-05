#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <math.h>
#ifdef GRAPH_GENERATOR_MPI
#include <mpi.h>
#endif
#ifdef GRAPH_GENERATOR_OMP
#include <omp.h>
#endif

#include "../edge_struct.h"
#include "../xalloc.h"
#include "../splittable_mrg.h"

#ifndef GRAPH_GENERATOR_MPI
void make_graph(int log_numverts, int64_t M, uint64_t userseed1, uint64_t userseed2, int64_t* nedges_ptr_in, packed_edge** result_ptr_in) {
    *nedges_ptr_in = M;
    *result_ptr_in = (packed_edge*)xmalloc(M * sizeof(packed_edge));

    int max = 1<<log_numverts;

    uint_fast32_t seed[5];
    make_mrg_seed(userseed1, userseed2, seed);

    for(int i=0 ; i<M ; i++)
    {
        write_edge(&(*result_ptr_in)[i],mrg_get_uint_orig((mrg_state*)seed)%max,mrg_get_uint_orig((mrg_state*)seed)%max);
    }
}
#endif

#ifdef GRAPH_GENERATOR_MPI
void make_graph(int log_numverts, int64_t M, uint64_t userseed1, uint64_t userseed2, int64_t* nedges_ptr, packed_edge** result_ptr) {

}
#endif
