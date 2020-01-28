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

int main(int argc, char *argv[]) {

	int socket_RV, socket_service;
	char buf[256];
	int entier_envoye;
	struct sockaddr_in adresseRV;
	int lgadresseRV;
	struct sockaddr_in adresseClient;
	int lgadresseClient;
	struct hostent *hote;
	unsigned short port;

	/* creation de la socket de RV */
	if ((socket_RV = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* preparation de l'adresse locale */
	port = (unsigned short) atoi(argv[1]);
	adresseRV.sin_family = AF_INET;
	adresseRV.sin_port = htons(port);
	adresseRV.sin_addr.s_addr = htonl(INADDR_ANY);
	lgadresseRV = sizeof(adresseRV);

	/* attachement de la socket a l'adresse locale */
	if ((bind(socket_RV, (struct sockaddr *) &adresseRV, lgadresseRV)) == -1) {
		perror("bind");
		exit(3);
	}
	/* declaration d'ouverture du service */
	if (listen(socket_RV, 10)==-1) {
		perror("listen");
		exit(4);
	}

	//ouverture du fichier destination
	int fddst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0640);

	/* boucle d'attente de connexion */
	while (1) {

		printf("Debut de boucle\n");
		fflush(stdout);

		/* attente d'un client */
		lgadresseClient = sizeof(adresseClient);
		socket_service=accept(socket_RV, (struct sockaddr *) &adresseClient, &lgadresseClient);

		if (socket_service==-1 && errno==EINTR) {
			/* reception d'un signal */
			continue;
		}

		if (socket_service==-1) {
			/* erreur plus grave */
			perror("accept");
			exit(5);
		}

		/* un client est arrive */
		printf("connexion acceptee\n");
		fflush(stdout);

		/* lecture dans la socket d'une chaine de caractères */
		int n = read(socket_service, buf, sizeof(buf));

		if (n < 0) {
			perror("read");
			exit(6);
		}

		while ((n != 0) && (n != -1)) {

			write(fddst, buf, n);

			printf("Nombre d'octets recus : %d\n", n);
			fflush(stdout);

			n = read(socket_service, buf, sizeof(buf));
		}

		if (n < 0) {
			perror("read");
			close(fddst);
			exit(6);
		}
	}

	close(socket_RV);
	close(fddst);

	return 0;
}