/* Lancement d'un client : client_tcp port machine_serveur nom_fichier */
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

#define BUFSIZE 1000000

int main (int argc, char **argv) {

  int sock, src, n, ecrit;
  char buf[BUFSIZE];
  struct sockaddr_in adresse_serveur;
  struct hostent *hote;
  unsigned short port;

  /* creation de la socket locale */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(-1);
  }

  /* recuperation de l'adresse IP du serveur (a partir de son nom) */
  if ((hote = gethostbyname(argv[2])) == NULL) {
   	perror("gethostbyname");
   	exit(-2);
  }

  /* preparation de l'adresse du serveur */
  port = (unsigned short) atoi(argv[1]);

  adresse_serveur.sin_family = AF_INET;
  adresse_serveur.sin_port = htons(port);
  bcopy(hote->h_addr, &adresse_serveur.sin_addr, hote->h_length);
  printf("L'adresse en notation pointee %s\n", inet_ntoa(adresse_serveur.sin_addr));
  fflush(stdout);

  /* demande de connexion au serveur */
  if (connect(sock, (struct sockaddr *) &adresse_serveur, sizeof(adresse_serveur))==-1) {
   	perror("connect");
   	exit(-3);
  }

  /* le serveur a accepte la connexion */
  printf("connexion acceptee\n");
  fflush(stdout);

  /* ouverture du fichier a lire */
  if ((src = open(argv[3], O_RDONLY)) == -1) {

   	printf("Pb avec %s\n", argv[3]);
   	perror("Erreur: ");
   	exit(-4);
  }
    
  /* lecture du fichier et d'ecriture dans la socket */
  if ((n=read(src,buf,BUFSIZE)) < 0) {
   	perror("gros read");
   	exit(-5);
  }

  if (write(sock, &n, sizeof(int)) != sizeof(int)) {
    perror("write");
    exit(7);
  }

  printf("[Envoyé au serveur]--Taille du fichier: %d octets\n", n);
  fflush(stdout);

  if ((ecrit = write(sock, buf, n)) != n)	{
   	printf("on a reussi a ecrire %d octets sur %d en un seul coup\n", ecrit, n);
   	perror("write");
   	exit(-6);
  }

  int retour;
  if(read(sock, &retour, sizeof(int)) < 0) {
    perror("read");
    exit(-7);
  }

  printf("[Recu]--Accuse de reception (%d)\n", retour);
  fflush(stdout);

  if (retour == 0) {

    printf("Fichier envoye avec succes\n");
  } else {
    printf("Erreur dans l'envoi du fichier\n");
  }

  fflush(stdout);

  /* fermeture du fichier a lire et de la socket */
  close(src);
  close(sock);

  return 0;
}
