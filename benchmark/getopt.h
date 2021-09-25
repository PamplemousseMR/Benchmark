#ifdef _WIN32
#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FLAG_PERMUTE	0x01
#define FLAG_ALLARGS	0x02
#define FLAG_LONGONLY	0x04
#define	EMSG ""
#define	INORDER (int)1
#define PRINT_ERROR	((opterr) && (*options != ':'))
#define	BADCH (int)'?'
#define	BADARG ((*options == ':') ? (int)':' : (int)'?')
#define __progname __argv[0]

#undef	optreset
#define optreset  __mingw_optreset

static int optreset;
static int	optind = 1;
static int	opterr = 1;
static int	optopt = '?';
static char* optarg;
static int nonopt_start = -1;
static int nonopt_end = -1;
static char *place = EMSG;
static const char illoptchar[] = "unknown option -- %c";
static const char recargchar[] = "option requires an argument -- %c";
static const char recargstring[] = "option requires an argument -- %s";
static const char ambig[] = "ambiguous option -- %.*s";
static const char noarg[] = "option doesn't take an argument -- %.*s";
static const char illoptstring[] = "unknown option -- %s";

struct option		/* specification for a long form option...	*/
{
  const char *name;		/* option name, without leading hyphens */
  int         has_arg;		/* does it take an argument?		*/
  int        *flag;		/* where to save its status, or NULL	*/
  int         val;		/* its associated status value		*/
};

enum    		/* permitted values for its `has_arg' field...	*/
{
  no_argument = 0,      	/* option never takes an argument	*/
  required_argument,		/* option always requires an argument	*/
  optional_argument		/* option may take an argument		*/
};

void _vwarnx(const char *,va_list);
void warnx(const char *,...);
int parse_long_options(char * const *, const char *,const struct option *, int *, int );
int gcd(int , int );
void permute_args(int , int , int , char * const *);
int getopt_internal(int , char * const *, const char *, const struct option *, int *, int );
int getopt(int, char * const *, const char*);

#ifdef __cplusplus
}
#endif

#endif
