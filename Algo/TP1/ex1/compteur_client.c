#include "compteur.h"

int compteur = 0;

void compteurprog_1(char *host, int ajout_1_arg) {

	CLIENT *clnt;
	void  *result_1;
	int  *result_2;
	char *lire_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, COMPTEURPROG, COMPTEURVERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = ajout_1(&ajout_1_arg, clnt);
	if (result_1 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = lire_1((void*)&lire_1_arg, clnt);
	if (result_2 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */

}


int main (int argc, char *argv[]) {

	char *host;
	int ajout_nombre = 0;
	if (argc < 3) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}

	host = argv[1];
	ajout_nombre = atoi(argv[2]);
	compteurprog_1 (host, ajout_nombre);
	exit (0);
}
