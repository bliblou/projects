#include "calculatrice.h"

void calc_prog_1(char *host) {

	CLIENT *clnt;
	long  *result_1;
	operation  ajouter_1_arg;
	long  *result_2;
	operation  soustraire_1_arg;
	long  *result_3;
	operation  multiplier_1_arg;
	void  *result_4;
	int  memoriser_1_arg = 3;
	long  *result_5;
	int  extraire_1_arg = 1;
	void  *result_6;
	char *allumer_1_arg;
	long *result_7;

#ifndef	DEBUG
	clnt = clnt_create (host, CALC_PROG, CALC_VERSION_1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_6 = allumer_1((void*)&allumer_1_arg, clnt);
	if (result_6 == (void *) NULL) {
		clnt_perror (clnt, "Allumer : erreur");
	}
	result_5 = extraire_1(&extraire_1_arg, clnt);
	if (result_5 == (long *) NULL) {
		clnt_perror (clnt, "Extraire : erreur");
	}
	ajouter_1_arg.a = *result_5;
	ajouter_1_arg.b = 10;
	result_1 = ajouter_1(&ajouter_1_arg, clnt);
	if (result_1 == (long *) NULL) {
		clnt_perror (clnt, "Ajouter : erreur");
	}
	/*result_2 = soustraire_1(&soustraire_1_arg, clnt);
	if (result_2 == (long *) NULL) {
		clnt_perror (clnt, "call failed");
	}*/
	multiplier_1_arg.a = *result_1;
	multiplier_1_arg.b = 4;
	result_3 = multiplier_1(&multiplier_1_arg, clnt);
	if (result_3 == (long *) NULL) {
		clnt_perror (clnt, "Multiplier : erreur");
	}
	result_4 = memoriser_1(&memoriser_1_arg, clnt);
	if (result_4 == (void *) NULL) {
		clnt_perror (clnt, "Memoriser : erreur");
	}
	extraire_1_arg = memoriser_1_arg;
	result_7 = extraire_1(&extraire_1_arg, clnt);
	if (result_7 == (long *) NULL) {
		clnt_perror (clnt, "Extraire : erreur");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int main (int argc, char *argv[]) {

	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	calc_prog_1 (host);
	exit (0);
}
