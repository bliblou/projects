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

int max(int a, int b) {

	if(a>b) {
		return a;
	} else {
		return b;
	}
}

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

	int continuer =0;
	int smax = max(sockAccueil, sockVente); //  smax c'est pour mettre dans le select afin de surveiller les deux sockets en meme temps... demande pas pk non plus mdrrrr
	
	struct timeval tv;
	tv.tv_sec = 30;
    tv.tv_usec = 0;

	// BOUCLE -----------------------------------------------------------------------------------------------------------------------------------------------------------
	while (continuer == 0) {

		FD_ZERO(&lect); //on met le lecteur à zero
		FD_SET(sockVente, &lect); //le lecteur doit surveiller les sockets
		FD_SET(sockAccueil, &lect);
		FD_SET(0, &lect); // le lecteur surveille l'entrée clavier (c'est 0 me demande pas pk)

		if(select(smax+1, &lect, NULL, NULL, &tv)) { //on attend qu'il se passe un truc sur les sockets....

			if (FD_ISSET(sockAccueil, &lect)) { ///SI IL SE PASSE UN TRUC SUR L'ACCUEIL BOUME on fait les trucs

				lgadresseEmetteur = sizeof(adresseEmetteur);
				if ((recu = recvfrom(sockAccueil, buf, sizeof(buf), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) == -1) {
					perror("recvfrom buf");
					close(sockVente);
					close(sockAccueil);
					exit(1);
				}
			
				adresse[nb] = adresseEmetteur;
				printf("Connection de: %s\n", buf);

				strcpy(buf, "Description objet blah blah");
				lgadresseEmetteur = sizeof(adresse[nb]);
				if ((envoye = sendto(sockVente, buf, sizeof(buf), 0, (struct sockaddr *) &adresse[nb], lgadresseEmetteur)) != sizeof(buf)) {
					perror("sendto");
					close(sockVente);
					close(sockAccueil);
					exit(1);
				}

				lgadresseEmetteur = sizeof(adresse[nb]);
				if ((envoye = sendto(sockVente, &prix, sizeof(int), 0, (struct sockaddr *) &adresse[nb], lgadresseEmetteur)) != sizeof(int)) {
					perror("sendto");
					close(sockVente);
					close(sockAccueil);
					exit(1);
				}

				nb++;
			}

			if (FD_ISSET(sockVente, &lect)) { // S'IL SE PASSE UN TRUC SUR LA VENTE BOUME ON PEUT LE FAIRE EN MEME TEMPS!!!

				lgadresseEmetteur = sizeof(adresseEmetteur);
				if ((recu = recvfrom(sockVente, &offre, sizeof(int), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) == -1) {
					perror("recvfrom");
					close(sockVente);
					close(sockAccueil);
					exit(1);
				}

				printf("Offre reçue: %d\n", offre);

				if (offre > offreCourante) {
					offreCourante = offre;

					for (int i = 0; i < nb; i++) {

						lgadresseEmetteur = sizeof(adresseEmetteur);
						if ((envoye = sendto(sockVente, &offreCourante, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
							perror("sendto");
							close(sockVente);
							close(sockAccueil);
							exit(1);
						}

						lgadresseEmetteur = sizeof(adresseEmetteur);
						if ((envoye = sendto(sockVente, &rien, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
							perror("sendto");
							close(sockVente);
							close(sockAccueil);
							exit(1);
						}
						
					}
				}
			}

			printf("Meilleure offre = %d euros. [n] pour terminer la vente.\n", offreCourante);

			if (FD_ISSET(0, &lect)) { // et là on checke s'il se passe un truc sur le clavier

				if(read(0, &reponse, 1) == 0) {
					perror("read");
					exit (1);
				}

				if(reponse == 'n') {

					printf("Fin de la vente...\n");

					for (int i = 0; i < nb; i++) {

						lgadresseEmetteur = sizeof(adresseEmetteur);
						if ((envoye = sendto(sockVente, &offreCourante, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
							perror("sendto");
							close(sockVente);
							close(sockAccueil);
							exit(1);
						}

						lgadresseEmetteur = sizeof(adresseEmetteur);
						if ((envoye = sendto(sockVente, &rien, 0, 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != 0) {
							perror("sendto");
							close(sockVente);
							close(sockAccueil);
							exit(1);
						}
						
					}

					continuer = 1; // on arrête de tout vérifier

				}
			}

		} else {

			printf("Fin de la vente...\n");

			for (int i = 0; i < nb; i++) {

				lgadresseEmetteur = sizeof(adresseEmetteur);
				if ((envoye = sendto(sockVente, &offreCourante, sizeof(int), 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != sizeof(int)) {
					perror("sendto");
					close(sockVente);
					close(sockAccueil);
					exit(1);
				}

				lgadresseEmetteur = sizeof(adresseEmetteur);
				if ((envoye = sendto(sockVente, &rien, 0, 0, (struct sockaddr *) &adresse[i], lgadresseEmetteur)) != 0) {
					perror("sendto");
					close(sockVente);
					close(sockAccueil);
					exit(1);
				}
				
			}

			continuer = 1; // on arrête de tout vérifier
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	close(sockVente);
	close(sockAccueil);

	return 1;

}
