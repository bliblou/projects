#ifndef _COMPTEUR_H_RPCGEN
#define _COMPTEUR_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define COMPTEURPROG 0x23456789
#define COMPTEURVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define ajout 1
extern  void * ajout_1(int *, CLIENT *);
extern  void * ajout_1_svc(int *, struct svc_req *);
#define lire 2
extern  int * lire_1(void *, CLIENT *);
extern  int * lire_1_svc(void *, struct svc_req *);
extern int compteurprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define ajout 1
extern  void * ajout_1();
extern  void * ajout_1_svc();
#define lire 2
extern  int * lire_1();
extern  int * lire_1_svc();
extern int compteurprog_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_COMPTEUR_H_RPCGEN */
