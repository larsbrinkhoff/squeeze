/* Definitions and external declarations */

EXTERN char	debug;	/* Boolean flag */

/* *** Stuff for first translation module *** */

EXTERN int likect;	/*count of consecutive identical chars */
EXTERN int lastchar, newchar;
EXTERN char state;

/* states */

#define NOHIST	0 	/*don't consider previous input*/
#define SENTCHAR 1 	/*lastchar set, no lookahead yet */
#define SENDNEWC 2 	/*newchar set, previous sequence done */
#define SENDCNT 3 	/*newchar set, DLE sent, send count next */

/* *** Stuff for second translation module *** */

#define NOCHILD -1	/* indicates end of path through tree */
#define NUMNODES (NUMVALS + NUMVALS - 1)	/* nbr of nodes */

#define MAXCOUNT 0xFFFF	/* biggest unsigned integer */

/* The following array of structures are the nodes of the
 * binary trees. The first NUMVALS nodes becomethe leaves of the
 * final tree and represent the values of the data bytes being
 * encoded and the special endfile, SPEOF.
 * The remaining nodes become the internal nodes of the final tree.
 */

EXTERN struct	nd {
	unsigned int weight;	/* number of appearances */
	char tdepth;		/* length on longest path in tre*/
	int lchild, rchild;	/* indexes to next level */
} node[NUMNODES];

EXTERN int dctreehd;	/*index to head node of final tree */

/* This is the encoding table:
 * The bit strings have first bit in  low bit.
 * Note that counts were scaled so code fits unsigned integer
 */

EXTERN char codelen[NUMVALS];		/* number of bits in code */
EXTERN unsigned int code[NUMVALS];	/* code itself, right adjusted */
EXTERN unsigned int tcode;		/* temporary code value */


/* Variables used by encoding process */

EXTERN int curin;		/* Value currently being encoded */
EXTERN char cbitsrem;		/* Number of code string bits remaining */
EXTERN unsigned int ccode;	/* Current code shifted so next code bit is at right */
