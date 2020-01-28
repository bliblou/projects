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
#include <string.h>

void arretParControlC(int sig) {

	printf("\nTerminaison par un Control-C\n");
	fflush(stdout);
}

int main (int argc, char **argv) {

	int sockAccueil,sockVente,recu,envoye;
	char confirmation[256], nomh[50];
	char buf[256];
	struct sockaddr_in adresseLocale;
	int lgadresseLocale;
	struct sockaddr_in adresseEmetteur;
	int lgadresseEmetteur;
	struct hostent *hote;
	struct sigaction action;

	struct sockaddr_in adresse[10];
	int nb = 0;

	int offre;
	int prix = 10;
	int offreCourante = 0;
	char c;

	/* handler de signal SIGINT */
	action.sa_handler = arretParControlC;
	sigaction(SIGINT, &action, NULL);

	/* cr'eation de la socket */
	if ((sockAccueil = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	  	perror("socket"); 
	  	exit(1);
	}

	/* pr'eparation de l'adresse locale : port + toutes les @ IP */
	adresseLocale.sin_family = AF_INET;
	adresseLocale.sin_port = htons(atoi(argv[1]));
	adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY);

	/* attachement de la socket a` l'adresse locale */
	lgadresseLocale = sizeof(adresseLocale);
	if ((bind(sockAccueil, (struct sockaddr *) &adresseLocale, lgadresseLocale)) == -1) {
	  	perror("bindAccueil");
	  	close(sockVente);
		close(sockAccueil);
	  	exit(1);
	}

	/*if ((sockVente = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	  	perror("socket"); 
	  	exit(1);
	}

	??????????????????

	if ((bind(sockVente, (struct sockaddr *) &adresseLocale, lgadresseLocale)) == -1) {
	  	perror("bindVente");
	  	close(sockVente);
		close(sockAccueil);
	  	exit(1);
	}*/

	lgadresseEmetteur = sizeof(adresseEmetteur);

	while(1) {

		if ((recu = recvfrom(sockAccueil, buf, sizeof(buf), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) != sizeof(buf)) {
			perror("recvfrom buf"); 
			close(sockVente);
			close(sockAccueil);
			exit(1);
		}

		printf("Demande de connexion de: %s\n", buf);

		printf("Accepter?[y/n]\n");
		scanf("%c", &c);

		if(c == 'y') {

			nb++;
			adresse[nb] = adresseEmetteur;

			strcpy(buf, "Confirmed");
			if ((envoye = sendto(sockAccueil, buf, sizeof(buf), 0, (struct sockaddr *) &adresseEmetteur, lgadresseEmetteur)) != sizeof(buf)) {
				perror("sendto");
				close(sockVente);
				close(sockAccueil);
				exit(1);
			}

			printf("Confirmation envoyee\n");
			continue;

		} else {

			//lol
		}
	}

	for (int i = 0; i < nb; ++i) {
		
		strcpy(buf, "");
		strcpy(buf, "Description objet blah blah");
		if ((envoye = sendto(sockVente, buf, sizeof(buf), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(buf)) {
			perror("sendto");
			close(sockVente);
			close(sockAccueil);
			exit(1);
		}

		printf("Description envoyee\n");

		if ((envoye = sendto(sockVente, &prix, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
			perror("sendto");
			close(sockVente);
			close(sockAccueil);
			exit(1);
		}

		printf("Prix envoye\n");

	}

	
	while(1) {

		if ((recu = recvfrom(sockVente, &offre, sizeof(int), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) == -1) {
			perror("recvfrom");
			close(sockVente);
			close(sockAccueil);
			exit(1);
		}

		printf("Offre reçue: %d", offre);

		if (offre < offreCourante) {
			offreCourante = offre;
		}

		printf("Accepter?[y/n]");
		scanf("%c", &c);

		if(c == 'n') {

			strcpy(buf, "");

			for (int i = 0; i < nb; ++i) {

				if ((envoye = sendto(sockAccueil, &offreCourante, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
					perror("sendto");
					close(sockVente);
					close(sockAccueil);
					exit(1);
				}

				if ((envoye = sendto(sockAccueil, buf, sizeof(buf), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(buf)) {
					perror("sendto");
					close(sockVente);
					close(sockAccueil);
					exit(1);
				}
				
			}

			printf("buf vide et offre courante envoyes\n");

			continue;

		} else if (c == 'y') {

			//lol
		}

	}

	close(sockVente);
	close(sockAccueil);

	return 1;

}
