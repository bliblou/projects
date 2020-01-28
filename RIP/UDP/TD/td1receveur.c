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

/* receveur portReceveur */
int main (int argc, char **argv) {

	int sock,recu,envoye;
	char confirmation[256];
	char nomh[50];
	struct sockaddr_in adresseLocale;
	int lgadresseLocale;
	struct sockaddr_in adresseEmetteur;
	int lgadresseEmetteur;
	struct hostent *hote;
	char descriptionVente[80];
	int offreInitiale;

	/* cr'eation de la socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0 )) == -1) {

		perror("socket");
		exit(1);
	}

	/* r'ecup'eration du nom de la machine pr'esente */
	if (gethostname(nomh, 50) == -1) {

		perror("gethostbyname");
		exit(1);
	}

	printf("Je m'execute sur %s\n", nomh);

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

	/* reception de la description */
	lgadresseEmetteur = sizeof(adresseEmetteur);

	if ((recu = recvfrom(sock, descriptionVente, sizeof(descriptionVente), 0, (struct sockaddr *) &adresseEmetteur , &lgadresseEmetteur)) == -1 ) {

		perror("recvfrom descriptionVente");
		exit(1);
	}

	printf("j'ai recu la description : %s\n", descriptionVente);

	/* reception de l'offre initiale */
	if ((recu = recvfrom(sock, &offreInitiale, sizeof(int), 0, (struct sockaddr *) &adresseEmetteur, &lgadresseEmetteur)) == -1 ) {

		perror("recvfrom offreInitiale");
		exit(1);
	}

	printf("j'ai recu l'offre initiale : %d\n", offreInitiale);

	/* envoi de la confirmation de reception */
	strcpy(confirmation, "conditions initiales recues");

	if ((envoye = sendto(sock, confirmation, sizeof(confirmation), 0, (struct sockaddr *) &adresseEmetteur, lgadresseEmetteur)) != sizeof(confirmation)) {

		perror("sendto");
		exit(1);
	}

	printf("confirmation envoyee\n");

	return 0;
}