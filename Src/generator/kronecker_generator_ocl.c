#ifdef GRAPH_GENERATOR_OCL
#ifndef GRAPH_GENERATOR_OMPI

#include "kronecker_generator.h"
#include "kernel_kronecker.c"

static cl_context* contexts;
static cl_command_queue* commands;
static cl_uint* maxComputeUnits;
static cl_uint deviceCount;

static unsigned int printAndGetPlatforms(cl_uint platformCount,cl_platform_id* platforms)
{
	unsigned int chosenPlatform;
	unsigned int i;
	int valid;
	int tempBufferLength = 10;
	char tempPlatform[10];
	const cl_platform_info attributeTypes[4] = { CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_VERSION, CL_PLATFORM_PROFILE };
	size_t infoSize;
	char* info;

	printf("%d platforme(s) disponible(s) :\n", platformCount);

	for(i=0 ; i<platformCount ; ++i)
	{
		clGetPlatformInfo(platforms[i], attributeTypes[0], 0, NULL, &infoSize);
		info = (char*) malloc(infoSize);
		clGetPlatformInfo(platforms[i], attributeTypes[0], infoSize, info, NULL);
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
}

static void createContexts()
{
    unsigned int i;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_device_id* devices;
    cl_int err;
    cl_context_properties properties[3];
	unsigned int chosenPlatform;

    if(clGetPlatformIDs(0, NULL, &platformCount) != CL_SUCCESS )
    {
        fprintf(stderr,"[createContexts] error when get platforms id\n");
        exit(EXIT_FAILURE);
    }
    if(platformCount<=0)
    {
        fprintf(stderr,"[createContexts] no platforms found\n");
        exit(EXIT_FAILURE);
    }
    platforms = (cl_platform_id*)xmalloc(sizeof(cl_platform_id) * platformCount);
    if(clGetPlatformIDs(platformCount, platforms, NULL) != CL_SUCCESS )
    {
        fprintf(stderr,"[createContexts] error when get platforms id\n");
        exit(EXIT_FAILURE);
	}

	chosenPlatform = printAndGetPlatforms(platformCount, platforms);

    if(clGetDeviceIDs(platforms[chosenPlatform], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount) != CL_SUCCESS )
    {
        fprintf(stderr,"[createContexts] error when get devices id\n");
        exit(EXIT_FAILURE);
    }
    if(deviceCount<=0)
    {
        fprintf(stderr,"[createContexts] no devices found\n");
        exit(EXIT_FAILURE);
    }
    devices = (cl_device_id*)xmalloc(sizeof(cl_device_id) * deviceCount);
    if(clGetDeviceIDs(platforms[chosenPlatform], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL) != CL_SUCCESS )
    {
        fprintf(stderr,"[createContexts] error when get devices id\n");
        exit(EXIT_FAILURE);
    }

    contexts = (cl_context*)xmalloc(sizeof(cl_context) * deviceCount);
    commands = (cl_command_queue*)xmalloc(sizeof(cl_command_queue) * deviceCount);
    maxComputeUnits = (cl_uint*)xmalloc(sizeof(cl_uint) * deviceCount);

    properties[0]= CL_CONTEXT_PLATFORM;
    properties[1]= (cl_context_properties)platforms[chosenPlatform];
    properties[2]= 0;

	GRAPH_OMP(omp parallel for shared(commands,contexts,properties,devices) private(err))
    for (i=0; i<deviceCount; ++i)
    {
        contexts[i] = clCreateContext(properties,1,&devices[i],NULL,NULL,&err);
        if(err != CL_SUCCESS)
        {
           fprintf(stderr,"[createContexts] error when create context\n");
           exit(EXIT_FAILURE);
        }
        #ifdef _WIN32
        commands[i] = clCreateCommandQueue(contexts[i], devices[i], 0, &err);
        #else
		commands[i] = clCreateCommandQueueWithProperties(contexts[i], devices[i], 0, &err);
        #endif
        if(err != CL_SUCCESS)
        {
           fprintf(stderr,"[createContexts] error when create command queue\n");
           exit(EXIT_FAILURE);
        }

        clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(cl_uint), &maxComputeUnits[i], NULL);
    }


    xfree_large(devices);
    xfree_large(platforms);
}

static void destroyContexts()
{
    unsigned int i;
	GRAPH_OMP(omp parallel for shared(commands,contexts))
    for (i=0; i<deviceCount; ++i)
    {
        if(clReleaseCommandQueue(commands[i]) != CL_SUCCESS )
        {
            fprintf(stderr,"[destroyContexts] error when release command queue\n");
            exit(EXIT_FAILURE);
        }
        if(clReleaseContext(contexts[i]) != CL_SUCCESS )
        {
            fprintf(stderr,"[destroyContexts] error when release device\n");
            exit(EXIT_FAILURE);
        }
    }
    xfree_large(contexts);
    xfree_large(commands);
	xfree_large(maxComputeUnits);
}

