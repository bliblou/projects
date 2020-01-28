/* receveur portReceveur */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>

int somme = 0;

void arretParControlC(int sig) {

	printf("\nTerminaison par un Control-C\n");
	printf("Somme = %d\n", somme);
	fflush(stdout);
}

int main (int argc, char **argv) {

	int sock,recu,envoye;
	char confirmation[256], nomh[50];
	struct sockaddr_in adresseLocale;
	int lgadresseLocale;
	struct sockaddr_in adresseEmetteur;
	int lgadresseEmetteur;
	struct hostent *hote;
	struct sigaction action;

	int unEntier;

	/* handler de signal SIGINT */
	action.sa_handler = arretParControlC;
	sigaction(SIGINT, &action, NULL);

	/* cr'eation de la socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	  	perror("socket"); 
	  	exit(1);
	}

	/* pr'eparation de l'adresse locale : port + toutes les @ IP */
	adresseLocale.sin_family = AF_INET;
	adresseLocale.sin_port = htons(atoi(argv[1]));
	adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY);

	/* attachement de la socket a` l'adresse locale */
	lgadresseLocale = sizeof(adresseLocale);
	if ((bind(sock, (struct sockaddr *) &adresseLocale, lgadresseLocale)) == -1) {
	  	perror("bind"); 
	  	exit(1);
	}

	/* reception d'un entier */
	lgadresseEmetteur = sizeof(adresseEmetteur);

	while (1) {

		if ((recu = recvfrom(sock, &unEntier, sizeof(int), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) != sizeof(int)) {
			perror("recvfrom unEntier"); 
			close(sock); 
			exit(1);
		}

		printf("j'ai recu l'entier : %d\n", unEntier);

		somme = somme + unEntier;
	}

	close(sock);
}
