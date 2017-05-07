#ifdef GRAPH_GENERATOR_OCL

#include "kronecker_generator.h"

static cl_context* contexts;
static cl_command_queue* commands;
static cl_uint deviceCount;

#define SHUFFLE(name,type)                                                  \
    static void name(type* array, int64_t l, mrg_state* seed)                   \
{                                                                           \
    int i;                                                                  \
    int j;                                                                  \
    type t;                                                                 \
    for(i=0 ; i<l ; ++i)                                                    \
{                                                                       \
    j = (int)(i+mrg_get_uint_orig(seed)/(MRG_RAND_MAX / (l-i)+1));		\
    t = array[j];                                                   \
    array[j] = array[i];                                            \
    array[i] = t;                                                   \
    }                                                                       \
    }

SHUFFLE(suffle_int,int)
SHUFFLE(suffle_edges,packed_edge)

#undef SHUFFLE

static void random_node_permutation(int numb_node,int64_t edge_count, packed_edge* edges, mrg_state* seed)
{
    int i;
    int* vec = (int*)xmalloc(numb_node * sizeof(int));

    GRAPH_OMP(omp parallel for shared(vec))
            for(i=0 ; i<numb_node;++i)
            vec[i] = i;

    suffle_int(vec,numb_node,seed);

    GRAPH_OMP(omp parallel for shared(edges,vec))
            for(i=0 ; i<edge_count ; ++i)
    {
        edges[i].v0 = vec[edges[i].v0-1];
        edges[i].v1 = vec[edges[i].v1-1];
    }

    xfree_large(vec);
}

static void random_edges_permutation(int64_t edge_count, packed_edge* edges, mrg_state* seed)
{
    suffle_edges(edges,edge_count, seed);
}

/*static unsigned int printAndGetPlatforms(cl_uint platformCount,cl_platform_id* platforms)
{
    unsigned int chosenPlatform;
    unsigned int i;
    int valid;
    int tempBufferLength = 10;
    char tempPlatform[10];
    size_t infoSize;
    char* info;

    printf("%d platforme(s) disponible(s) :\n", platformCount);

    for(i=0 ; i<platformCount ; ++i)
    {
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, NULL, &infoSize);
        info = (char*)malloc(infoSize);
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, infoSize, info, NULL);
        printf("\t%d -> %s\n", i, info);
        free(info);
    }

    printf("\n");
    valid = 0;

    if(platformCount == 1)
        return 0;

    do {

        printf("Veuillez choisir une platforme (défaut : 0) : ");
        fgets(tempPlatform, tempBufferLength, stdin);
        chosenPlatform = atoi(tempPlatform);
        valid = 1;

        if (chosenPlatform >= platformCount) {

            printf("Numéro invalide\n");
            valid = 0;

        }

    } while (!valid);
    return chosenPlatform;
}*/

static void createContexts()
{
    int i;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_device_id* devices;
    cl_context_properties properties[3];
    unsigned int chosenPlatform;

    /* recuperer les platformes */
    getPlatformIDs(0, NULL, &platformCount);
	if(platformCount==0)
	{
		fprintf(stderr,"[createContext] platformCount = 0, check qmake and NVIDIA/AMD platform !!!\n");
		exit(EXIT_FAILURE);
	}
    platforms = (cl_platform_id*)xmalloc(sizeof(cl_platform_id) * platformCount);
    getPlatformIDs(platformCount, platforms, NULL);

    /* choisir la platform */
    /*chosenPlatform = printAndGetPlatforms(platformCount, platforms);*/
    chosenPlatform = 0;

    /* recuperer les peripheriques de la platforme */
    getDeviceIDs(&platforms[chosenPlatform], KRONECKER_DEVICE_TYPE, 0, NULL, &deviceCount);
    devices = (cl_device_id*)xmalloc(sizeof(cl_device_id) * deviceCount);
    getDeviceIDs(&platforms[chosenPlatform], KRONECKER_DEVICE_TYPE, deviceCount, devices, NULL);

    /* alouer un espace pour un contexte/commande par peripherique */
    contexts = (cl_context*)xmalloc(sizeof(cl_context) * deviceCount);
    commands = (cl_command_queue*)xmalloc(sizeof(cl_command_queue) * deviceCount);

    /* creer les contextes et les commandes */
    properties[0]= CL_CONTEXT_PLATFORM;
    properties[1]= (cl_context_properties)platforms[chosenPlatform];
    properties[2]= 0;

    GRAPH_OMP(omp parallel for shared(commands,contexts,properties,devices))
            for (i=0; i<(signed)deviceCount; ++i)
    {
        createContext(properties,1,&devices[i],NULL,NULL,&contexts[i]);
        createCommandQueue(&contexts[i], &devices[i], 0, &commands[i]);
    }

    xfree_large(devices);
    xfree_large(platforms);
}

