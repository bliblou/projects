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

int main (int argc, char **argv) {

	int sockAccueil,sockVente,recu,envoye;
	char nomh[50], reponse[2];
	char buf[256];
	struct sockaddr_in adresseLocale;
	int lgadresseLocale;
	struct sockaddr_in adresseEmetteur;
	int lgadresseEmetteur;
	struct hostent *hote;
	struct sigaction action;

	struct sockaddr_in adresse[10];
	struct sockaddr_in best;
	int nb = 0; 

	int offre;
	int prix = 10;
	int offreCourante = prix;
	int conf;

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

	if ((sockVente = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
	  	perror("socket"); 
	  	exit(1);
	}

	adresseLocale.sin_family = AF_INET;
	adresseLocale.sin_port = htons(atoi(argv[2]));
	adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((bind(sockVente, (struct sockaddr *) &adresseLocale, lgadresseLocale)) == -1) {
	  	perror("bindVente");
	  	close(sockVente);
		close(sockAccueil);
	  	exit(1);
	}

	strcpy(reponse, "");

	//BOUCLE D'ACCUEIL------------------------------------------------------------------------------------------------------------------------------------
	while(nb < 3) {

		lgadresseEmetteur = sizeof(adresseEmetteur);
		if ((recu = recvfrom(sockAccueil, buf, sizeof(buf), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) == -1) {
			perror("recvfrom buf"); 
			close(sockVente);
			close(sockAccueil);
			exit(1);
		}

		printf("Demande de connexion de: %s\n", buf);

		printf("Accepter?[y/n]\n");
		scanf("%s", reponse);

		if(strcmp(reponse, "y") == 0) {

			adresse[nb] = adresseEmetteur;
			nb++;

			conf = 1;
			lgadresseEmetteur = sizeof(adresseEmetteur);
			if ((envoye = sendto(sockAccueil, &conf, sizeof(int), 0, (struct sockaddr *) &adresseEmetteur, lgadresseEmetteur)) != sizeof(int)) {
				perror("sendto");
				close(sockVente);
				close(sockAccueil);
				exit(1);
			}

			printf("Confirmation envoyee.\n");

		} else if (strcmp(reponse, "n") == 0) {

			conf = 0;
			lgadresseEmetteur = sizeof(adresseEmetteur);
			if ((envoye = sendto(sockAccueil, &conf, sizeof(int), 0, (struct sockaddr *) &adresseEmetteur, lgadresseEmetteur)) != sizeof(int)) {
				perror("sendto");
				close(sockVente);
				close(sockAccueil);
				exit(1);
			}

			printf("Connexion refusee.\n");
		}
	}
	
	close(sockAccueil);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// BOUCLE D'AVANT VENTE -------------------------------------------------------------------------------------------------------------------------
	for (int i = 0; i < nb; i++) {
		
		strcpy(buf, "");
		strcpy(buf, "Description objet blah blah");
		lgadresseEmetteur = sizeof(adresseEmetteur);
		if ((envoye = sendto(sockVente, buf, sizeof(buf), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(buf)) {
			perror("sendto");
			close(sockVente);
			exit(1);
		}

		printf("Description envoyee\n");

		lgadresseEmetteur = sizeof(adresseEmetteur);
		if ((envoye = sendto(sockVente, &prix, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
			perror("sendto");
			close(sockVente);
			exit(1);
		}

		printf("Prix envoye\n");

	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// BOUCLE DE VENTE --------------------------------------------------------------------------------------------------------------------------------
	int continuer = 0;
	while (continuer == 0) {

		printf("Attente offre...\n");

		for (int i = 0; i<nb; i++) {

			lgadresseEmetteur = sizeof(adresseEmetteur);
			if ((recu = recvfrom(sockVente, &offre, sizeof(int), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) == -1) {
				perror("recvfrom");
				close(sockVente);
				exit(1);
			}

			printf("Offre reçue: %d\n", offre);

			if (offre > offreCourante) {
				offreCourante = offre;
			}
		}

		printf("Meilleure offre = %d euros. Continuer la vente?[y/n]\n", offreCourante);
		scanf("%s", reponse);

		int rien;

		if(strcmp(reponse, "n") == 0) {

			printf("Fin de la vente...\n");

			for (int i = 0; i < nb; ++i) {

				lgadresseEmetteur = sizeof(adresseEmetteur);
				if ((envoye = sendto(sockVente, &offreCourante, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
					perror("sendto");
					close(sockVente);
					exit(1);
				}

				lgadresseEmetteur = sizeof(adresseEmetteur);
				if ((envoye = sendto(sockVente, &rien, 0, 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != 0) {
					perror("sendto");
					close(sockVente);
					exit(1);
				}
				
			}

			printf("buffer vide et offre courante envoyes\n");

			continuer = 1;

		} else if (strcmp(reponse, "y") == 0) {

			for (int i = 0; i < nb; ++i) {

				lgadresseEmetteur = sizeof(adresseEmetteur);
				if ((envoye = sendto(sockVente, &offreCourante, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
					perror("sendto");
					close(sockVente);
					exit(1);
				}

				lgadresseEmetteur = sizeof(adresseEmetteur);
				if ((envoye = sendto(sockVente, &rien, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
					perror("sendto");
					close(sockVente);
					exit(1);
				}
				
			}

			printf("Offre courante envoyee\n");

		}

	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	close(sockVente);

	return 1;

}
