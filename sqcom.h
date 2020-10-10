#ifdef SQMAIN
#define EXTERN
#else
#define EXTERN extern
#endif

/* Definitions and external declarations */

#define RECOGNIZE 0xFF76	/* unlikely pattern */

/* *** Stuff for first translation module *** */

#define DLE 0x90

EXTERN unsigned int crc;	/* error check code */

/* *** Stuff for second translation module *** */

#define SPEOF 256	/* special endfile token */
#define NUMVALS 257	/* 256 data values plus SPEOF*/
