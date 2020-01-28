/* emetteur portReceveur machineReceveur N*/

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
	if ((hote = gethostbyname(argv[2])) == NULL) {
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

	strcpy(buf, nomh);
	if ((envoye = sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *) &adresseReceveur, lgadresseReceveur)) != sizeof(buf)) {
		perror("sendto");
		close(sock);
		exit(1);
	}
	printf("Demande de participation en cours...\n");

	//attente confirmation
	strcpy(buf, "");
	if ((recu = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) != sizeof(buf)) {
		perror("recvfrom confirmation"); 
		close(sock);
		exit(1);
	}
	printf("Participation confirmee\n");

	//attente description
	strcpy(buf, "");
	if ((recu = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) != sizeof(buf)) {
		perror("recvfrom buf"); 
		close(sock);
		exit(1);
	}
	printf("Description de l'objet: %s\n", buf);

	//attente prix
	if ((recu = recvfrom(sock, &prix, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) != sizeof(int)) {
		perror("recvfrom prix"); 
		close(sock);
		exit(1);
	}
	printf("Prix: %d\n", prix);

	printf("Saisissez offre:");
	scanf("%d", &offre);

	if(offre > 0) {

		//envoi offre
		if ((envoye = sendto(sock, &offre, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, lgadresseReceveur)) != sizeof(int)) {
			perror("sendto");
			close(sock);
			exit(1);
		}
		printf("Offre envoyée\n");

	} else {
		//lol
	}

	//reception offre courante
	if ((recu = recvfrom(sock, &prix, sizeof(int), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) != sizeof(int)) {
		perror("recvfrom offreCourante"); 
		close(sock);
		exit(1);
	}
	printf("Offre courante = %d euros\n", prix);

	//reception datagramme vide
	strcpy(buf, "");
	if ((recu = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &adresseReceveur, &lgadresseReceveur)) != sizeof(buf)) {
		perror("recvfrom buf"); 
		close(sock);
		exit(1);
	}
	if(buf == "") {

		printf("Adjuge vendu!");

	} else {

		//lol
	}

	close(sock);

	return 1;
}