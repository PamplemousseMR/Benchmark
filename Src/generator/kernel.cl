typedef struct packed_edge 
{
	long v0;
	long v1; 	
} packed_edge;

__kernel void generate_kronecker(__global int* scale, __global long* edge_number, __global unsigned int* compute_units,__global packed_edge* output)
{
  size_t id = get_global_id(0);
  output[id].v0 = *scale;
  output[id].v1 = *compute_units;
};