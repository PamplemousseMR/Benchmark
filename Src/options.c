#include "options.h"

int VERBOSE = 0;

char *dumpname = NULL;
char *rootname = NULL;

double A = A_PARAM;
double B = B_PARAM;
double C = C_PARAM;
double D = 1.0 - (A_PARAM + B_PARAM + C_PARAM);

int NBFS = NBFS_MAX;

int64_t SCALE = DEFAULT_SCALE;
int64_t edgefactor = DEFAULT_EDGEFACTOR;

static void print_options()
{
    printf ("Options:\n"
            "  v   : version\n"
            "  h|? : this message\n"
            "  s   : scale (default %" PRId64 ")\n"
            "  e   : edge factor (default %" PRId64 ")\n"
            "  A|a : A (default %lg) >= 0\n"
            "  B|b : B (default %lg) >= 0\n"
            "  C|c : C (default %lg) >= 0\n"
            "  D|d : D (default %lg) >= 0\n"
            "        Note: Setting 3 of A,B,C,D requires the arguments to sum to\n"
            "        at most 1.  Otherwise, the parameters are added and normalized\n"
            "        so that the sum is 1.\n"
            "  V   : Enable extra (Verbose) output\n"
            "  o   : Read the edge list from (or dump to) the named file\n"
            "  r   : Read the BFS roots from (or dump to) the named file\n"
            "\n"
            "Outputs take the form of \"key: value\", with keys:\n"
            "  SCALE\n"
            "  edgefactor\n"
            "  construction_time\n"
            "  min_time\n"
            "  firstquartile_time\n"
            "  median_time\n"
            "  thirdquartile_time\n"
            "  max_time\n"
            "  mean_time\n"
            "  stddev_time\n"
            "  min_nedge\n"
            "  firstquartile_nedge\n"
            "  median_nedge\n"
            "  thirdquartile_nedge\n"
            "  max_nedge\n"
            "  mean_nedge\n"
            "  stddev_nedge\n"
            "  min_TEPS\n"
            "  firstquartile_TEPS\n"
            "  median_TEPS\n"
            "  thirdquartile_TEPS\n"
            "  max_TEPS\n"
            "  harmonic_mean_TEPS\n"
            "  harmonic_stddev_TEPS\n"
            , DEFAULT_SCALE, DEFAULT_EDGEFACTOR,
            A_PARAM, B_PARAM, C_PARAM,
            (1.0 - (A_PARAM + B_PARAM + C_PARAM))
            );
}

void get_options (int argc, char **argv)
{
    char *optarg = "";
    int c, err = 0;
    int nset = 0;
    int whichset = 0;

	#ifdef _WIN32
	char* buf = NULL;
	size_t sz = 0;
	if (_dupenv_s(&buf,&sz,"VERBOSE"))
	#else
	if (getenv("VERBOSE"))
	#endif
        VERBOSE = 1;

	while ((c = getopt(argc, argv, "v?hVo:r:s:e:A:a:B:b:C:c:D:d:")) != -1)
        switch (c) {
        case 'v':
			printf ("%s version %f\n", NAME, VERSION);
            exit (EXIT_SUCCESS);
            break;
        case 'h':
        case '?':
            print_options();
            exit (EXIT_SUCCESS);
            break;
        case 'V':
            VERBOSE = 1;
            break;
        case 'o':
			#ifdef _WIN32
			dumpname = _strdup(optarg);
			#else
			dumpname = strdup(optarg);
			#endif
            if (!dumpname) {
                fprintf (stderr, "Cannot copy dump file name.\n");
                err = 1;
            }
            break;
        case 'r':
		#ifdef _WIN32
		rootname = _strdup(optarg);
		#else
		rootname = strdup(optarg);
		#endif
            if (!rootname) {
                fprintf (stderr, "Cannot copy BFS root file name.\n");
                err = 1;
            }
            break;
        case 's':
            errno = 0;
            SCALE = strtol(optarg, NULL, 10);
            if (errno) {
                fprintf (stderr, "Error parsing scale %s\n", optarg);
                err = -1;
            }
            if (SCALE <= 0) {
                fprintf (stderr, "Scale must be non-negative.\n");
                err = -1;
            }
            break;
        case 'e':
            errno = 0;
            edgefactor = strtol (optarg, NULL, 10);
            if (errno) {
                fprintf (stderr, "Error parsing edge factor %s\n", optarg);
                err = -1;
            }
            if (edgefactor <= 0) {
                fprintf (stderr, "Edge factor must be non-negative.\n");
                err = -1;
            }
            break;
        case 'A':
        case 'a':
            errno = 0;
            A = strtod (optarg, NULL);
            if (whichset & 1) {
                fprintf (stderr, "A already set\n");
                err = -1;
            }
            if (errno) {
                fprintf (stderr, "Error parsing A %s\n", optarg);
                err = -1;
            }
            if (A < 0) {
                fprintf (stderr, "A must be non-negative\n");
                err = -1;
            }
            whichset |= 1;
            ++nset;
            break;
        case 'B':
        case 'b':
            errno = 0;
            B = strtod (optarg, NULL);
            if (whichset & 2) {
                fprintf (stderr, "B already set\n");
                err = -1;
            }
            if (errno) {
                fprintf (stderr, "Error parsing B %s\n", optarg);
                err = -1;
            }
            if (B < 0) {
                fprintf (stderr, "B must be non-negative\n");
                err = -1;
            }
            whichset |= 2;
            ++nset;
            break;
        case 'C':
        case 'c':
            errno = 0;
            C = strtod (optarg, NULL);
            if (whichset & 4) {
                fprintf (stderr, "C already set\n");
                err = -1;
            }
            if (errno) {
                fprintf (stderr, "Error parsing C %s\n", optarg);
                err = -1;
            }
            if (C < 0) {
                fprintf (stderr, "C must be non-negative\n");
                err = -1;
            }
            whichset |= 4;
            ++nset;
            break;
        case 'D':
        case 'd':
            errno = 0;
            D = strtod (optarg, NULL);
            if (whichset & 8) {
                fprintf (stderr, "D already set\n");
                err = -1;
            }
            if (errno) {
                fprintf (stderr, "Error parsing D %s\n", optarg);
                err = -1;
            }
            if (D < 0) {
                fprintf (stderr, "D must be non-negative\n");
                err = -1;
            }
            whichset |= 8;
            ++nset;
            break;
        default:
            fprintf (stderr, "Unrecognized option\n");
            err = -1;
        }

    if (err)
        exit (EXIT_FAILURE);
    if (nset == 3) {
        switch (whichset) {
        case (4+2+1) :
            D = 1.0 - (A + B + C);
            break;
        case (8+2+1) :
            C = 1.0 - (A + B + D);
            break;
        case (8+4+1) :
            B = 1.0 - (A + C + D);
            break;
        case (8+4+2) :
            A = 1.0 - (B + C + D);
            break;
        default:
            fprintf (stderr, "Impossible combination of three bits...\n");
            abort ();
        }
        if (A < 0 || B < 0 || C < 0 || D < 0) {
            fprintf (stderr, "When setting three parameters, all must be < 1.\n"
                             "  A = %lg\n  B = %lg\n  C = %lg\n  D = %lg\n",
                     A, B, C, D);
            exit (EXIT_FAILURE);
        }
    } else if (nset > 0) {
        long double sum = A;
        sum += B;
        sum += C;
        sum += D;
        A /= sum;
        B /= sum;
        C /= sum;
        D = 1.0 - (A + B + C);
    }
}
