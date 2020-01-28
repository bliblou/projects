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

int main(int argc, char **argv) {

	int sock, envoye, recu;
	char buf[256];
	struct sockaddr_in adr;
	int lgadr;
	struct hostent *hote;

	/* cr'eation de la socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* recherche de l'@ IP de la machine distante */
	if ((hote = gethostbyname(argv[2])) == NULL) {

		perror("gethostbyname");
		exit(2);
	}

	/* pr'eparation de l'adresse distante : port + la premier @ IP */
	adr.sin_family = AF_INET;
	adr.sin_port = htons(atoi(argv[1]));
	bcopy(hote->h_addr, &adr.sin_addr, hote->h_length);
	printf("L'adresse en notation pointee %s\n", inet_ntoa(adr.sin_addr));

	/* echange de datagrammes */
	lgadr = sizeof(adr);

	strcpy(buf, "heyoooooo BITCH wanna suck my DICK?");
	if ((envoye = sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *) &adr, lgadr)) != sizeof(buf)) {
		perror("sendto");
		exit(1);
	}

	printf("msg envoye\n");

	strcpy(buf, "FUCK COMPUTER SCIENCE");
	if ((envoye = sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *) &adr, lgadr)) != sizeof(buf)) {
		perror("sendto");
		exit(1);
	}

	printf("msg envoye\n");

	return 1;
}