static void permutation(int64_t numb_node, cl_mem* scale,cl_mem* edge_number, cl_mem* edges, cl_mem* seed, cl_program* program)
{
	/*	variables utiles */
	size_t global;
	cl_kernel kernel;
	cl_mem cl_perm;

	/* creer les buffers */
	createBuffer(&contexts[0], CL_MEM_READ_WRITE, sizeof(int64_t)*numb_node, NULL, &cl_perm);
	createKernel(program, KERNEL_NODE_SHUFFLE_NAME, &kernel);

	/* affecter les arguments au programme */
	setKernelArg(&kernel, 0, sizeof(cl_mem), scale);
	setKernelArg(&kernel, 1, sizeof(cl_mem), edge_number);
	setKernelArg(&kernel, 2, sizeof(cl_mem), edges);
	setKernelArg(&kernel, 3, sizeof(cl_mem), seed);
	setKernelArg(&kernel, 4, sizeof(cl_mem), &cl_perm);

	/* lancer le programme */
	global=1;
	enqueueNDRangeKernel(&commands[0], &kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
	finish(&commands[0]);

	/*	netoyer le GPU */
	releaseMemObject(&cl_perm);
	releaseKernel(&kernel);
}

void generate_kronecker_egdes(int scale, int64_t edge_number, mrg_state* seed, packed_edge* edges)
{
    cl_kernel kernel;
    cl_program program;
	cl_mem cl_seed;
	cl_mem cl_edges;
    cl_mem cl_scale;
    cl_mem cl_edge_number;
	size_t global;
	int i;

	createContexts();

	/* creer le programme */
	createProgram(&contexts[0],1,kernel_kronecker, NULL, &program);
	buildProgram(&program,&contexts[0], 0, NULL, NULL, NULL, NULL);
	createKernel(&program, KERNEL_KRONECKER_NAME, &kernel);

    /* alouer les buffers */
	createBuffer(&contexts[0], CL_MEM_READ_ONLY, sizeof(mrg_state), NULL, &cl_seed);
	createBuffer(&contexts[0], CL_MEM_READ_ONLY, sizeof(int), NULL, &cl_scale);
	createBuffer(&contexts[0], CL_MEM_READ_ONLY, sizeof(long), NULL, &cl_edge_number);
	createBuffer(&contexts[0], CL_MEM_READ_WRITE, sizeof(packed_edge) * edge_number, NULL, &cl_edges);

    /* affecter les donnees aux buffers */
	enqueueWriteBuffer(&commands[0], &cl_seed, CL_TRUE, 0, sizeof(int), seed, 0, NULL, NULL);
	enqueueWriteBuffer(&commands[0], &cl_scale, CL_TRUE, 0, sizeof(int), &scale, 0, NULL, NULL);
	enqueueWriteBuffer(&commands[0], &cl_edge_number, CL_TRUE, 0, sizeof(unsigned long int), &edge_number, 0, NULL, NULL);

    /* affecter les arguments au programme */
	setKernelArg(&kernel, 0, sizeof(cl_mem), &cl_seed);
	setKernelArg(&kernel, 1, sizeof(cl_mem), &cl_scale);
	setKernelArg(&kernel, 2, sizeof(cl_mem), &cl_edge_number);
	setKernelArg(&kernel, 3, sizeof(cl_mem), &cl_edges);

    /* lancer le programme */
	global=maxComputeUnits[0];
	enqueueNDRangeKernel(&commands[0], &kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
	finish(&commands[0]);

	/* netoyage des donnees GPU */
	releaseKernel(&kernel);

	/* permutation aleatoire des sommets puis des aretes	*/
	permutation(1<<scale,&cl_scale, &cl_edge_number, &cl_edges,&cl_seed, &program);

	/* recuperer les donnees GPU dans le CPU */
	clEnqueueReadBuffer(commands[0], cl_edges, CL_TRUE, 0, sizeof(packed_edge)*edge_number, edges, 0, NULL, NULL);

	/* dernier netoyage */
	releaseMemObject(&cl_scale);
	releaseMemObject(&cl_edges);
	releaseMemObject(&cl_edge_number);
	releaseMemObject(&cl_seed);
	releaseProgram(&program);

					printf("output: \n");

					for(i=0;i<32; i++)
					{
						printf("%ld -> %ld\n",edges[i].v0, edges[i].v1);
					}


	/* supression des contextes */
    destroyContexts();
}

#endif /* GRAPH_GENERATOR_OMPI */
#endif /* GRAPH_GENERATOR_OCL */
