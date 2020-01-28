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

/* emetteur portReceveur machineReceveur */
int main(int argc, char **argv) {

	int sock, envoye, recu;
	char confirmation[256];
	struct sockaddr_in adresseReceveur;
	int lgadresseReceveur;
	struct hostent *hote;
	char descriptionVente[80];
	int offreInitiale = 149;
	strcpy(descriptionVente, "Téléphone portable");

	/* cr'eation de la socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM , 0 )) == -1) {

		perror("socket");
		exit(1);
	}

	/* recherche de l'@ IP de la machine distante */
	if ((hote = gethostbyname(argv[2])) == NULL) {

		perror("gethostbyname");
		exit(2);
	}

	/* pr'eparation de l'adresse distante : port + la premier @ IP */
	adresseReceveur.sin_family = AF_INET;
	adresseReceveur.sin_port = htons(atoi(argv[1]));
	bcopy(hote->h_addr, &adresseReceveur.sin_addr, hote->h_length);
	printf("L'adresse en notation pointee %s\n", inet_ntoa(adresseReceveur.sin_addr));

	/* envoi de la description */
	lgadresseReceveur = sizeof(adresseReceveur);

	if ((envoye = sendto(sock, descriptionVente, sizeof(descriptionVente), 0, (struct sockaddr *) &adresseReceveur, lgadresseReceveur)) != sizeof(descriptionVente)) {

		perror("sendto descriptionVente");
		exit(1);
	}

	printf("descriptionVente envoyee\n");

	/* envoi de l'offre initiale */
	if ((envoye = sendto(sock, &offreInitiale, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, lgadresseReceveur)) != sizeof(int)) {

		perror("sendto offreInitiale");
		exit(1);
	}

	printf("offreInitiale envoyee\n");

	/* confirmation de reception */
	if ((recu = recvfrom(sock, confirmation, sizeof(confirmation), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) == -1) {

		perror("recvfrom");
		exit(1);
	}

	printf("confirmation recue.");

	return 0;
}
