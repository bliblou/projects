#include "geometrie.h"


void geom_prog_1(char *host, rectangle surface_rectangle_1_arg, coordonnees creer_rectangle_1_arg, param_inclus inclus_1_arg) {

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

	result_1 = surface_rectangle_1(&surface_rectangle_1_arg, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = creer_rectangle_1(&creer_rectangle_1_arg, clnt);
	if (result_2 == (rectangle *) NULL) {
		clnt_perror (clnt, "call failed");
	}
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

	rectangle rect;

	rect.p1.x = atoi(argv[2]);
	rect.p1.y = atoi(argv[3]);
	rect.p2.x = atoi(argv[4]);
	rect.p2.y = atoi(argv[5]);

	coordonnees coo;

	coo.x1 = atoi(argv[6]);
	coo.x2 = atoi(argv[7]);
	coo.y1 = atoi(argv[8]);
	coo.y2 = atoi(argv[9]);

	param_inclus param;

	param.rect = rect;
	param.p.x = atoi(argv[10]);
	param.p.y = atoi(argv[11]);

	geom_prog_1 (host, rect, coo, param);
	exit (0);

}
