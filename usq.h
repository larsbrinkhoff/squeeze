#define LARGE 30000

/* Decoding tree */
EXTERN struct {
	int children[2];	/* left, right */
} dnode[NUMVALS - 1];

EXTERN int bpos;	/* last bit position read */
EXTERN int curin;	/* last byte value read */

/* Variables associated with repetition decoding */
EXTERN int repct;	/*Number of times to retirn value*/
EXTERN int value;	/*current byte value or EOF */
