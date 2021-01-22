/* Lancement d'un serveur : serveur_tcp port fichier_destination*/
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
# include <fcntl.h>

int main(int argc, char *argv[]) {

	int socket_RV, socket_service;
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

	//ouverture destination
	int fdst;
	fdst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0640);

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

		if (fdst == -1) {
			perror("open");
			return 1;
		}

		int lus;
		char buf[30];

		lus = read(socket_service, buf, sizeof(buf));

		printf("Nb d'octets lus : %d\n", lus);
		fflush(stdout);

		while((lus!=0)&&(lus!=-1)) {

			write(fdst, buf, lus);

			lus = read(socket_service, buf, sizeof(buf));

			printf("Nb d'octets lus : %d\n", lus);
			fflush(stdout);
		}

		if (lus < 0) {
			perror("read");
			close(fdst);
			return 1;
		}

		close(fdst);
	}

	close(socket_RV);

	return 0;
}