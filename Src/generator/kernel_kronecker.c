#include "kernel_kronecker.h"

#ifdef _WIN32
char* create_kernel_generator(unsigned int nbBuffers) {

    char numberTmp[10];
    unsigned int i;
    int size = sizeof(char) * FUNCTION_MAX_SIZE;
    char* function = (char*)xmalloc(size);

    /* Construction du prototype */
    strcpy_s(function, size, "__kernel void "KERNEL_KRONECKER_NAME"(const double A, const double B, const double C, __global mrg_state* seeds,const int scale, const unsigned long int edges_count_total");
    for (i = 0; i < nbBuffers; ++i)
    {
        sprintf_s(numberTmp, size, "%d", i);
        strcat_s(function, size, ", __global packed_edge* edges");
        strcat_s(function, size, numberTmp);
        strcat_s(function, size, " ,const unsigned long int edges_count");
        strcat_s(function, size, numberTmp);
    }
    strcat_s(function, size, ")\n{\n");

    strcat_s(function, size,    "\tunsigned int i;\n"\
                                "\tunsigned int indice;\n"\
                                "\tunsigned int edge;\n"\
                                "\tpacked_edge* currentEdges;\n"\
                                "\tsize_t id = get_global_id(0);\n"\
                                "\tsize_t size = get_global_size(0);\n"\
                                "\tint mul;\n"\
                                "\tint ii_bit;\n"\
                                "\tdouble ab = A+B;\n"\
                                "\tdouble c_norm = C/(1-ab);\n"\
                                "\tdouble a_norm = A/ab;\n"\
                                "\tunsigned long int bornMin = (edges_count_total * id)/size;\n"\
                                "\tunsigned long int bornMax = (edges_count_total * (id + 1))/size;\n\n"\
                                "\tfor(int i=bornMin ; i<bornMax ; ++i)\n"\
                                "\t{\n"\
                                "\t\tindice = i;\n"\
                                "\t\tcurrentEdges = edges0;\n");

    for (i = 0; i < nbBuffers - 1; ++i)
    {
        strcat_s(function, size, "\t\tif (indice >= edges_count");
        sprintf_s(numberTmp, size, "%d", i);
        strcat_s(function, size, numberTmp);
        strcat_s(function, size, ") {\n");
        strcat_s(function, size, "\t\t\tindice -= edges_count");
        strcat_s(function, size, numberTmp);
        strcat_s(function, size, ";\n");
        strcat_s(function, size, "\t\t\tcurrentEdges = edges");
        sprintf_s(numberTmp, size, "%d", i+1);
        strcat_s(function, size, numberTmp);
        strcat_s(function, size, ";\n");
    }
    strcat_s(function, size, "\t\t");
    for (i = 0; i < nbBuffers - 1; ++i)
        strcat_s(function, size, "}");

    strcat_s(function, size,    "\t\tcurrentEdges[indice].v0 = 1;\n"\
                                "\t\tcurrentEdges[indice].v1 = 1;\n"\
                                "\t}\n\n"\
                                "\tfor(i=0 ; i<scale ; ++i)\n"\
                                "\t{\n"\
                                "\t\tmul = 1<<i;\n"\
                                "\t\tfor(edge=bornMin ; edge<bornMax ; ++edge)\n"\
                                "\t\t{\n"\
                                "\t\t\tii_bit = mrg_get_double_orig(&seeds[id])>ab;\n"\
                                "\t\t\tindice = edge;\n\n"\
                                "\t\t\tcurrentEdges = edges0;\n");

    for (i = 0; i < nbBuffers - 1; ++i)
    {
        strcat_s(function, size, "\t\t\tif (indice >= edges_count");
        sprintf_s(numberTmp, size, "%d", i);
        strcat_s(function, size, numberTmp);
        strcat_s(function, size, ") {\n");
        strcat_s(function, size, "\t\t\t\tindice -= edges_count");
        strcat_s(function, size, numberTmp);
        strcat_s(function, size, ";\n");
        strcat_s(function, size, "\t\t\t\tcurrentEdges = edges");
        sprintf_s(numberTmp, size, "%d", i+1);
        strcat_s(function, size, numberTmp);
        strcat_s(function, size, ";\n");
    }
    strcat_s(function, size, "\t\t\t");
    for (i = 0; i < nbBuffers - 1; ++i)
        strcat_s(function, size, "}");

    strcat_s(function, size,  "\n\n\t\t\tcurrentEdges[indice].v1 +=  mul * ( mrg_get_double_orig(&seeds[id]) > (c_norm*ii_bit + a_norm*(!ii_bit)) );\n"\
                                "\t\t\tcurrentEdges[indice].v0 +=  mul * ii_bit;\n"\
                                "\t\t}\n"\
                                "\t}\n"\
                                "}\n");

    int newLength = sizeof(char) * (strlen(kernel_kronecker) + strlen(function));
    char* res = (char*)xmalloc(newLength);
    strcpy_s(res, newLength, kernel_kronecker);
    strcat_s(res, newLength, function);
    free(function);

    return res;

}
#else
char* create_kernel_generator(unsigned int nbBuffers) {

    char numberTmp[10];
    unsigned int i;
    char* function = (char*)xmalloc(sizeof(char) * FUNCTION_MAX_SIZE);

    /* Construction du prototype */
    strcpy(function, "__kernel void "KERNEL_KRONECKER_NAME"(const double A, const double B, const double C, __global mrg_state* seeds,const int scale, const unsigned long int edges_count_total");
    for (i = 0; i < nbBuffers; ++i)
    {
        sprintf(numberTmp, "%d", i);
        strcat(function, ", __global packed_edge* edges");
        strcat(function, numberTmp);
        strcat(function, " ,const unsigned long int edges_count");
        strcat(function, numberTmp);
    }
    strcat(function, ")\n{\n");

    strcat(function,    "\tunsigned int i;\n"\
                        "\tunsigned int indice;\n"\
                        "\tunsigned int edge;\n"\
                        "\tpacked_edge* currentEdges;\n"\
                        "\tsize_t id = get_global_id(0);\n"\
                        "\tsize_t size = get_global_size(0);\n"\
                        "\tint mul;\n"\
                        "\tint ii_bit;\n"\
                        "\tdouble ab = A+B;\n"\
                        "\tdouble c_norm = C/(1-ab);\n"\
                        "\tdouble a_norm = A/ab;\n"\
                        "\tunsigned long int bornMin = (edges_count_total * id)/size;\n"\
                        "\tunsigned long int bornMax = (edges_count_total * (id + 1))/size;\n\n"\
                        "\tfor(int i=bornMin ; i<bornMax ; ++i)\n"\
                        "\t{\n"\
                        "\t\tindice = i;\n"\
                        "\t\tcurrentEdges = edges0;\n");

    for (i = 0; i < nbBuffers - 1; ++i)
    {
        strcat(function, "\t\tif (indice >= edges_count");
        sprintf(numberTmp, "%d", i);
        strcat(function, numberTmp);
        strcat(function, ") {\n");
        strcat(function, "\t\t\tindice -= edges_count");
        strcat(function, numberTmp);
        strcat(function, ";\n");
        strcat(function, "\t\t\tcurrentEdges = edges");
        sprintf(numberTmp, "%d", i + 1);
        strcat(function, numberTmp);
        strcat(function, ";\n");
    }
    strcat(function, "\t\t");
    for (i = 0; i < nbBuffers - 1; ++i)
        strcat(function, "}");

    strcat(function,    "\t\tcurrentEdges[indice].v0 = 1;\n"\
                        "\t\tcurrentEdges[indice].v1 = 1;\n"\
                        "\t}\n\n"\
                        "\tfor(i=0 ; i<scale ; ++i)\n"\
                        "\t{\n"\
                        "\t\tmul = 1<<i;\n"\
                        "\t\tfor(edge=bornMin ; edge<bornMax ; ++edge)\n"\
                        "\t\t{\n"\
                        "\t\t\tii_bit = mrg_get_double_orig(&seeds[id])>ab;\n"\
                        "\t\t\tindice = edge;\n\n"\
                        "\t\t\tcurrentEdges = edges0;\n");

    for (i = 0; i < nbBuffers - 1; ++i)
    {
        strcat(function, "\t\t\tif (indice >= edges_count");
        sprintf(numberTmp, "%d", i);
        strcat(function, numberTmp);
        strcat(function, ") {\n");
        strcat(function, "\t\t\t\tindice -= edges_count");
        strcat(function, numberTmp);
        strcat(function, ";\n");
        strcat(function, "\t\t\t\tcurrentEdges = edges");
        sprintf(numberTmp, "%d", i + 1);
        strcat(function, numberTmp);
        strcat(function, ";\n");
    }
    strcat(function, "\t\t\t");
    for (i = 0; i < nbBuffers - 1; ++i)
        strcat(function, "}");

    strcat(function,    "\n\n\t\t\tcurrentEdges[indice].v1 +=  mul * ( mrg_get_double_orig(&seeds[id]) > (c_norm*ii_bit + a_norm*(!ii_bit)) );\n"\
                        "\t\t\tcurrentEdges[indice].v0 +=  mul * ii_bit;\n"\
                        "\t\t}\n"\
                        "\t}\n"\
                        "}\n");

    char* res = (char*)xmalloc(sizeof(char) * (strlen(kernel_kronecker) + strlen(function)));
    strcpy(res, kernel_kronecker);
    strcat(res, function);
    free(function);

    return res;

}
#endif
