#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFSIZE 512

int main(int argc, char const *argv[]) {

	int sock;
	char buf[BUFSIZE];
	int n, continuer;
	struct sockaddr_in adresse_serveur;
	struct hostent *hote;
	u_short port;
	/* test des paramètres d'entrée*/

	if (argc != 3) {
		printf("Usage : executable port machine \n");
		exit(2);
	}

	/* creation de la socket */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* recherche des informations sur le destinataire */
	hote = gethostbyname(argv[2]);
	/* conversion du port passe en argument */
	port = (u_short) atoi(argv[1]);
	/* adresse du destinataire */
	adresse_serveur.sin_family = AF_INET;
	adresse_serveur.sin_port = htons(port);
	bcopy(hote->h_addr, &adresse_serveur.sin_addr.s_addr, hote->h_length);

	/* demande de connexion au serveur */
	if (connect(sock, (struct sockaddr *) &adresse_serveur, sizeof(adresse_serveur))==-1){
		perror("connect");
		exit(2);
	}
	printf("connexion acceptee\n");
	fflush(stdout);

	continuer = 1;
	while(continuer) {

		/* requete au serveur */
		strcpy(buf, "un Big Mac");
		if (write(sock,buf,strlen(buf)+1) == -1) {
			perror("write");
			exit(1);
		}
		printf("j'ai commande un Big Mac\n");
		fflush(stdout);

		/* attente de la reponse */
		if (read(sock,buf,BUFSIZE) == -1) {
			perror("read");
			exit(1);
		}
		printf("j'ai recu un %s\n", buf);
		fflush(stdout);

		printf("Voulez-vous continuer, oui (1), non (0)\n");
		scanf("%d", &continuer);
	}
	
	/* fermeture de la socket */
	close(sock);

	return 1;
}
