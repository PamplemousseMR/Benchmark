#ifdef GRAPH_GENERATOR_OCL
#ifndef GRAPH_GENERATOR_OMPI
#ifndef GRAPH_GENERATOR_OMP

#include "kronecker_generator.h"

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

static cl_context* contexts;
static cl_command_queue* commands;
static cl_uint deviceCount;

static void createContexts()
{
    unsigned int i;
    cl_uint platformCount;
    cl_platform_id* platforms;
    cl_device_id* devices;
    cl_int err;
    cl_context_properties properties[3];

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

    if(clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount) != CL_SUCCESS )
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
    if(clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, deviceCount, devices, NULL) != CL_SUCCESS )
    {
        fprintf(stderr,"[createContexts] error when get devices id\n");
        exit(EXIT_FAILURE);
    }

    contexts = (cl_context*)xmalloc(sizeof(cl_context) * deviceCount);
    commands = (cl_command_queue*)xmalloc(sizeof(cl_command_queue) * deviceCount);

    properties[0]= CL_CONTEXT_PLATFORM;
    properties[1]= (cl_context_properties)platforms[0];
    properties[2]= 0;

    for (i=0; i<deviceCount; ++i)
    {
        contexts[i] = clCreateContext(properties,1,&devices[i],NULL,NULL,&err);
        if(err != CL_SUCCESS)
        {
           fprintf(stderr,"[createContexts] error when create context\n");
           exit(EXIT_FAILURE);
        }
        commands[i] = clCreateCommandQueue(contexts[i], devices[i], 0, &err);
        if(err != CL_SUCCESS)
        {
           fprintf(stderr,"[createContexts] error when create command queue\n");
           exit(EXIT_FAILURE);
        }
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

const char *ProgramSource =
"typedef struct"\
"{"\
"   int a;"\
"   int b;"\
"} test;"\
"__kernel void generate_kronecker(__global test* output)\n"\
"{\n"\
"  size_t id = get_global_id(0);\n"\
"  output[id].a = id;\n"\
"  output[id].b = id;\n"\
"}\n";

typedef struct
{
    cl_int a;
    cl_int b;
} test;

void generate_kronecker_egdes(int scale, int64_t edge_number, mrg_state* seed, packed_edge* edges)
{
    createContexts();
    cl_kernel kernel;
    cl_program program;
    cl_int err;

    cl_mem output;

    size_t global;

    test* results = (test*)xmalloc(sizeof(test) * 5);

    int i;

    /* cree le programme depuis un noyaux */
    program = clCreateProgramWithSource(contexts[0],1,(const char **) &ProgramSource, NULL, &err);

    /* compile le programme */
    if (clBuildProgram(program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS)
    {
        printf("[generate_kronecker_egdes] Error building program\n");
        exit(EXIT_FAILURE);
    }

    /* specifier quel noyaux executer */
    kernel = clCreateKernel(program, "generate_kronecker", &err);


    /* cree les buffer de sortie */
    output = clCreateBuffer(contexts[0], CL_MEM_WRITE_ONLY, sizeof(test) * 5, NULL, NULL);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &output);

    global=5;

    // enqueue the kernel command for execution
    clEnqueueNDRangeKernel(commands[0], kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
    clFinish(commands[0]);

    // copy the results from out of the output buffer
    clEnqueueReadBuffer(commands[0], output, CL_TRUE, 0, sizeof(test) *5, results, 0, NULL, NULL);

    // print the results
    printf("output: \n");

    for(i=0;i<5; i++)
    {
        printf("%f -> %f\n",results[i].a, results[i].b);
    }

    // cleanup - release OpenCL resources
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    destroyContexts();
}

#endif /* GRAPH_GENERATOR_OMP */
#endif /* GRAPH_GENERATOR_OMPI */
#endif /* GRAPH_GENERATOR_OCL */
