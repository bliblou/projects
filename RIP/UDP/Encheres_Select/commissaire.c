#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAXCLIENTS 10

int main (int argc, char **argv) {

	int sockAccueil,sockVente,recu,envoye;
	char nomh[50];
	char reponse;
	char buf[256];
	struct sockaddr_in adresseLocale;
	int lgadresseLocale;
	struct sockaddr_in adresseEmetteur;
	int lgadresseEmetteur;
	struct hostent *hote;
	struct sigaction action;

	struct sockaddr_in adresse[MAXCLIENTS];
	int nb = 0; 

	int offre;
	int prix = 10;
	int offreCourante = prix;
	int conf;
	int rien;

	fd_set lect;

	/* creation de la socket */
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

	printf("Démarrer la vente?[y]\n");

	//BOUCLE D'ACCUEIL------------------------------------------------------------------------------------------------------------------------------------
	int continuer = 0;
	while(continuer == 0) {

		FD_ZERO(&lect);
		FD_SET(sockAccueil, &lect);
		FD_SET(0, &lect);

		select(sockAccueil+1, &lect, NULL, NULL, NULL);

		if (FD_ISSET(sockAccueil, &lect)) {

			lgadresseEmetteur = sizeof(adresseEmetteur);
			if ((recu = recvfrom(sockAccueil, buf, sizeof(buf), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) == -1) {
				perror("recvfrom buf");
				close(sockVente);
				close(sockAccueil);
				exit(1);
			}
		
			adresse[nb] = adresseEmetteur;
			nb++;
			printf("Connection de: %s\n", buf);
		}

		if (FD_ISSET(0, &lect)) { //surveillance

			if(read(0, &reponse, 1) == 0) {
				perror("read");
				exit (1);
			}

			if(reponse == 'y') {

				continuer = 1;
				conf = 1;

				for (int i=0;i<nb;i++) {
				
					lgadresseEmetteur = sizeof(adresse[i]);
					if ((envoye = sendto(sockAccueil, &conf, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
						perror("sendto");
						close(sockVente);
						close(sockAccueil);
						exit(1);
					}
				}

				printf("Démarrage de la vente.\n");

			}
		}
	}
	
	close(sockAccueil);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// BOUCLE D'AVANT VENTE -------------------------------------------------------------------------------------------------------------------------

	for (int i = 0; i < nb; i++) {
		
		strcpy(buf, "");
		strcpy(buf, "Description objet blah blah");
		lgadresseEmetteur = sizeof(adresse[i]);
		if ((envoye = sendto(sockVente, buf, sizeof(buf), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(buf)) {
			perror("sendto");
			close(sockVente);
			exit(1);
		}

		printf("Description envoyee\n");

		lgadresseEmetteur = sizeof(adresse[i]);
		if ((envoye = sendto(sockVente, &prix, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
			perror("sendto");
			close(sockVente);
			exit(1);
		}

		printf("Prix envoye\n");
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// BOUCLE DE VENTE --------------------------------------------------------------------------------------------------------------------------------
	continuer = 0;
	while (continuer == 0) {

		FD_ZERO(&lect);
		FD_SET(sockVente, &lect);
		FD_SET(0, &lect);

		select(sockVente+1, &lect, NULL, NULL, NULL);

		printf("Attente offre...\n");

		if (FD_ISSET(sockVente, &lect)) {

			lgadresseEmetteur = sizeof(adresseEmetteur);
			if ((recu = recvfrom(sockVente, &offre, sizeof(int), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) == -1) {
				perror("recvfrom");
				close(sockVente);
				exit(1);
			}

			printf("Offre reçue: %d\n", offre);

			if (offre > offreCourante) {
				offreCourante = offre;

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
			}
		}

		printf("Meilleure offre = %d euros. [n] pour terminer la vente.\n", offreCourante);

		if (FD_ISSET(0, &lect)) {

			if(read(0, &reponse, 1) == 0) {
				perror("read");
				exit (1);
			}

			if(reponse == 'n') {

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

			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	close(sockVente);

	return 1;

}
