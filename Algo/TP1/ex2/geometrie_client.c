#include "geometrie.h"


void geom_prog_1(char *host, coordonnees creer_rectangle_1_arg, param_inclus inclus_1_arg) {

	CLIENT *clnt;
	int  *result_1;
	rectangle  *result_2;
	booleen  *result_3;

#ifndef	DEBUG
	clnt = clnt_create (host, GEOM_PROG, GEOM_VERSION_1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_2 = creer_rectangle_1(&creer_rectangle_1_arg, clnt);
	if (result_2 == (rectangle *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_1 = surface_rectangle_1(result_2, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	inclus_1_arg.rect = *result_2;
	result_3 = inclus_1(&inclus_1_arg, clnt);
	if (result_3 == (booleen *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int main (int argc, char *argv[]) {

	char *host;

	/*if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}*/
	host = argv[1];

	coordonnees coo;

	coo.x1 = atoi(argv[2]);
	coo.x2 = atoi(argv[3]);
	coo.y1 = atoi(argv[4]);
	coo.y2 = atoi(argv[5]);

	param_inclus param;

	param.p.x = atoi(argv[6]);
	param.p.y = atoi(argv[7]);

	geom_prog_1 (host, coo, param);
	exit (0);

}
