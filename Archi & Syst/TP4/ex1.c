#define _GNU_SOURCE
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

/*********** Data Type ***********/

typedef enum {

  STATE_WAIT,
  STATE_MULT,
  STATE_ADD,
  STATE_PRINT
} State;

typedef struct {
  State state;
  int * pendingMult;
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  size_t nbIterations;
  size_t size;
  double * v1;
  double * v2;
  double * v3;
  double result;
  long nbCoeur;
} Product;

/*********** Data structure ***********/

Product prod;

/*********** Function ***********/

void initPendingMult(Product * prod) {

  size_t i;

  for(i=0;i<prod->size;i++) {

    prod->pendingMult[i]=1;
  }
}

int nbPendingMult(Product * prod) {

  size_t i;
  int nb=0;

  for(i=0;i<prod->size;i++) {

    nb+=prod->pendingMult[i];
  }

  return(nb);
}

void wasteTime(unsigned long ms) {

  unsigned long t,t0;
  struct timeval tv;
  gettimeofday(&tv,(struct timezone *)0);
  t0=tv.tv_sec*1000LU+tv.tv_usec/1000LU;

  do {
    gettimeofday(&tv,(struct timezone *)0);
    t=tv.tv_sec*1000LU+tv.tv_usec/1000LU;
  } while(t-t0<ms);
}

/*****************************************************************************/
void * mult(void * data) {

  size_t index;
  size_t iter;

  printf("%p\n", data);
  /*=>Recuperation de l'index, c'est a dire index = ... */
  index = (size_t)data;

  cpu_set_t s;
  int coeur = index%prod.nbCoeur;
  CPU_SET(coeur, &s);

  if (sched_setaffinity(0, sizeof(cpu_set_t), &s) != 0) {
    perror("setaffinity");
    exit (1);
  }

  fprintf(stderr,"Begin mult(%ld) sur coeur(%d)\n",index,sched_getcpu());
                                               /* Tant que toutes les iterations */
  for(iter=0;iter<prod.nbIterations;iter++) {  /* n'ont pas eu lieu              */

   /*=>Attendre l'autorisation de multiplication POUR UNE NOUVELLE ITERATION...*/
    pthread_mutex_lock(&(prod.mutex));

    while((prod.state != STATE_MULT) || (prod.pendingMult[index] != 1)) {
      pthread_cond_wait(&(prod.cond), &(prod.mutex));
      printf (".");
    }
    printf("\n");

    pthread_mutex_unlock(&(prod.mutex));

    fprintf(stderr,"--> mult(%ld)\n",index); /* La multiplication peut commencer */

    /*=>Effectuer la multiplication a l'index du thread courant... */
    prod.v3[index] = prod.v1[index]*prod.v2[index];

    wasteTime(200+(rand()%200)); /* Perte du temps avec wasteTime() */

    fprintf(stderr,"<-- mult(%ld) : %.3g*%.3g=%.3g\n",           /* Affichage du */
          index,prod.v1[index],prod.v2[index],prod.v3[index]);  /* calcul sur   */
                                                                /* l'index      */
    
    /*=>Si c'est la derniere... */
    pthread_mutex_lock(&(prod.mutex));
    /*=>Marquer la fin de la multiplication en cours... */
    prod.pendingMult[index] = 0;
    if (!nbPendingMult(&prod)) {
      /*=>Autoriser le demarrage de l'addition... */
      prod.state = STATE_ADD;
      pthread_cond_broadcast(&(prod.cond));
    }
    pthread_mutex_unlock(&(prod.mutex));
  }

  fprintf(stderr,"Quit mult(%ld)\n",index);

  return(data);
}

/*****************************************************************************/
void * add(void * data) {

  size_t iter;
  fprintf(stderr,"Begin add()\n");
                                             /* Tant que toutes les iterations */
  for(iter=0;iter<prod.nbIterations;iter++) { /* n'ont pas eu lieu              */
  
    size_t index;

    /*=>Attendre l'autorisation d'addition... */
    pthread_mutex_lock(&(prod.mutex));

    while (prod.state != STATE_ADD) {
      pthread_cond_wait(&(prod.cond), &(prod.mutex));
      printf (".");
    }
    printf("\n");

    pthread_mutex_unlock(&(prod.mutex));

    fprintf(stderr,"--> add\n"); /* L'addition peut commencer */

    /* Effectuer l'addition... */
    prod.result=0.0;

    for(index=0;index<prod.size;index++) {

     prod.result = prod.result + prod.v3[index];
    }

    wasteTime(100+(rand()%100)); /* Perdre du temps avec wasteTime() */

    fprintf(stderr,"<-- add\n");

    /*=>Autoriser le demarrage de l'affichage... */
    pthread_mutex_lock(&(prod.mutex));
    prod.state = STATE_PRINT;
    pthread_cond_broadcast(&(prod.cond));
    pthread_mutex_unlock(&(prod.mutex));

  }

  fprintf(stderr,"Quit add()\n");
  return(data);
}

