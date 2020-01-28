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

	int port;
	int socket_client;
	struct hostent *hp;
	struct sockaddr_in adresse_serveur;
	char buf[256];

		/* creation de la socket locale */
	if ((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if (argc!=4) {

		fprintf(stderr, "Nombre de parametres incorrect\n");
		exit(2);
	}

	/* test d'existence du serveur */
	if ((hp=gethostbyname(argv[1]))==NULL) {

		fprintf(stderr, "Serveur %s inconnu\n", argv[1]);
		exit(2);
	}

	/* preparation de l'adresse du serveur */
	port = (unsigned short) atoi(argv[1]);
	adresse_serveur.sin_family = AF_INET;
	adresse_serveur.sin_port = htons(port);
	bcopy(hp->h_addr, &adresse_serveur.sin_addr, hp->h_length);
	
	printf("L'adresse du serveur en notation pointee %s\n", inet_ntoa(adresse_serveur.sin_addr));
	fflush(stdout);

	/* demande de connexion au serveur */
	if (connect(socket_client, (struct sockaddr *) &adresse_serveur, sizeof(adresse_serveur))==-1) {

		perror("connect");
		exit(2);
	}

	printf("connexion acceptee\n");
	fflush(stdout);

	/* à compléter en fonction de l’application */
	// ouverture du fichier
	int fdsrc = open(argv[3], O_RDONLY);

	if (fdsrc == -1) {

		perror("open");
		exit (1);
	}

	/* ecriture dans la socket d'une chaine */
	int n = read(fdsrc, buf, sizeof(buf));

	if (n < 0) {
		perror("read");
		close(fdsrc);
		exit(6);
	}

	while ((n != 0) && (n != -1)) {

		write(socket_client, buf, n);

		printf("Nombre d'octets envoyés: %d\n", n);
		fflush(stdout);

		n = read(fdsrc, buf, sizeof(buf));

		if (n < 0) {
			perror("read");
			close(fdsrc);
			exit(6);
		}
	}

	close(fdsrc);

	return 0;
}