static void destroyContexts()
{
    int i;
    GRAPH_OMP(omp parallel for shared(commands,contexts))
            for (i=0; i<(signed)deviceCount; ++i)
    {
        releaseCommandQueue(&commands[i]);
        releaseContext(&contexts[i]);
    }
    xfree_large(contexts);
    xfree_large(commands);
}

static void init(size_t* local, size_t* global, cl_ulong* buffer_count, int64_t edge_count)
{
    /* variable utils */
    cl_device_id deviceId;
    unsigned int max_work_item;
    size_t max_item_per_group;
    cl_ulong max_mem_size;
    char* value;
    size_t valueSize;
    cl_device_type type;
    unsigned int i;

    /* verifier le define */
	if(GENERATOR_ITEMS_BY_GROUP % OPTIMAL_GENERATOR_MOD != 0)
    {
		fprintf(stderr,"[generate_kronecker_egdes] items by group %d is not mod %d",GENERATOR_ITEMS_BY_GROUP,OPTIMAL_GENERATOR_MOD);
        exit(EXIT_FAILURE);
    }

    /* recuperer le minimum du maximum des work group et des work item de chaque peripheriques */
    /* recuperer le nombre minimum de buffer par GPU necessaire pour lancer le programme */
    max_work_item = UINT_MAX;
    max_item_per_group = ULONG_MAX;
    *buffer_count = 0;
    for(i=0 ; i<deviceCount ; ++i)
    {
        clGetContextInfo(contexts[0],CL_CONTEXT_DEVICES,sizeof(cl_device_id), &deviceId, NULL);
        if(getMaxWorkItem(&deviceId) < max_work_item)
            max_work_item = getMaxWorkItem(&deviceId);
        if(getMaxItemByGroup(&deviceId) < max_item_per_group)
            max_item_per_group = getMaxItemByGroup(&deviceId);
        clGetDeviceInfo(deviceId, CL_DEVICE_MAX_MEM_ALLOC_SIZE,sizeof(cl_ulong), &max_mem_size, NULL);
        if(ceil((sizeof(packed_edge)*edge_count)/(double)max_mem_size) > *buffer_count)
            *buffer_count = (cl_ulong)ceil((sizeof(packed_edge)*edge_count)/(double)max_mem_size);
    }

    /* calculs du nombre de groupe et d'item */
    *global = edge_count;
    if(edge_count > max_work_item)
        *global = max_work_item;

	if(*global > GENERATOR_ITEMS_BY_GROUP)
    {
		*global -= (*global)%GENERATOR_ITEMS_BY_GROUP;
		*local = (*global)/GENERATOR_ITEMS_BY_GROUP;
        if(*local > getMaxItemByGroup(&deviceId))
        {
            *local = getMaxItemByGroup(&deviceId);
			*global = (*local)*GENERATOR_ITEMS_BY_GROUP;
        }
    }
    else
        *local = 1;

    /* informations */
    if(VERBOSE)
    {
		printf ("\n===============GPU GENERATE PARAMETERS===============\n\n");

        printf("Packed edge size : %lu B\n",sizeof(packed_edge) * edge_count);
        printf("Buffer used per device : %lu\n",*buffer_count);
        printf("Edges count : %ld\n",edge_count);
        printf("Maximum items per device : %d\n",max_work_item);
#ifdef _WIN32
        printf("Items count %u\n",*global);
        printf("Blocks count %u\n",*local);
        printf("Items per block : %u\n",*global / *local);
#else
        printf("Items count %zu\n",*global);
        printf("Blocks count %zu\n",*local);
        printf("Items per block : %zu\n",*global / *local);
#endif
        printf("Items iterations : %f\n",((double)(edge_count)) / *global);
        printf("Run on : \n");
        for (i=0; i<deviceCount; ++i)
        {
            clGetContextInfo(contexts[0],CL_CONTEXT_DEVICES,sizeof(cl_device_id), &deviceId, NULL);
            clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, NULL, &valueSize);
            value = (char*)xmalloc(valueSize);
            clGetDeviceInfo(deviceId, CL_DEVICE_NAME, valueSize, value, NULL);
            clGetDeviceInfo(deviceId, CL_DEVICE_TYPE, sizeof(cl_device_type), &type, NULL);
            char* t;
            switch (type)
            {
            case CL_DEVICE_TYPE_CPU:
                t = "CPU";
                break;
            case CL_DEVICE_TYPE_GPU:
                t="GPU";
                break;
            case CL_DEVICE_TYPE_ACCELERATOR:
                t="ACCELERATOR";
                break;
            default:
                t="DEFAULT";
                break;
            }
            printf("\t%d - Device: %s, type: %s.\n", i+1, value,t);
            xfree_large(value);
        }
		printf ("\n===============GPU GENERATE PARAMETERS===============\n\n");
    }
}

