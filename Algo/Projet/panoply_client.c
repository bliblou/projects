#include "panoply.h"


void pano_prog_1(char *host) {

	CLIENT *clnt;
	client  *result_1;
	char *nouveau_client_1_arg;
	void  *result_2;
	char *lister_abo_1_arg;
	void  *result_3;
	client  affecter_abo_1_arg;
	void  *result_4;
	client  modifier_abo_1_arg;
	void  *result_5;
	char *lister_marques_1_arg;
	void  *result_6;
	char *recherche_vetements_marque_1_arg;
	void  *result_7;
	vetement  louer_vetement_1_arg;
	int  *result_8;
	char *connexion_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, PANO_PROG, PANO_VERSION_1, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = nouveau_client_1((void*)&nouveau_client_1_arg, clnt);
	if (result_1 == (client *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = lister_abo_1((void*)&lister_abo_1_arg, clnt);
	if (result_2 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = affecter_abo_1(&affecter_abo_1_arg, clnt);
	if (result_3 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_4 = modifier_abo_1(&modifier_abo_1_arg, clnt);
	if (result_4 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_5 = lister_marques_1((void*)&lister_marques_1_arg, clnt);
	if (result_5 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_6 = recherche_vetements_marque_1((void*)&recherche_vetements_marque_1_arg, clnt);
	if (result_6 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_7 = louer_vetement_1(&louer_vetement_1_arg, clnt);
	if (result_7 == (void *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_8 = connexion_1((void*)&connexion_1_arg, clnt);
	if (result_8 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
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
	pano_prog_1 (host);
	exit (0);
}