/*****************************************************************************/
int main(int argc,char ** argv) {

  size_t i, iter;
  pthread_t *multTh;
  size_t    *multData;
  pthread_t  addTh;
  void      *threadReturnValue;

  char *ptr;
  int fd;

  int tailleFichier;
  struct stat st;

  char chaine[12];

  prod.nbCoeur = sysconf(_SC_NPROCESSORS_ONLN);
  printf("Nombre de coeurs = %ld\n", prod.nbCoeur);

  fd = open(argv[1], O_RDONLY);
  if(fd == -1) {
    perror("read");
    exit (1);
  }

  if(!fstat(fd, &st)) {
    tailleFichier = st.st_size;
  } else {
    perror("stat");
    exit (1);
  }

  ptr = mmap(NULL, tailleFichier, PROT_READ, MAP_PRIVATE, fd, 0);

  prod.nbIterations == (size_t) atoi(ptr);
  printf("%ld\n", prod.nbIterations);

  prod.size == (size_t) atoi(ptr + 2);
  printf("%ld\n", prod.size);

  /* Initialisations (Product, tableaux, generateur aleatoire,etc) */
  prod.state=STATE_WAIT;
  prod.pendingMult=(int *)malloc(prod.size*sizeof(int));

  /*=>initialiser prod.mutex ... */
  pthread_mutex_init(&(prod.mutex), NULL);

  /*=>initialiser prod.cond ...  */
  pthread_cond_init(&(prod.cond), NULL);

  /* Allocation dynamique des 3 vecteurs v1, v2, v3 */
  prod.v1=(double *)malloc(prod.size*sizeof(double));
  prod.v2=(double *)malloc(prod.size*sizeof(double));
  prod.v3=(double *)malloc(prod.size*sizeof(double));

  /* Allocation dynamique du tableau pour les threads multiplieurs */
  multTh=(pthread_t *)malloc(prod.size*sizeof(pthread_t));

  /*=>Creer les threads de multiplication... */
  for (i=0;i<prod.size;i++) {

    if (pthread_create(&multTh[i], NULL, mult, (void * restrict) i) != 0) {

      perror("thcreate");
      exit (1);
    }
  }

  /*=>Creer le thread d'addition...          */
  if (pthread_create(&addTh, NULL, add, NULL) != 0) {

    perror("thcreate");
    exit (1);
  }

  srand(time((time_t *)0));   /* Init du generateur de nombres aleatoires */

  /* Pour chacune des iterations a realiser, c'est a dire :                   */
  for(iter=0;iter<prod.nbIterations;iter++) { /* tant que toutes les iterations */
                                              /* n'ont pas eu lieu              */
    size_t j;
   
    ///init vecteurs
    for(int j=0;j<prod.size;j++) {

      prod.v1[j]=atoi(ptr+j);
      prod.v2[j]=atoi(ptr+j + (prod.size*2));

      /*prod.v1[j]=10.0*(0.5-((double)rand())/((double)RAND_MAX));
      prod.v2[j]=10.0*(0.5-((double)rand())/((double)RAND_MAX));*/
    }


    /*=>Autoriser le demarrage des multiplications pour une nouvelle iteration..*/
    pthread_mutex_lock(&(prod.mutex));

    for(int i=0; i<prod.size;i++) {
      prod.pendingMult[i] = 1;
    }

    prod.state = STATE_MULT;
    pthread_cond_broadcast(&(prod.cond));
    pthread_mutex_unlock(&(prod.mutex));


    /*=>Attendre l'autorisation d'affichage...*/
    pthread_mutex_lock(&(prod.mutex));

    while(prod.state != STATE_PRINT) {
      pthread_cond_wait(&(prod.cond), &(prod.mutex)); 
      printf (".");
    }
    printf("\n");
    pthread_cond_broadcast(&(prod.cond));

    /*=>Afficher le resultat de l'iteration courante...*/
    printf("Result = %f\n", prod.result);
    pthread_mutex_unlock(&(prod.mutex));
    
  }

  /*=>Attendre la fin des threads de multiplication...*/

  for (i=0;i<prod.size;i++) {

    if (pthread_join(multTh[i], NULL) != 0) {

      perror("thjoin");
      exit(1);
    }
  }

  /*=>Attendre la fin du thread d'addition...*/
  if (pthread_join(addTh, NULL) != 0) {

    perror("thjoin");
    exit (1);
  }

  /*=> detruire prod.cond ... */
  pthread_cond_destroy(&(prod.cond));

  /*=> detruire prod.mutex ... */
  pthread_mutex_destroy(&(prod.mutex));

  /* Detruire avec free ce qui a ete initialise avec malloc */
  free(prod.pendingMult);
  free(prod.v1);
  free(prod.v2);
  free(prod.v3);
  free(multTh);

  return(1);

}