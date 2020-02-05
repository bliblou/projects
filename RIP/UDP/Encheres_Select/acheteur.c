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

int main(int argc, char *argv[]) {

	int sock, envoye, recu;
	struct sockaddr_in adresseReceveur;
	char buf[256], nomh[256];
	int lgadresseReceveur;
	struct hostent *hote;

	int prix;
	int offre;

	/* cr'eation de la socket */
	if ((sock = socket( AF_INET, SOCK_DGRAM , 0 )) == -1) {
	  	perror("socket"); 
	  	exit(1);
	  }

	/* recherche de l'@ IP de la machine distante */
	if ((hote = gethostbyname(argv[3])) == NULL) {
	  	perror("gethostbyname"); 
	  	close(sock); 
	  	exit(2);
	}

	/* pr'eparation de l'adresse distante : port + la premiere @ IP */
	adresseReceveur.sin_family = AF_INET;
	adresseReceveur.sin_port = htons(atoi(argv[1]));
	bcopy(hote->h_addr, &adresseReceveur.sin_addr, hote->h_length);
	printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseReceveur.sin_addr));

	lgadresseReceveur = sizeof(adresseReceveur);

	//demande de participation
	if (gethostname(nomh, sizeof(nomh)) == -1) {
		perror("gethostname");
		exit(1);
	}
	//envoi nom de la machine
	if ((envoye = sendto(sock, nomh, sizeof(nomh), 0, (struct sockaddr *) &adresseReceveur, lgadresseReceveur)) != sizeof(nomh)) {
		perror("sendto");
		close(sock);
		exit(1);
	}
	printf("Demande de participation en cours...\n");

	//attente confirmation
	int conf = -1;
	if ((recu = recvfrom(sock, &conf, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) == -1) {
		perror("recvfrom confirmation"); 
		close(sock);
		exit(1);
	}
	if (conf == 1) {
		printf("Participation confirmee\n");
	} else {
		printf("Participation refusee\n");
		exit (1);
	}
	
	adresseReceveur.sin_port = htons(atoi(argv[2]));

	//attente description
	strcpy(buf, "");
	if ((recu = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) == -1) {
		perror("recvfrom buf"); 
		close(sock);
		exit(1);
	}
	printf("Description de l'objet: %s\n", buf);

	//attente prix
	if ((recu = recvfrom(sock, &prix, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) == -1) {
		perror("recvfrom prix"); 
		close(sock);
		exit(1);
	}
	printf("Prix: %d\n", prix);

	int continuer = 0;

	while (continuer == 0) {

		printf("Saisissez offre:\n");
		scanf("%d", &offre);

		if ((envoye = sendto(sock, &offre, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, lgadresseReceveur)) != sizeof(int)) {
			perror("sendto");
			close(sock);
			exit(1);
		}
		printf("Offre envoyée\n");

		//reception offre courante
		if ((recu = recvfrom(sock, &prix, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) == -1) {
			perror("recvfrom offreCourante"); 
			close(sock);
			exit(1);
		}
		printf("Offre courante = %d euros\n", prix);

		//reception datagramme vide
		int rien;
		if ((recu = recvfrom(sock, &rien, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) == -1) {
			perror("recvfrom buf"); 
			close(sock);
			exit(1);
		}

		if (recu == 0) {

			printf("Adjugé vendu!\n");

			if(offre == prix) {
				printf("Vous avez gagné la vente!\n");
			}
			continuer = 1;

		} else {

			printf("...\n");
		}
		
	}

	close(sock);

	return 1;
}