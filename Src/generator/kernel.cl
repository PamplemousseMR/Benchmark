/*  ===== random =====  */

typedef struct mrg_state {
  unsigned long int z1, z2, z3, z4, z5;
} mrg_state;

unsigned long int mod_add(unsigned long int a, unsigned long int b)
{
    unsigned long int x;
    x = a + b;
    x = (x >= 0x7FFFFFFF) ? (x - 0x7FFFFFFF) : x;
    return x;
}

unsigned long int mod_mul_y(unsigned long int a)
{
    const unsigned int q = 20554;
    const unsigned int r = 1727;
    unsigned long int result = (unsigned long int)(a) / q;
    result = 104480 * ((unsigned long int)(a) - result * q) - result * r;
    result += (result < 0 ? 0x7FFFFFFF : 0);
    return (unsigned long int)result;
}

unsigned long int mod_mac_y(unsigned long int sum, unsigned long int a)
{
    unsigned long int result = mod_add(sum, mod_mul_y(a));
    return result;
}

unsigned long int mod_mul_x(unsigned long int a) {
    unsigned long int q = 20;
    unsigned long int r = 7;
    unsigned long int result = (unsigned long int)(a) / q;
    result = 107374182 * ((unsigned long int)(a) - result * q) - result * r;
    result += (result < 0 ? 0x7FFFFFFF : 0);
    return (unsigned long int)result;
}

void mrg_orig_step(__private mrg_state* state)
{
        unsigned long int new_elt = mod_mac_y(mod_mul_x(state->z1), state->z5);
        state->z5 = state->z4;
        state->z4 = state->z3;
        state->z3 = state->z2;
        state->z2 = state->z1;
        state->z1 = new_elt;
}

unsigned long int mrg_get_uint_orig(__private mrg_state* state)
{
        mrg_orig_step(state);
        return state->z1;
}

double mrg_get_double_orig(__private mrg_state* state)
{
    double temp = (double)mrg_get_uint_orig(state) * .000000000465661287524579692 + (double)mrg_get_uint_orig(state) * .0000000000000000002168404346990492787;
    temp -= (unsigned long int)temp;
    return temp;
}

void make_mrg_seed(unsigned long int userseed1, unsigned long int userseed2, mrg_state* seed)
{
        seed->z1 = (userseed1 & 0x3FFFFFFF) + 1;
        seed->z2 = ((userseed1 >> 30) & 0x3FFFFFFF) + 1;
        seed->z3 = (userseed2 & 0x3FFFFFFF) + 1;
        seed->z4 = ((userseed2 >> 30) & 0x3FFFFFFF) + 1;
        seed->z5 = ((userseed2 >> 60) << 4) + (userseed1 >> 60) + 1;
}

/*  ===== programme =====   */

typedef struct packed_edge
{
    long v0;
    long v1;
} packed_edge;

__kernel void generate_kronecker(__private mrg_state* seed,const __global int* scale,const __global unsigned long int* edge_number, const __global unsigned int* compute_units,__global packed_edge* output)
{
    unsigned int i;
    unsigned int edge;
    size_t id = get_global_id(0);
    unsigned long int mySeed[5];

    for(i=0 ; i<id ; ++i)
        make_mrg_seed(mrg_get_uint_orig(seed) , mrg_get_uint_orig(seed), mySeed);

    int mul;
    int	ii_bit;
    double ab = 0.57+0.19;
    double c_norm = 0.19/(1-ab);
    double a_norm = 0.57/ab;

    unsigned long int bornMin = ((*edge_number)*id)/(*compute_units);
    unsigned long int bornMax = ((*edge_number)*(id+1))/(*compute_units);

    for(int i=bornMin ; i<bornMax ; ++i)
    {
        output[i].v0 = 1;
        output[i].v1 = 1;
    }

    for(i=0 ; i<*scale ; ++i)
    {
        mul = 1<<i;
        for(edge=bornMin ; edge<bornMax ; ++edge)
        {
            ii_bit = ( mrg_get_double_orig(mySeed)>ab );
            output[edge].v1 +=  mul * ( mrg_get_double_orig(mySeed) > (c_norm*ii_bit + a_norm*(!ii_bit)) );
            output[edge].v0 +=  mul * ii_bit;
        }
    }
};
