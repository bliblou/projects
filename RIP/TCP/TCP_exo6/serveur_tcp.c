/* Lancement d'un serveur :  serveur_tcp port nomfichier*/
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <sys/time.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netdb.h>
# include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TRUE 1
#define BUFSIZE 1000000

int main (int argc, char **argv) {

  int socket_RV, socket_service, dst, n;
  char buf[BUFSIZE];
  struct sockaddr_in adresseRV;
  int lgadresseRV;
  struct sockaddr_in adresseClient;
  int lgadresseClient;
  struct hostent *hote;
  unsigned short port;


  /* creation de la socket de RV */
  if ((socket_RV = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
   	perror("socket");
   	exit(-1);
  }

  /* preparation de l'adresse locale */
  port = (unsigned short) atoi(argv[1]);

  adresseRV.sin_family = AF_INET;
  adresseRV.sin_port = htons(port);
  adresseRV.sin_addr.s_addr = htonl(INADDR_ANY);

  lgadresseRV = sizeof(adresseRV);

  /* attachement de la socket a l'adresse locale */
  if ((bind(socket_RV, (struct sockaddr *) &adresseRV, lgadresseRV)) == -1) {
   	perror("bind");
   	exit(-3);
  }

  /* declaration d'ouverture du service */
  if (listen(socket_RV, 10)==-1) {
  	perror("listen");
   	exit(-4);
  }

  /* boucle d'attente de connexion */
  while(TRUE) {

    printf("Debut de boucle\n");
    fflush(stdout);

    /* attente d'un client */
    lgadresseClient = sizeof(adresseClient);
    socket_service=accept(socket_RV, (struct sockaddr *) &adresseClient, &lgadresseClient);

    if (socket_service==-1) {
      perror("accept");
      exit(-5);
    }

    /* un client est arrive */
    printf("connexion acceptee\n");
    fflush(stdout);

    /* lancement du processus de service */
    if (fork()==0) {
      /* il n'utilise plus la socket de RV */
      close(socket_RV);

      /* ouverture du fichier a ecrire */
      if ((dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0640)) == -1) {
        printf("Pb avec %s\n", argv[2]);
        perror("Erreur: ");
        exit(-6);
      }

      /* lecture dans la socket et ecriture dans le fichier */
      int taille;
      if(read(socket_service, &taille, sizeof(int)) < 0) {
        perror("read");
        exit(-7);
      }

      if((n=read(socket_service,buf,BUFSIZE)) < 0) {
        perror("gros read");
        exit(-7);
      }

      printf("[Reception fichier]--Taille du fichier: %d octets\n", taille);
      fflush(stdout);
      printf("[Reception fichier]--Nombre d'octets recus: %d octets\n", n);
      fflush(stdout);

      int returnValue;

      if (n == taille) {

        printf("Ecriture en cours...\n");
        fflush(stdout);

        if (write(dst, buf, n) != n) {
          perror("write");
          exit(-8);
        }

        returnValue = 0;

      } else {

        returnValue = -1;
      }

      if (write(socket_service, &returnValue, sizeof(int)) != sizeof(int)) {
        perror("write");
        exit(7);
      }

      printf("[Envoyé au client]--Accusé de reception (%d)\n", returnValue);
      fflush(stdout);

      /* fermeture du fichier a ecrire */
      close(dst);
      close(socket_service);
      exit(0);

    } else {
    /* le pere n'utilise pas socket_service */
      close(socket_service);
    }

  }

  return 0;
}