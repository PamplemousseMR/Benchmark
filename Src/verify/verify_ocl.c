#ifdef GRAPH_VERIFY_OCL

#include "verify.h"

static cl_context* contexts;
static cl_command_queue* commands;
static cl_uint deviceCount;

static char* sources = "typedef struct packed_edge {\n" \
					   "\n" \
					   "\tunsigned long v0;\n" \
					   "\tunsigned long v1;\n" \
					   "\n" \
					   "} packed_edge;\n" \
					   "\n" \
					   "__kernel void graph_verify_edges(__global packed_edge* IJ, __global long int* level, __global int* err, __global int* nedge_traversed, unsigned long int nedge) {\n" \
					   "\n" \
					   "\tsize_t id = get_global_id(0);\n" \
					   "\tsize_t size = get_global_size(0);\n" \
					   "\tunsigned long x;\n" \
					   "\tunsigned long y;\n" \
					   "\tint neither_in;\n" \
					   "\tint both_in;\n" \
					   "\tint i;\n" \
					   "\tint cptr_edge = 0;\n" \
					   "\tlong int respect_tree_level;\n" \
					   "\t\n" \
					   "\t/* calcul des bornes */\n" \
					   "\t\n" \
					   "\tunsigned long int borneInf = nedge * id / size;\n" \
					   "\tunsigned long int borneSup = nedge * (id + 1) / size;\n" \
					   "\t\n" \
					   "\tfor (i = borneInf; i < borneSup; ++i) {\n" \
					   "	\n" \
					   "\t\tx = IJ[i].v0;\n" \
					   "\t\ty = IJ[i].v1;\n" \
					   "		\n" \
					   "\t\tneither_in = level[x] == -1 && level[y] == -1;\n" \
					   "\t\tboth_in = level[x] >= 0 && level[y] >= 0;\n" \
					   "		\n" \
					   "\t\tif (!(neither_in || both_in)) {\n" \
					   "		\n" \
					   "\t\t\terr[id] = -4;\n" \
					   "		\n" \
					   "\t\t}\n" \
					   "		\n" \
					   "\t\trespect_tree_level = level[x] - level[y];\n" \
					   "\t\trespect_tree_level = (respect_tree_level >= 0 ? respect_tree_level : -respect_tree_level);\n" \
					   "		\n" \
					   "\t\tif (!(neither_in || respect_tree_level)) {\n" \
					   "		\n" \
					   "\t\t\terr[id] = -5;\n" \
					   "		\n" \
					   "\t\t}\n" \
					   "		\n" \
					   "\t\tcptr_edge++;\n" \
					   "		\n" \
					   "\t}\n" \
					   "	\n" \
					   "\tnedge_traversed[id] = cptr_edge;\n" \
					   "	\n" \
					   "}\n";

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
	platforms = (cl_platform_id*)xmalloc(sizeof(cl_platform_id) * platformCount);
	getPlatformIDs(platformCount, platforms, NULL);

	/* choisir la platform */
	/*chosenPlatform = printAndGetPlatforms(platformCount, platforms);*/
	chosenPlatform = 0;

	/* recuperer les peripheriques de la platforme */
	getDeviceIDs(&platforms[chosenPlatform], VERIFY_DEVICE_TYPE, 0, NULL, &deviceCount);
	devices = (cl_device_id*)xmalloc(sizeof(cl_device_id) * deviceCount);
	getDeviceIDs(&platforms[chosenPlatform], VERIFY_DEVICE_TYPE, deviceCount, devices, NULL);

	/* alouer un espace pour un contexte/commande par peripherique */
	contexts = (cl_context*)xmalloc(sizeof(cl_context) * deviceCount);
	commands = (cl_command_queue*)xmalloc(sizeof(cl_command_queue) * deviceCount);

	/* creer les contextes et les commandes */
	properties[0]= CL_CONTEXT_PLATFORM;
	properties[1]= (cl_context_properties)platforms[chosenPlatform];
	properties[2]= 0;

	VERIFY_OMP(omp parallel for shared(commands,contexts,properties,devices))
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
	VERIFY_OMP(omp parallel for shared(commands,contexts))
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
	unsigned int i;

	/* verifier le define */
	if(VERIFY_ITEMS_BY_GROUP % OPTIMAL_VERIFY_MOD != 0)
	{
		fprintf(stderr,"[generate_VERIFY_egdes] items by group %d is not mod %d",VERIFY_ITEMS_BY_GROUP,OPTIMAL_VERIFY_MOD);
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

	if(*global > VERIFY_ITEMS_BY_GROUP)
	{
		*global -= (*global)%VERIFY_ITEMS_BY_GROUP;
		*local = (*global)/VERIFY_ITEMS_BY_GROUP;
		if(*local > getMaxItemByGroup(&deviceId))
		{
			*local = getMaxItemByGroup(&deviceId);
			*global = (*local)*VERIFY_ITEMS_BY_GROUP;
		}
	}
	else
		*local = 1;
}

