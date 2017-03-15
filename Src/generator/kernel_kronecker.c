#ifdef GRAPH_GENERATOR_OCL

static const char* KERNEL_KRONECKER_NAME = "generate_kronecker";

static const char* kernel_kronecker=
	"/*  =================== random ===================  */\n"\

	"typedef struct mrg_state {\n"\
        "unsigned int z1, z2, z3, z4, z5;\n"\
	"} mrg_state;\n"\

	"unsigned long int mod_add(unsigned long int a, unsigned long int b)\n"\
	"{\n"\
		"unsigned long int x;\n"\
		"x = a + b;\n"\
		"x = (x >= 0x7FFFFFFF) ? (x - 0x7FFFFFFF) : x;\n"\
		"return x;\n"\
	"}\n"\

	"unsigned long int mod_mul_y(unsigned long int a)\n"\
	"{\n"\
		"const unsigned int q = 20554;\n"\
		"const unsigned int r = 1727;\n"\
		"unsigned long int result = (unsigned long int)(a) / q;\n"\
		"result = 104480 * ((unsigned long int)(a) - result * q) - result * r;\n"\
		"return (unsigned long int)result;\n"\
	"}\n"\

	"unsigned long int mod_mac_y(unsigned long int sum, unsigned long int a)\n"\
	"{\n"\
		"unsigned long int result = mod_add(sum, mod_mul_y(a));\n"\
		"return result;\n"\
	"}\n"\

	"unsigned long int mod_mul_x(unsigned long int a) {\n"\
		"unsigned long int q = 20;\n"\
		"unsigned long int r = 7;\n"\
		"unsigned long int result = (unsigned long int)(a) / q;\n"\
		"result = 107374182 * ((unsigned long int)(a) - result * q) - result * r;\n"\
		"return (unsigned long int)result;\n"\
	"}\n"\

    "void mrg_orig_step(__global mrg_state* state)\n"\
	"{\n"\
		"unsigned long int new_elt = mod_mac_y(mod_mul_x(state->z1), state->z5);\n"\
		"state->z5 = state->z4;\n"\
		"state->z4 = state->z3;\n"\
		"state->z3 = state->z2;\n"\
		"state->z2 = state->z1;\n"\
		"state->z1 = new_elt;\n"\
	"}\n"\

    "unsigned long int mrg_get_uint_orig(__global mrg_state* state)\n"\
	"{\n"\
		"mrg_orig_step(state);\n"\
		"return state->z1;\n"\
	"}\n"\

    "double mrg_get_double_orig(__global mrg_state* state)\n"\
	"{\n"\
		"double temp = (double)mrg_get_uint_orig(state) * .000000000465661287524579692 + (double)mrg_get_uint_orig(state) * .0000000000000000002168404346990492787;\n"\
		"temp -= (unsigned long int)temp;\n"\
		"return temp;\n"\
	"}\n"\

	"/*  =================== programme ===================   */\n"\

	"typedef struct packed_edge\n"\
	"{\n"\
        "unsigned long v0;\n"\
        "unsigned long v1;\n"\
	"} packed_edge;\n"\

    "__kernel void generate_kronecker(const double A, const double B, const double C, __global mrg_state* seeds,const int scale,const unsigned long int edge_number, __global packed_edge* edges)\n"\
	"{\n"\
		"unsigned int i;\n"\
		"unsigned int edge;\n"\
		"size_t id = get_global_id(0);\n"\
        "size_t size = get_global_size(0);\n"\

		"int mul;\n"\
		"int ii_bit;\n"\
        "double ab = A+B;\n"\
        "double c_norm = C/(1-ab);\n"\
        "double a_norm = A/ab;\n"\

        "unsigned long int bornMin = (edge_number*id)/size;\n"\
        "unsigned long int bornMax = (edge_number*(id+1))/size;\n"\

		"for(int i=bornMin ; i<bornMax ; ++i)\n"\
		"{\n"\
			"edges[i].v0 = 1;\n"\
			"edges[i].v1 = 1;\n"\
		"}\n"\

		"for(i=0 ; i<scale ; ++i)\n"\
		"{\n"\
			"mul = 1<<i;\n"\
			"for(edge=bornMin ; edge<bornMax ; ++edge)\n"\
			"{\n"\
					"ii_bit = mrg_get_double_orig(&seeds[id])>ab;\n"\
					"edges[edge].v1 +=  mul * ( mrg_get_double_orig(&seeds[id]) > (c_norm*ii_bit + a_norm*(!ii_bit)) );\n"\
					"edges[edge].v0 +=  mul * ii_bit;\n"\
			"}\n"\
		"}\n"\
	"};\n"\
;

#endif
