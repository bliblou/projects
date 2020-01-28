/* Lancement d'un serveur : serveur_tcp port */
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
		if (read(socket_service, buf, 256) < 0) {
			perror("read");
			exit(6);
		}

		printf("Chaine recue : %s\n", buf);
		fflush(stdout);

		entier_envoye = 2006;

		if (write(socket_service, &entier_envoye, sizeof(int)) != sizeof(int)) {
			perror("write");
			exit(7);
		}

		printf("Entier envoye : %d\n", entier_envoye);
		close(socket_service);
	}

	close(socket_RV);

	return 0;
}