int64_t verify_bfs_tree (int64_t* bfs_tree, int64_t max_bfsvtx, int64_t root,const packed_edge* IJ, int64_t nedge)
{
	/* variables utils */
	int i, end;
	int64_t *slice, *level, *P, *mask;

	int err = 0;
	int j=0;
	int k=0;
	int nedge_traversed=0;
	int nslice_value = 0;
	const int64_t nvalues = max_bfsvtx+1;
	
	/* variables ajoutées pour l'utilisation d'openCL */
	
	cl_ulong buffer_count;
	size_t global;
	size_t local;
	cl_kernel kernel;
	cl_program program;
	cl_mem cl_edges;
	cl_mem cl_level;
	cl_mem cl_err;
	cl_mem cl_traveresed;
	int* erreurs;
	int* perr;
	int* tab_nedge_traversed;
	int* current_nedge_traversed;

	printf("%s\n",sources);
	
	/* verification basique */
	if(bfs_tree[root] != root || root > max_bfsvtx)
	{
		err = -999;
		goto done;
	}

	/* Calcul du nombre de sommets possédant un parent */
	VERIFY_OMP(omp parallel for shared(bfs_tree) reduction(+:nslice_value))
			for(i = 0; i<nvalues; ++i)
	{
		if(bfs_tree[i]>=0)
			++nslice_value;
	}

	/* Tableau des indices des sommets possédant un parent */
	slice = (int64_t*)xmalloc(sizeof(int64_t)*nslice_value);
	/* Tableau utile pour les calculs */
	level = (int64_t*)xmalloc(sizeof(int64_t)*nvalues);
	/* Tableau des parents des sommets */
	P = (int64_t*)xmalloc(sizeof(int64_t)*nslice_value);
	/* Tableau vérifiant si un sommet parent est le sommet racine */
	mask = (int64_t*)xmalloc(sizeof(int64_t)*nslice_value);

	for(i = 0; i<nvalues; ++i)
	{
		level[i] = bfs_tree[i]>=0;
		if(level[i]==1)
		{
			slice[j] = i;
			P[j] = bfs_tree[i];
			mask[j] = bfs_tree[i] != root;
			++j;
		}
	}

	do
	{
		end = 0;
		VERIFY_OMP(omp parallel for shared(mask, level, slice, P, bfs_tree, root, end))
				for(i = 0; i<nslice_value; ++i)
		{
			if(mask[i] == 1)
			{
				VERIFY_OMP(omp critical)
						++level[slice[i]];
				P[i] = bfs_tree[P[i]];
				mask[i] = P[i] != root;
				end = 1;
			}
		}
		++k;
		if(k>nvalues)
		{
			err = -3;
			goto done;
		}

	}while(end);
	
	/* appeler le kernel */
	
	/*création des contextes */
	createContexts();

	/* récupérer les valeurs pour le nombre d'items et de groupes de travail */
	init(&local, &global, &buffer_count, nedge);
	
	/* creer le programme */
	if(!createProgram(&contexts[0],1,sources, NULL, &program))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error while creating the program\n");
		exit(EXIT_FAILURE);
	}
	if(!buildProgram(&program,&contexts[0], 0, NULL, NULL, NULL, NULL))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error while building the program\n");
		exit(EXIT_FAILURE);
	}
	if(!createKernel(&program, "graph_verify_edges", &kernel))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error while creating the kernel\n");
		exit(EXIT_FAILURE);
	}
	
	/* les arguments */
	
	/* tableau d'arêtes */
	if(!createBuffer(&contexts[0], CL_MEM_READ_ONLY, sizeof(packed_edge)*nedge, NULL, &cl_edges))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when create seeds buffer\n");
		exit(EXIT_FAILURE);
	}
	if(!enqueueWriteBuffer(&commands[0], &cl_edges, CL_TRUE, 0, sizeof(packed_edge)*nedge, IJ, 0, NULL, NULL))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when wrtie into seeds buffer\n");
		exit(EXIT_FAILURE);
	}
	if(!setKernelArg(&kernel, 0, sizeof(cl_mem), &cl_edges))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when set arg 0\n");
		exit(EXIT_FAILURE);
	}
	
	/* tableau de niveaux */
	if(!createBuffer(&contexts[0], CL_MEM_READ_ONLY, sizeof(int64_t)*nvalues, NULL, &cl_level))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when create seeds buffer\n");
		exit(EXIT_FAILURE);
	}
	if(!enqueueWriteBuffer(&commands[0], &cl_level, CL_TRUE, 0, sizeof(int64_t)*nvalues, level, 0, NULL, NULL))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when wrtie into seeds buffer\n");
		exit(EXIT_FAILURE);
	}
	if(!setKernelArg(&kernel, 1, sizeof(cl_mem), &cl_level))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when set arg 0\n");
		exit(EXIT_FAILURE);
	}
	
	/* le nombre d'arêtes */
	if(!setKernelArg(&kernel, 4, sizeof(cl_ulong), (cl_ulong*)(&nedge)))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when set arg scale\n");
		exit(EXIT_FAILURE);
	}
	
	/* le tableau des erreurs */
	if(!createBuffer(&contexts[0], CL_MEM_READ_ONLY, sizeof(int)*global, NULL, &cl_err))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when create seeds buffer\n");
		exit(EXIT_FAILURE);
	}
	if(!setKernelArg(&kernel, 2, sizeof(cl_mem), &cl_err))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when set arg 0\n");
		exit(EXIT_FAILURE);
	}
	
	/* le tableau de nedge_traversed */
	if(!createBuffer(&contexts[0], CL_MEM_READ_ONLY, sizeof(int)*global, NULL, &cl_traveresed))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when create seeds buffer\n");
		exit(EXIT_FAILURE);
	}
	if(!setKernelArg(&kernel, 3, sizeof(cl_mem), &cl_traveresed))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when set arg 0\n");
		exit(EXIT_FAILURE);
	}
	
	/* appel du programme */
	if(!enqueueNDRangeKernel(&commands[0], &kernel, 1, NULL, &global, &local, 0, NULL, NULL))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when launch program\n");
		exit(EXIT_FAILURE);
	}
	if(!finish(&commands[0]))
	{
		fprintf(stderr,"[generate_VERIFY_egdes] Error when finish program\n");
		exit(EXIT_FAILURE);
	}
	
	/* récupération des erreurs */
	erreurs = (int*) xmalloc(sizeof(int) * global);
	clEnqueueReadBuffer(commands[0], cl_err, CL_TRUE, 0, sizeof(int)*global, erreurs, 0, NULL, NULL);
	
	for (perr = erreurs; perr < erreurs + global; perr++) {

		if (*perr == -4 || *perr == -5) {

			printf("il y a eu une erreur %d\n", *perr);
			break;

		}

	}
	
	free(erreurs);
	
	/* mise à jour de nedge_traversed */
	tab_nedge_traversed = (int*) xmalloc(sizeof(int) * global);
	clEnqueueReadBuffer(commands[0], cl_traveresed, CL_TRUE, 0, sizeof(int)*global, tab_nedge_traversed, 0, NULL, NULL);
	
	for (current_nedge_traversed = tab_nedge_traversed; current_nedge_traversed < tab_nedge_traversed + global; current_nedge_traversed++) {

		nedge_traversed += *current_nedge_traversed;

	}
	
	free(tab_nedge_traversed);
	
	/* nettoyage GPU */
	releaseMemObject(&cl_err);
	releaseMemObject(&cl_traveresed);
	releaseMemObject(&cl_edges);
	releaseMemObject(&cl_level);
	releaseProgram(&program);
	releaseKernel(&kernel);

	destroyContexts();
	
	/* suite du programme */

done:
	
	xfree_large(mask);
	xfree_large(P);
	xfree_large(level);
	xfree_large(slice);

	if(err) return err;
	return nedge_traversed;
}

#endif
