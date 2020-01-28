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

	int sock,recu,envoye;
	char buf[256], nomh[50];
	struct sockaddr_in adr;
	int lgadr;
	struct hostent *hote;

	/* cr'eation de la socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	/* r'ecup'eration du nom de la machine presente */
	if (gethostname(nomh, 50) == -1) {
		perror("gethostname");
		exit(1);
	}

	printf("Je m'execute sur %s\n", nomh);
	/* pr'eparation de l'adresse locale : port + toutes les @ IPadr.sin_family = AF_INET;*/
	adr.sin_family = AF_INET;
	adr.sin_port = htons(atoi(argv[1]));
	adr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* attachement de la socket a` l'adresse locale */
	lgadr = sizeof(adr);
	if ((bind(sock, (struct sockaddr *) &adr, lgadr)) == -1) {
		perror("bind");
		exit(1);
	}

	int x;
	printf("Entrez entier x = ");
	scanf("%d", &x);

	/* 'echange de datagrammes */
	lgadr = sizeof(adr);
	if ((recu = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &adr, &lgadr)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	printf("j'ai recu %s\n", buf);

	if ((recu = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *) &adr, &lgadr)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	printf("j'ai recu %s\n", buf);

	return 1;
}