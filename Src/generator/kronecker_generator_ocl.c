#ifdef GRAPH_GENERATOR_OCL
#ifndef GRAPH_GENERATOR_OMPI

#include "kronecker_generator.h"
#include "kernel_kronecker.c"

static cl_context* contexts;
static cl_command_queue* commands;
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
	int i;
	cl_uint platformCount;
	cl_platform_id* platforms;
	cl_device_id* devices;
	cl_context_properties properties[3];
	unsigned int chosenPlatform;

	/* recuperer les platformes */
	getPlatformIDs(0, NULL, &platformCount);
	if(platformCount<=0)
	{
		fprintf(stderr,"[createContexts] no platforms found\n");
		exit(EXIT_FAILURE);
	}
	platforms = (cl_platform_id*)xmalloc(sizeof(cl_platform_id) * platformCount);
	getPlatformIDs(platformCount, platforms, NULL);

	/* choisir la platform */
	chosenPlatform = printAndGetPlatforms(platformCount, platforms);

	/* recuperer les peripheriques de la platforme */
	getDeviceIDs(&platforms[chosenPlatform], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
	if(deviceCount<=0)
	{
		fprintf(stderr,"[createContexts] no devices found\n");
		exit(EXIT_FAILURE);
	}
	devices = (cl_device_id*)xmalloc(sizeof(cl_device_id) * deviceCount);
	getDeviceIDs(&platforms[chosenPlatform], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL);

	/* alouer un espace pour un(e) contexte/commande par peripherique */
	contexts = (cl_context*)xmalloc(sizeof(cl_context) * deviceCount);
	commands = (cl_command_queue*)xmalloc(sizeof(cl_command_queue) * deviceCount);

	/* creer les contextes et les commandes */
	properties[0]= CL_CONTEXT_PLATFORM;
	properties[1]= (cl_context_properties)platforms[chosenPlatform];
	properties[2]= 0;

	GRAPH_OMP(omp parallel for shared(commands,contexts,properties,devices))
	for (i=0; i<deviceCount; ++i)
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
}

