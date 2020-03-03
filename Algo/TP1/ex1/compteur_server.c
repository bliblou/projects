#include "compteur.h"

int compteur = 0;

void * ajout_1_svc(int *argp, struct svc_req *rqstp) {

	static char * result;
	compteur = compteur + *argp;
	printf("-----------------------------------------------------\n");
	printf("[ajout_1_svc]-Ajout de %d\n", *argp);

	return (void *) &result;
}

int * lire_1_svc(void *argp, struct svc_req *rqstp) {

	static int  result;
	result = compteur;
	printf("[lire_1_svc]-Nouveau compteur à %d\n", compteur);
	return &result;
}
