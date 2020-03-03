/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "geometrie.h"

int * surface_rectangle_1_svc(rectangle *argp, struct svc_req *rqstp) {
	
	static int result;

	result = (argp->p2.x - argp->p1.x) * (argp->p2.y - argp->p1.y);
	printf("Surface = %d\n", result);

	return &result;
}

rectangle * creer_rectangle_1_svc(coordonnees *argp, struct svc_req *rqstp) {
	
	static rectangle result;

	result.p1.x= argp->x1;
	result.p1.y= argp->x2;
	result.p2.x= argp->y1;
	result.p2.y= argp->y2;

	printf("Rectangle créé\np1={%d;%d} p2={%d;%d}\n", result.p1.x, result.p1.y, result.p2.x, result.p2.y);

	return &result;
}

booleen * inclus_1_svc(param_inclus *argp, struct svc_req *rqstp) {

	static booleen result;

	if((argp->rect.p1.x<= argp->p.x <= argp->rect.p2.x) && (argp->rect.p1.y <= argp->p.y <= argp->rect.p2.y)) {
		result = 1;
		printf("True\n");
	} else {
		result = 0;
		printf("False\n");
	}

	return &result;
}
