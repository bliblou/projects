#include "calculatrice.h"

long registres[6];
long res = 0;

long * ajouter_1_svc(operation *argp, struct svc_req *rqstp) {

	static long result;

	result = argp->a + argp->b;
	printf("%ld + %ld = %ld\n", argp->a, argp->b, result);

	res = result;
	return &result;
}

long * soustraire_1_svc(operation *argp, struct svc_req *rqstp) {

	static long  result;

	result = argp->a - argp->b;
	printf("%ld - %ld = %ld\n", argp->a, argp->b, result);

	res = result;
	return &result;
}

long * multiplier_1_svc(operation *argp, struct svc_req *rqstp) {

	static long result;

	result = (argp->a) * (argp->b);
	printf("%ld * %ld = %ld\n", argp->a, argp->b, result);

	res = result;
	return &result;
}

void * memoriser_1_svc(int *argp, struct svc_req *rqstp) {

	registres[*argp] = res;
	printf("%ld mémorisé dans le registre %d.\n", res, *argp);
}


long * extraire_1_svc(int *argp, struct svc_req *rqstp) {

	static long result;

	if((*argp >= 6) || (*argp < 0)){

		result = 0;
		printf("Registre inexistant.\n");

	} else {

		result = registres[*argp];
		printf("Valeur %ld extraite du registre %d.\n", registres[*argp], *argp);
	}

	return &result;
}

void * allumer_1_svc(void *argp, struct svc_req *rqstp) {

	for(int i=0;i<6;i++) {
		registres[i]=0.0;
	}

	printf("Mémoire initialisée.\n");
}
