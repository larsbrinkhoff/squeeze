#include <stdio.h>
#include "sqcom.h"
#include "sq.h"
#define ERROR -1

/* Get next byte from file and update checksum */

int
getc_crc(ib)
FILE *ib;
{
	int c;

	c = getc(ib);
	if (c != EOF)
		crc += c;		/* checksum */
	return (c);
}

/* Output functions with error reporting */

static char obuf[128];
static int oblen = 0;

putce(c,  iob)
int c;
FILE *iob;
{
	obuf[oblen++] = c;
	if (oblen >= sizeof(obuf)) oflush(iob);
}

putwe(w,  iob)
int w;
FILE *iob;
{
	obuf[oblen++] = w;
	if (oblen >= sizeof(obuf)) oflush(iob);
	obuf[oblen++] = w >> 8;
	if (oblen >= sizeof(obuf)) oflush(iob);
}

oflush(iob)				/* flush output buffer */
FILE *iob;
{
	if (oblen && !fwrite(obuf, oblen, 1, iob)) {
		printf("Error writing output file\n");
		exit(1);
	}
	oblen = 0;
}
