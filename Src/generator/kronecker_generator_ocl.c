#ifdef GRAPH_GENERATOR_OCL
#ifndef GRAPH_GENERATOR_OMPI
#ifndef GRAPH_GENERATOR_OMP

#include "kronecker_generator.h"

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

static cl_context* contexts;
static cl_command_queue* commands;
static cl_uint* maxComputeUnits;
static cl_uint deviceCount;

static void createContexts()
{
    unsigned int i;
    int valid;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_device_id* devices;
    cl_int err;
    cl_context_properties properties[3];
    unsigned int chosenPlatform;
    int tempBufferLength = 10;
    char tempPlatform[10];

    /* for printing infos */
    const cl_platform_info attributeTypes[4] = { CL_PLATFORM_NAME, CL_PLATFORM_VENDOR, CL_PLATFORM_VERSION, CL_PLATFORM_PROFILE };
    size_t infoSize;
    char* info;

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

    printf("%d platforme(s) disponibles :\n", platformCount);

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

static char* loadKernel(const char* path)
{
    FILE *fp;
    long lSize;
    char *buffer;
	#ifdef _WIN32
	errno_t  err;

	err = fopen_s (&fp,path , "rb");
	#else
	fp = fopen(path , "rb");
	#endif
    if(!fp)
    {
        perror("[loadKernel] can't open file");
        exit(EXIT_FAILURE);
    }

    fseek( fp , 0L , SEEK_END);
    lSize = ftell( fp );
    rewind( fp );

    buffer = calloc( 1, lSize+1 );
    if( !buffer )
    {
        fclose(fp);
        fprintf(stderr,"[loadKernel] error when allocate buffer\n");
        exit(1);
    }

    if( 1!=fread( buffer , lSize, 1 , fp) )
    {
        fclose(fp);
        free(buffer);
        perror("[loadKernel] can't read file");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
    return buffer;
}

void generate_kronecker_egdes(int scale, int64_t edge_number, mrg_state* seed, packed_edge* edges)
{
    cl_kernel kernel;
    cl_program program;
    cl_int err;
    cl_mem output;
    cl_mem cl_scale;
    cl_mem cl_edge_number;
    cl_mem cl_compute_units;
    size_t global;
    int i;

	char* prog = loadKernel("/home/romain/Documents/Benchmark/Src/generator/kernel.cl");
    createContexts();

    /* creer le programme */
    program = clCreateProgramWithSource(contexts[0],1,(const char **) &prog, NULL, &err);
    if (clBuildProgram(program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS)
    {
        printf("[generate_kronecker_egdes] Error building program\n");
        exit(EXIT_FAILURE);
    }
    kernel = clCreateKernel(program, "generate_kronecker", &err);

    /* alouer les buffers */
    cl_scale = clCreateBuffer(contexts[0], CL_MEM_READ_ONLY, sizeof(int), NULL, NULL);
    cl_edge_number = clCreateBuffer(contexts[0], CL_MEM_READ_ONLY, sizeof(long), NULL, NULL);
    cl_compute_units = clCreateBuffer(contexts[0], CL_MEM_WRITE_ONLY, sizeof(cl_uint), NULL, NULL);
    output = clCreateBuffer(contexts[0], CL_MEM_WRITE_ONLY, sizeof(packed_edge) * edge_number, NULL, NULL);

    /* affecter les donnees aux buffers */
    clEnqueueWriteBuffer(commands[0], cl_scale, CL_TRUE, 0, sizeof(int), &scale, 0, NULL, NULL);
    clEnqueueWriteBuffer(commands[0], cl_edge_number, CL_TRUE, 0, sizeof(long), &edge_number, 0, NULL, NULL);
    clEnqueueWriteBuffer(commands[0], cl_compute_units, CL_TRUE, 0, sizeof(cl_uint), &maxComputeUnits[0], 0, NULL, NULL);

    /* affecter les arguments au programme */
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &cl_scale);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &cl_edge_number);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &cl_compute_units);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &output);

    /* lancer le programme */
    global=maxComputeUnits[0];
    clEnqueueNDRangeKernel(commands[0], kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
    clFinish(commands[0]);

    /* recuperer les donnees GPU dans le CPU */
    clEnqueueReadBuffer(commands[0], output, CL_TRUE, 0, sizeof(packed_edge) *edge_number, edges, 0, NULL, NULL);

    printf("output: \n");

    for(i=0;i<32; i++)
    {
        printf("%d -> %d\n",edges[i].v0, edges[i].v1);
    }

    // cleanup - release OpenCL resources
    clReleaseMemObject(output);
    clReleaseMemObject(cl_compute_units);
    clReleaseMemObject(cl_scale);
    clReleaseMemObject(cl_edge_number);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    destroyContexts();
    free(prog);
}

#endif /* GRAPH_GENERATOR_OMP */
#endif /* GRAPH_GENERATOR_OMPI */
#endif /* GRAPH_GENERATOR_OCL */
