/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _GEOMETRIE_H_RPCGEN
#define _GEOMETRIE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct point {
	int x;
	int y;
};
typedef struct point point;

struct rectangle {
	struct point p1;
	struct point p2;
};
typedef struct rectangle rectangle;

struct coordonnees {
	int x1;
	int x2;
	int y1;
	int y2;
};
typedef struct coordonnees coordonnees;

struct param_inclus {
	struct rectangle rect;
	struct point p;
};
typedef struct param_inclus param_inclus;

typedef int booleen;

#define GEOM_PROG 0x20000001
#define GEOM_VERSION_1 1

#if defined(__STDC__) || defined(__cplusplus)
#define SURFACE_RECTANGLE 1
extern  int * surface_rectangle_1(rectangle *, CLIENT *);
extern  int * surface_rectangle_1_svc(rectangle *, struct svc_req *);
#define CREER_RECTANGLE 2
extern  rectangle * creer_rectangle_1(coordonnees *, CLIENT *);
extern  rectangle * creer_rectangle_1_svc(coordonnees *, struct svc_req *);
#define INCLUS 3
extern  booleen * inclus_1(param_inclus *, CLIENT *);
extern  booleen * inclus_1_svc(param_inclus *, struct svc_req *);
extern int geom_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define SURFACE_RECTANGLE 1
extern  int * surface_rectangle_1();
extern  int * surface_rectangle_1_svc();
#define CREER_RECTANGLE 2
extern  rectangle * creer_rectangle_1();
extern  rectangle * creer_rectangle_1_svc();
#define INCLUS 3
extern  booleen * inclus_1();
extern  booleen * inclus_1_svc();
extern int geom_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_point (XDR *, point*);
extern  bool_t xdr_rectangle (XDR *, rectangle*);
extern  bool_t xdr_coordonnees (XDR *, coordonnees*);
extern  bool_t xdr_param_inclus (XDR *, param_inclus*);
extern  bool_t xdr_booleen (XDR *, booleen*);

#else /* K&R C */
extern bool_t xdr_point ();
extern bool_t xdr_rectangle ();
extern bool_t xdr_coordonnees ();
extern bool_t xdr_param_inclus ();
extern bool_t xdr_booleen ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_GEOMETRIE_H_RPCGEN */
