#ifndef RMAT_HEADER_
#define RMAT_HEADER_

#include "compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <malloc.h>
#include "xalloc.h"
#include "prng.h"
#include "generator/splittable_mrg.h"
#include "generator/graph_generator.h"

void rmat_edgelist (struct packed_edge*, int64_t, int, double, double, double);

#endif /* RMAT_HEADER_ */