void generate_kronecker_egdes(int scale, int64_t edge_count, mrg_state* seed, packed_edge* edges)
{
    /* variables utiles */
    cl_kernel kernel;
    cl_program program;
    char* program_sources;
    cl_mem cl_seed;
    cl_mem* cl_edges;
    uint64_t* edges_count;
    size_t global;
    size_t local;
    cl_ulong buffer_count;
    unsigned int i;
    mrg_state* seeds;
    uint64_t bornMin;
    uint64_t bornMax;
    unsigned int start;

    /*creation des contextes */
    createContexts();

    /* recuperer les valeur pour le nombre d'item et de groupes de travail */
    init(&local, &global, &buffer_count, edge_count);

    /* une graine par thread */
    seeds = (mrg_state*)xmalloc(sizeof(mrg_state)*global);
    for(i=0 ; i<global ; ++i)
        make_mrg_seed(mrg_get_uint_orig(seed),mrg_get_uint_orig(seed),(uint_fast32_t*)&seeds[i]);

    /* creer le programme */
    program_sources = create_kernel_generator((unsigned int)buffer_count);
    if(!createProgram(&contexts[0],1,program_sources, NULL, &program))
    {
        fprintf(stderr,"[generate_kronecker_egdes] Error when create program\n");
        exit(EXIT_FAILURE);
    }
    if(!buildProgram(&program,&contexts[0], 0, NULL, NULL, NULL, NULL))
    {
        fprintf(stderr,"[generate_kronecker_egdes] Error when build program\n");
        exit(EXIT_FAILURE);
    }
    if(!createKernel(&program, KERNEL_KRONECKER_NAME, &kernel))
    {
        fprintf(stderr,"[generate_kronecker_egdes] Error when create kernel\n");
        exit(EXIT_FAILURE);
    }

    /* creer le buffer de graine */
    if(!createBuffer(&contexts[0], CL_MEM_READ_ONLY, sizeof(mrg_state)*global, NULL, &cl_seed))
    {
        fprintf(stderr,"[generate_kronecker_egdes] Error when create seeds buffer\n");
        exit(EXIT_FAILURE);
    }
    if(!enqueueWriteBuffer(&commands[0], &cl_seed, CL_TRUE, 0, sizeof(mrg_state)*global, seeds, 0, NULL, NULL))
    {
        fprintf(stderr,"[generate_kronecker_egdes] Error when wrtie into seeds buffer\n");
        exit(EXIT_FAILURE);
    }

    /* les buffer d'arretes */
    cl_edges = (cl_mem*)xmalloc(sizeof(cl_mem)*buffer_count);
    edges_count = (uint64_t*)xmalloc(sizeof(uint64_t)*buffer_count);
    for(i=0 ; i<buffer_count ; ++i)
    {
        bornMin = (edge_count*i)/buffer_count;
        bornMax = (edge_count*(i+1))/buffer_count;
        edges_count[i] = bornMax-bornMin;
        if(!createBuffer(&contexts[0], CL_MEM_READ_WRITE, sizeof(packed_edge)*edges_count[i], NULL, &cl_edges[i]))
        {
            fprintf(stderr,"[generate_kronecker_egdes] Error when create buffer %i\n",i);
            exit(EXIT_FAILURE);
        }
        if(!setKernelArg(&kernel, 6+i*2, sizeof(cl_mem), &cl_edges[i]))
        {
            fprintf(stderr,"[generate_kronecker_egdes] Error when set arg %i\n",i);
            exit(EXIT_FAILURE);
        }
        if(!setKernelArg(&kernel, 7+i*2, sizeof(cl_long), (cl_long*)(&edges_count[i])))
        {
            fprintf(stderr,"[generate_kronecker_egdes] Error when set arg %i\n",i);
            exit(EXIT_FAILURE);
        }
    }

    /* affecter les arguments au programme */
	if(!setKernelArg(&kernel, 0, sizeof(cl_double), (cl_double*)(&A)))
    {
		fprintf(stderr,"[generate_kronecker_egdes] Error when set arg A\n");
        exit(EXIT_FAILURE);
    }
    if(!setKernelArg(&kernel, 1, sizeof(cl_double), (cl_double*)(&B)))
    {
		fprintf(stderr,"[generate_kronecker_egdes] Error when set arg B\n");
        exit(EXIT_FAILURE);
    }
    if(!setKernelArg(&kernel, 2, sizeof(cl_double), (cl_double*)(&C)))
    {
		fprintf(stderr,"[generate_kronecker_egdes] Error when set arg C\n");
        exit(EXIT_FAILURE);
    }
    if(!setKernelArg(&kernel, 3, sizeof(cl_mem), &cl_seed))
    {
		fprintf(stderr,"[generate_kronecker_egdes] Error when set arg seed\n");
        exit(EXIT_FAILURE);
    }
    if(!setKernelArg(&kernel, 4, sizeof(cl_int), (cl_int*)(&scale)))
    {
		fprintf(stderr,"[generate_kronecker_egdes] Error when set arg scale\n");
        exit(EXIT_FAILURE);
    }
    if(!setKernelArg(&kernel, 5, sizeof(cl_long), (cl_long*)(&edge_count)))
    {
		fprintf(stderr,"[generate_kronecker_egdes] Error when set arg edge_count\n");
        exit(EXIT_FAILURE);
    }

    /* lancer le programme */
    if(!enqueueNDRangeKernel(&commands[0], &kernel, 1, NULL, &global, &local, 0, NULL, NULL))
    {
		fprintf(stderr,"[generate_kronecker_egdes] Error when launch program\n");
        exit(EXIT_FAILURE);
    }
    if(!finish(&commands[0]))
    {
		fprintf(stderr,"[generate_kronecker_egdes] Error when finish program\n");
        exit(EXIT_FAILURE);
    }

    /* recuperer les donnees GPU dans le CPU */
    start = 0;
    for(i=0 ; i<buffer_count ; ++i)
    {
        clEnqueueReadBuffer(commands[0], cl_edges[i], CL_TRUE, 0, sizeof(packed_edge)*edges_count[i], &edges[start], 0, NULL, NULL);
        start+=(unsigned int)edges_count[0];
    }

    /* permutation aleatoire des sommets	*/
    random_node_permutation(1<<scale, edge_count, edges,seed);
    /*	permutation ameatoire des aretes	*/
    random_edges_permutation(edge_count, edges, seed);

    /* netoyage GPU */
    for(i=0 ; i<buffer_count ; ++i)
        releaseMemObject(&cl_edges[i]);
    releaseMemObject(&cl_seed);
    releaseProgram(&program);
    releaseKernel(&kernel);

    /* netoyage CPU */
    free(seeds);
    free(cl_edges);
    free(edges_count);
    free(program_sources);

    /* supression des contextes */
    destroyContexts();
}

#endif /* GRAPH_GENERATOR_OCL */
