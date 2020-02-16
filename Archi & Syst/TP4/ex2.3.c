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
#include <string.h>
#include <assert.h>

/*********** Data Type ***********/

typedef enum {

  STATE_WAIT,
  STATE_MULT,
  STATE_PRINT
} State;

typedef struct {
  State state;
  int * pendingMult;
  pthread_cond_t cond;
  pthread_mutex_t mutex;
  pthread_t *multTh;
  size_t l1;
  size_t l2;
  size_t l3;
  size_t c1;
  size_t c2;
  size_t c3;
  long ** M1;
  long ** M2;
  long ** M3;
} Product;

/*********** Data structure ***********/

size_t nbIterations;
Product prod;
long nbCoeur;

/*********** Function ***********/

int nbPendingMult(Product * prod) {

  size_t i;
  int nb=0;

  for(i=0;i<(prod->l3*prod->c3);i++) {

    nb+=prod->pendingMult[i];
  }

  return(nb);
}

/*****************************************************************************/
void * mult(void * data) {

  size_t index = (size_t)data;

  //lancement de chaque thread sur un coeur différent
  cpu_set_t s;
  int coeur = index%nbCoeur;
  CPU_SET(coeur, &s);

  if (sched_setaffinity(0, sizeof(cpu_set_t), &s) != 0) {
    perror("setaffinity");
    exit (1);
  }

  //calcul indice du coefficient à calculer en fonction de l'index
  int c = index%prod.c3; //colonne
  int l;                    //ligne

  int count = 0;
  int check = index;

  while (check >= prod.c3) {

    check = check - prod.c3;
    count ++;
  }

  l = count;

  /*=>Attendre l'autorisation de multiplication POUR UNE NOUVELLE ITERATION...*/
  pthread_mutex_lock(&(prod.mutex));

  while((prod.state != STATE_MULT) || (prod.pendingMult[index] != 1)) {
    pthread_cond_wait(&(prod.cond), &(prod.mutex));
    printf (".");
  }
  printf("\n");

  pthread_mutex_unlock(&(prod.mutex));

  fprintf(stderr,"[%ld]--Start on processor core %d\n", index,sched_getcpu()); /* La multiplication peut commencer */

  /*=>Effectuer la multiplication a l'index du thread courant... */
  for (int i=0;i<prod.l1;i++) {

    prod.M3[l][c] += prod.M1[l][i]*prod.M2[i][c];
  }

  /*=>Si c'est la derniere... */
  pthread_mutex_lock(&(prod.mutex));
  /*=>Marquer la fin de la multiplication en cours... */
  prod.pendingMult[index] = 0;
  if (!nbPendingMult(&prod)) {
    //=>Autoriser le demarrage de l'affichage...
    prod.state = STATE_PRINT;
    pthread_cond_broadcast(&(prod.cond));
    printf("[%ld]--Print authorized\n", index);
  }
  pthread_mutex_unlock(&(prod.mutex));

  fprintf(stderr,"[%ld]--End\n", index);

  return(data);
}