void generate_kronecker_egdes(int scale, int64_t edge_number, mrg_state* seed, packed_edge* edges)
{
	cl_kernel kernel;
	cl_program program;
	cl_mem cl_seed;
	cl_mem cl_edges;
	size_t global;
	size_t local;
	unsigned int i;

	/*creation des contextes */
	createContexts();

	/* peripherique associer */
	cl_device_id deviceId;
	clGetContextInfo(contexts[0],CL_CONTEXT_DEVICES,sizeof(cl_device_id), &deviceId, NULL);

	/* verifier le define */
	if(ITEMS_BY_GROUP % OPTIMAL_MOD != 0)
	{
		fprintf(stderr,"[generate_kronecker_egdes] items by group %d is not mod %d",ITEMS_BY_GROUP,OPTIMAL_MOD);
		exit(EXIT_FAILURE);
	}

	/* calculs du nombre de groupe */
	global = edge_number;
	if(edge_number > getMaxWorkItem(&deviceId))
		global = getMaxWorkItem(&deviceId);

	if(global > ITEMS_BY_GROUP)
	{
		global -= global%ITEMS_BY_GROUP;
		local = global/ITEMS_BY_GROUP;
		if(local > getMaxItemByGroup(&deviceId))
		{
			local = getMaxItemByGroup(&deviceId);
			global = local*ITEMS_BY_GROUP;
		}
	}
	else
		local = 1;

	/* une graine par thread */
	mrg_state* seeds = (mrg_state*)xmalloc(sizeof(mrg_state) * global);
	for(i=0 ; i<global ; ++i)
		make_mrg_seed(mrg_get_uint_orig(seed),mrg_get_uint_orig(seed),(uint_fast32_t*)&seeds[i]);

	/* creer le programme */
	createProgram(&contexts[0],1,kernel_kronecker, NULL, &program);
	buildProgram(&program,&contexts[0], 0, NULL, NULL, NULL, NULL);
	createKernel(&program, KERNEL_KRONECKER_NAME, &kernel);

	/* creer les buffers */
	createBuffer(&contexts[0], CL_MEM_READ_ONLY, sizeof(mrg_state)*global, NULL, &cl_seed);
	createBuffer(&contexts[0], CL_MEM_READ_WRITE, sizeof(packed_edge) * edge_number, NULL, &cl_edges);

	/* affecter les donnees aux buffers */
	enqueueWriteBuffer(&commands[0], &cl_seed, CL_TRUE, 0, sizeof(mrg_state)*global, seeds, 0, NULL, NULL);

	/* suppression des graine dans le CPU */
	free(seeds);

	/* affecter les arguments au programme */
	setKernelArg(&kernel, 0, sizeof(cl_mem), &cl_seed);
	setKernelArg(&kernel, 1, sizeof(int), &scale);
	setKernelArg(&kernel, 2, sizeof(int64_t), &edge_number);
	setKernelArg(&kernel, 3, sizeof(cl_mem), &cl_edges);


	printf("max item : %d\n",getMaxWorkItem(&deviceId));
	printf("numb edges : %d\n",edge_number);
	printf("item par block : %d\n",global/local);
	printf("iteration moyenne par item : %f\n",((double)(edge_number))/global);
	printf("global %zu , local %zu\n",global, local);

	/* lancer le programme */
	enqueueNDRangeKernel( &commands[0], &kernel, 1, NULL, &global, &local, 0, NULL, NULL);
	finish(&commands[0]);

	/* recuperer les donnees GPU dans le CPU */
	clEnqueueReadBuffer(commands[0], cl_edges, CL_TRUE, 0, sizeof(packed_edge)*edge_number, edges, 0, NULL, NULL);

					/*printf("\noutput: \n");

					for(i=0;i<edge_number; i++)
					{
						printf("%ld -> %ld\n",edges[i].v0, edges[i].v1);
					}*/

	/* netoyage GPU */
	releaseMemObject(&cl_edges);
	releaseMemObject(&cl_seed);
	releaseProgram(&program);
	releaseKernel(&kernel);

	/* supression des contextes */
	destroyContexts();
}

/* pour les tests */
/*cl_device_id deviceId;
clGetContextInfo(contexts[0],CL_CONTEXT_DEVICES,sizeof(cl_device_id), &deviceId, NULL);

size_t si;
clGetDeviceInfo(deviceId, CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(size_t), &si, NULL);
printf("CL_DEVICE_MAX_WORK_GROUP_SIZE : %zu\n",si);

size_t work = 0;
clGetKernelWorkGroupInfo(kernel,deviceId, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t),&work,NULL);
printf("CL_KERNEL_WORK_GROUP_SIZE : %zu\n",work);

size_t item[3];
clGetDeviceInfo(deviceId, CL_DEVICE_MAX_WORK_ITEM_SIZES	,sizeof(size_t)*3, &item, NULL);
printf("CL_DEVICE_MAX_WORK_ITEM_SIZES : %zu %zu %zu\n",item[0],item[1],item[2]);

cl_uint dim;
clGetDeviceInfo(deviceId, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,sizeof(cl_uint), &dim, NULL);
printf("CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS : %d\n",dim);

printf("%u\n",getMaxWorkItem(&deviceId));

cl_ulong buf = 0;
clGetDeviceInfo(deviceId, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,sizeof(cl_ulong), &buf, NULL);
printf("CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE : %zu\n",buf);

cl_ulong mem = 0;
clGetDeviceInfo(deviceId, CL_DEVICE_MAX_MEM_ALLOC_SIZE ,sizeof(cl_ulong), &mem, NULL);
printf("CL_DEVICE_MAX_MEM_ALLOC_SIZE : %zu\n",mem);

printf("CL_DEVICE_MAX_COMPUTE_UNITS : %d\n",maxComputeUnits[0]);*/

/*	fin tests */

#endif /* GRAPH_GENERATOR_OMPI */
#endif /* GRAPH_GENERATOR_OCL */
