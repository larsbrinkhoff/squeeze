/* This program compresses a file without losing information.
 * The "usq" program is required to unsqueeze the file
 * before it can be used.
 *
 * Typical compression rates are between 30 and 50 percent for text files.
 *
 * Squeezing a really big file takes a few minutes.
 *
 * Useage:
 *	sq [file1] [file2] ... [filen]
 *
 * where file1 through filen are the names of the files to be squeezed.
 * The file type (under CP/M or MS-DOS) is changed to ".SQ"; under UN*X,
 * ".SQ" is appended to the file name. The original file name is stored
 * in the squeezed file.
 *
 * If no file name is given on the command line you will be
 * prompted for commands (one at a time). An empty command
 * terminates the program.
 *
 * The transformations compress strings of identical bytes and
 * then encode each resulting byte value and EOF as bit strings
 * having lengths in inverse proportion to their frequency of
 * occurrance in the intermediate input stream. The latter uses
 * the Huffman algorithm. Decoding information is included in
 * the squeezed file, so squeezing short files or files with
 * uniformly distributed byte values will actually increase size.
 */

/* CHANGE HISTORY:
 * 1.3	Close files properly in case of error exit.
 * 1.4	Break up long introductory lines.
 * 1.4	Send introduction only to console.
 * 1.4	Send errors only to console.
 * 1.5  Fix BUG that caused a rare few squeezed files
 *	to be incorrect and fail the USQ crc check.
 *	The problem was that some 17 bit codes were
 *	generated but are not supported by other code.
 *	THIS IS A MAJOR CHANGE affecting TR2.C and SQ.H and
 *	requires recompilation of all files which are part
 *	of SQ. Two basic changes were made: tree depth is now
 *	used as a tie breaker when weights are equal. This
 *	makes the tree shallower. Although that may always be
 *	sufficient, an error trap was added to cause rescaling
 *	of the counts if any code > 16 bits long is generated.
 * 1.5	Add debugging displays option '-'.
 * 1.6  Fixed to work correctly under MP/M II.  Also shortened
 *      signon message.
 * 2.0	New version for use with CI-C86 compiler (CP/M-86 and MS-DOS)
 * 2.1  Converted for use in MLINK
 * 2.2  Converted for use with optimizing CI-C86 compiler (MS-DOS)
 * 3.0  Generalized for UN*X use, changed output file naming convention
 */

/* ejecteject */

/*
 * The following define MUST be set to the maximum length of a file name
 * on the system "sq" is being compiled for.  If not, "sq" will not be
 * able to check for whether the output file name it creates is valid
 * or not.
 */

#define FNM_LEN 14
#define UNIX				/* comment out for CP/M, MS-DOS versions */
#define SQMAIN

#define VERSION "3.2   05/04/85"

#include <stdio.h>
#include "sqcom.h"
#include "sq.h"
#define FALSE 0

main(argc, argv)
int argc;
char *argv[];
{
	int i,c;
	char inparg[128];	/* parameter from input */

	debug = FALSE;
	printf("File squeezer version %s (original author: R. Greenlaw)\n\n", VERSION);

	/* Process the parameters in order */
	for(i = 1; i < argc; ++i)
		obey(argv[i]);

	if(argc < 2) {
		printf("Enter file names, one line at a time, or type <RETURN> to quit.");
		do {
			printf("\n*");
			for(i = 0; i < 16; ++i) {
				if((c = getchar()) == EOF)
					c = '\n';	/* fake empty (exit) command */
				if((inparg[i] = c) == '\n') {
					inparg[i] = '\0';
					break;
				}
			}
			if(inparg[0] != '\0')
				obey(inparg);
		} while(inparg[0] != '\0');
	}
}

/* ejecteject */

obey(p)
char *p;
{
	char *q;
	char outfile[128];	/* output file spec. */

	if(*p == '-') {
		/* toggle debug option */
		debug = !debug;
		return;
	}

	/* Check for ambiguous (wild-card) name */
	for(q = p; *q != '\0'; ++q)
		if(*q == '*' || *q == '?') {
			printf("\nAmbiguous name %s ignored", p);
			return;
	}
	/* First build output file name */
	strcpy(outfile, p);		/* copy input name to output */

	/* Find and change output file suffix */

	if (strlen(outfile) + 3 > FNM_LEN) {	/* check for long file name */
		q = outfile + FNM_LEN - 3;
		*q = '\0';		/* make room for suffix */
	}
	else {
		q = outfile + strlen(outfile);
#ifndef UNIX
		for(; --q >= outfile;)
			if (*q == '.') {
				*q = '\0';	/* delete file type */
				break;
			}
#else
		--q;
#endif
	}

	strcat(outfile, ".SQ");

	squeeze(p, outfile);
}

/* ejecteject */

squeeze(infile, outfile)
char *infile, *outfile;
{
	int i, c,c2;
	FILE *inbuff, *outbuff;		/* file buffers */

	printf("%s -> %s: ", infile, outfile);

	if(!(inbuff=fopen(infile, "rb"))) {
		printf("Can't open %s for input pass 1\n", infile);
		return;
	}
	if(!(outbuff=fopen(outfile, "wb"))) {
		printf("Can't create %s\n", outfile);
		fclose(inbuff);
		return;
	}

	/* First pass - get properties of file */
	crc = 0;	/* initialize checksum */
	printf("analyzing, ");
	init_ncr();
	init_huff(inbuff);   
	fclose(inbuff);

	/* Write output file header with decoding info */
	wrt_head(outbuff, infile);

	/* Second pass - encode the file */
	printf("squeezing,");
	if(!(inbuff=fopen(infile, "rb"))) {
		printf("Can't open %s for input pass 2\n", infile);
		goto closeout;
	}
	init_ncr();	/* For second pass */

	/* Translate the input file into the output file */
	while((c = gethuff(inbuff)) != EOF)
		putce(c, outbuff);
	oflush(outbuff);
	printf(" done.\n");
closeall:
	fclose(inbuff);
closeout:
	fclose(outbuff);
}
