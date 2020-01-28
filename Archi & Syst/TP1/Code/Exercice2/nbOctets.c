#include <stdio.h>                        /* nbOctets.c */
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "partage.h"


void handler(int s) {

  printf("%d\n", s);

}

int main(int argc,char **argv) {

  pid_t  pidWC;    
  pid_t  pidREAD;
  int    status;   //pour les waitpid

  int    tube[2];
  FILE  *fIn;      // Pour faire un fdopen : int -> FILE *

  struct sigaction action;

  Zone     z;
  int *ptDeb;      // Un pointeur (int*) sur la zone debut

  char *fileName = NULL;

  int nb = -1;

  if (argc!=2) {

    fprintf(stderr,"Usage: %s fileName\n",argv[0]);
    return 1;
  }

  fileName=argv[1];

  /* A cause de warnings lorsque le code n'est pas encore la ...*/

  (void)action; (void)fIn; (void)tube; (void)status; (void)pidREAD; (void)pidWC;

  /* Gestion des signaux */
  /* =================== */

  action.sa_handler = handler;
  sigfillset(&(action.sa_mask));
  action.sa_flags = 0;

  if (sigaction(SIGUSR1, &action, NULL)) {

    printf("Erreur");
    return 1;
  }

  /* Creation de la zone de memoire partagee */
  /* ======================================= */

  if (creerZonePartagee(sizeof(int), &z)) {

    return -1;
  }

  ptDeb=(int*)z.debut;    // *ptDeb <=> *((int*)z.debut)

  /* Creation du tube */
  /* ================ */

  if (pipe(tube) == -1) {

    perror("pipe");
    supprimerZonePartagee(&z);
    exit (1);
  }

  /* Creation du processus qui fera le exec ...   */
  /* ============================================ */

  pidWC= fork();

  switch (pidWC) {

    case -1: perror("fork");
    
             close(tube[0]);
             close (tube[1]);
             supprimerZonePartagee(&z);

             return 1;

    case 0: /* Dans le processus cree :
            - Rediriger la sortie standard vers le tube
            - Fermer le(s) descripteur(s) inutile(s) a cet enfant
            - Recouvrir par la commande ``wc'' 
            */

            dup2(tube[1], STDOUT_FILENO);

            close(tube[0]);
            close(tube[1]);

            execlp("wc", "wc", "-c", fileName, NULL);

            perror("execlp");

            return 1;

  }

  /* Creation du processus qui fera la lecture ...*/
  /* ============================================ */

  pidREAD= fork();

  switch (pidREAD) {

    case -1: perror("fork");

             close(tube[0]);
             close (tube[1]);
             supprimerZonePartagee(&z);
             waitpid(pidWC, &status, 0);

             return 1;

    case 0: close(tube[1]);

            fIn=fdopen(tube[0],"r");

            if (fIn != NULL) {

              fscanf(fIn, "%d", &nb);
              fclose(fIn);
            }

            close(tube[0]);

            *ptDeb = nb;

            sleep(1);
            kill(getppid(), SIGUSR1);

            return 1;
  }

  /* La suite du pere */
  /* ================ */

  /* Fermer les descripteurs de tube inutiles au pere */
  close (tube[0]);
  close (tube[1]);

  /* Attente d'un signal */
  pause();

  /* Afficher le resultat dans la memoire partagee */
  if (*ptDeb == -1) {

    printf ("FLOP");

  } else {

    printf("Taille: %d\n", *ptDeb);
  }

  /* Attendre le 1er enfant  */
  waitpid(pidWC, &status, 0);

  /* Attendre le 2eme enfant */
  waitpid(pidREAD, &status, 0);

  /* Supprimer la memoire partagee */
  supprimerZonePartagee(&z);

  return 0;

}
