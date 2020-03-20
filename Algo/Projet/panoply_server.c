#include "panoply.h"

client * nouveau_client_1_svc(void *argp, struct svc_req *rqstp) {

	static client result;

	printf("Entrer nom\n");
	scanf("%s", &result.nom);

	printf("Entrer prenom\n");
	scanf("%s", &result.prenom);

	printf("Entrer mail\n");
	scanf("%s", &result.mail);

	printf("Entrer mot de passe\n");
	scanf("%s", &result.password);

	return &result;
}

void * lister_abo_1_svc(void *argp, struct svc_req *rqstp) {

	static char * result;

	for(int i=0;i<NBABO;i++) {
		printf("Abonnement %d\n", i);
		printf("Prix /3mois : %d\n", );
		printf("Crédits : %d\n", );
	}

	return (void *) &result;
}

void * affecter_abo_1_svc(client *argp, struct svc_req *rqstp) {

	int index;

	printf("Choisir nouvel abonnement :\n");
	scanf("%d", index);

	argp->abo = xxxxxxxxxxx;
}

void * modifier_abo_1_svc(client *argp, struct svc_req *rqstp) {

	static char * result;

	/*
	 * insert server code here
	 */

	return (void *) &result;
}

void * lister_marques_1_svc(void *argp, struct svc_req *rqstp) {

	static char * result;

	/*
	 * insert server code here
	 */

	return (void *) &result;
}

void * recherche_vetements_marque_1_svc(void *argp, struct svc_req *rqstp) {

	static char * result;

	/*
	 * insert server code here
	 */

	return (void *) &result;
}

void * louer_vetement_1_svc(vetement *argp, struct svc_req *rqstp) {

	static char * result;

	/*
	 * insert server code here
	 */

	return (void *) &result;
}

int * connexion_1_svc(void *argp, struct svc_req *rqstp) {

	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}