/*****************************************************************************/
int main(int argc,char ** argv) {

  struct timeval  tv1, tv2;
  gettimeofday(&tv1, NULL);

  size_t iter;
  size_t    *multData;
  pthread_t  addTh;
  void      *threadReturnValue;

  char *ptr; 			//pointeur sur début mmap
  int fd; 				//fichier source
  FILE * dst; 			//fichier destination
  int tailleFichier; 	//taille source
  struct stat st;
  char * c; 			//curseur de lecture

  char buf[512];		//chaine ou on écrit la matrice produit
  char s[32];			//pour lecture d'un coefficient
  strcpy(buf, "");

  //ouverture des fichiers
  fd = open(argv[1], O_RDONLY);
  if(fd == -1) {
    perror("open fd");
    exit (1);
  }

  dst = fopen(argv[2], "w");
  if(!fstat(fd, &st)) {
    tailleFichier = st.st_size;
  } else {
    perror("stat");
    exit (1);
  }

  //projection en mémoire du fichier source
  ptr = mmap(NULL, tailleFichier, PROT_READ, MAP_PRIVATE, fd, 0);

  //lecture du nombre de produits matriciels à calculer
  nbIterations = strtol(ptr, &c, 10);
  assert(ptr != c);
  ptr = c;

  //calcul du nombre de coeur
  nbCoeur = sysconf(_SC_NPROCESSORS_ONLN);
  printf("Nombre de coeurs = %ld\n", nbCoeur);

  /* Pour chacune des iterations a realiser, c'est a dire :            */
  for(iter=0;iter<nbIterations;iter++) { /* tant que tous les produits */
                                              /* n'ont pas été calculés*/
      
    /* Initialisations*/
    prod.state=STATE_WAIT;

    printf("////////////////////////////////////////\n");
    printf("Produit Matriciel n°%ld\n", iter+1);

    //lecture des coefficients
    prod.l1 = strtol(ptr, &c, 10);
    assert(ptr != c);
    ptr = c;

    prod.c1 = strtol(ptr, &c, 10);
    assert(ptr != c);
    ptr = c;

    prod.l2 = strtol(ptr, &c, 10);
    assert(ptr != c);
    ptr = c;

    prod.c2 = strtol(ptr, &c, 10);
    assert(ptr != c);
    ptr = c;

    //calcul taill matrice produit
    if (prod.c1 != prod.l2) {
      printf("Matrices incompatibles !\n");
      exit(1);
    }

    prod.l3 = prod.l1;
    prod.c3 = prod.c2;

    // Allocation dynamique des 3 matrices
    prod.M1=(long **)malloc(prod.l1*sizeof(long*));
    for(int i=0;i<prod.l1;i++) {
      prod.M1[i] = (long *)malloc(prod.c1*sizeof(long));
    }
    prod.M2=(long **)malloc(prod.l2*sizeof(long*));
    for(int i=0;i<prod.l2;i++) {
      prod.M2[i] = (long *)malloc(prod.c2*sizeof(long));
    }
    prod.M3=(long **)malloc(prod.l3*sizeof(long*));
    for(int i=0;i<prod.l3;i++) {
      prod.M3[i] = (long *)malloc(prod.c3*sizeof(long));
    }

    ///init matrice 1
    for(int j=0;j<prod.l1;j++) {

      for(int k=0;k<prod.c1;k++) {

        prod.M1[j][k]=strtol(ptr, &c, 10);
        assert(ptr != c);
        ptr = c;
      }
    }

    ///init matrice 2
    for(int j=0;j<prod.l2;j++) {

      for(int k=0;k<prod.c2;k++) {

        prod.M2[j][k]=strtol(ptr, &c, 10);
        assert(ptr != c);
        ptr = c;
      }
    }

    //inittialisation pending mult
    prod.pendingMult=(int *)malloc(prod.l3*prod.c3*sizeof(int));

    for(int i=0; i<(prod.c3*prod.l3);i++) {
      prod.pendingMult[i] = 1;
    }

    /*=>initialiser prod.mutex ... */
    pthread_mutex_init(&(prod.mutex), NULL);

    /*=>initialiser prod.cond ...  */
    pthread_cond_init(&(prod.cond), NULL);

    /* Allocation dynamique du tableau pour les threads multiplieurs */
    prod.multTh=(pthread_t *)malloc(prod.l3*prod.c3*sizeof(pthread_t));

    /*=>Creer les threads de multiplication... */
    for (size_t i=0;i<(prod.l3 * prod.c3);i++) {
      if (pthread_create(&prod.multTh[i], NULL, mult, (void * restrict) i) != 0) {
        perror("thcreate");
        exit (1);
      }
    }

    /*=>Autoriser le demarrage des multiplications pour un nouveau calcul matriciel*/
    printf("Autorisation début mult\n");
    pthread_mutex_lock(&(prod.mutex));
    prod.state = STATE_MULT;
    pthread_cond_broadcast(&(prod.cond));
    pthread_mutex_unlock(&(prod.mutex));

    printf("Attente autorisation print\n");

    /*=>Attendre l'autorisation d'affichage...*/
    pthread_mutex_lock(&(prod.mutex));

    while(prod.state != STATE_PRINT) {
      pthread_cond_wait(&(prod.cond), &(prod.mutex)); 
      printf (".");
    }
    printf("\n");
    pthread_cond_broadcast(&(prod.cond));

    /*=>écriture dans le buffer de la matrice produit*/
    printf("Ecriture du résultat\n");
    for (int i=0;i<prod.l3;i++) {
      for(int j=0;j<prod.c3;j++) {
      	//écriture du coefficient lu dans le buffer
        sprintf(s, "%ld ", prod.M3[i][j]);
        strcat(buf, s);
      }
      strcat(buf, "\n");
    }
    strcat(buf, "\n");
    fputs(buf, dst); //écriture de la matrice produit dans le fichier destination
    pthread_mutex_unlock(&(prod.mutex));
    //on vide le buffer contenant la matrice produit
    strcpy(buf, "");

    //=>Attendre la fin des threads de multiplication...
	for (int i=0;i<(prod.l3*prod.c3);i++) {
	  if (pthread_join(prod.multTh[i], NULL) != 0) {
	    perror("thjoin");
	    exit(1);
	  }
	}

	/*=> detruire prod.cond ... */
	pthread_cond_destroy(&(prod.cond));

	/*=> detruire prod.mutex ... */
	pthread_mutex_destroy(&(prod.mutex));

	/* Detruire avec free ce qui a ete initialise avec malloc */
	free(prod.pendingMult);
	free(prod.M1);
	free(prod.M2);
	free(prod.M3);
	free(prod.multTh);

  }

  //fermeture des fichiers...
  close(fd);
  fclose(dst);

  //récupération du temps d'execution...
  gettimeofday(&tv2, NULL);

  printf ("Execution time = %f seconds\n",
        (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
        (double) (tv2.tv_sec - tv1.tv_sec));

  return(1);

}