/* Lancement d'un client : client_tcp port machine_serveur fichier_source*/
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

int main (int argc, char *argv[]) {

	int sock;
	struct sockaddr_in adresse_serveur;
	struct hostent *hote;
	unsigned short port;

	/* creation de la socket locale */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* recuperation de l'adresse IP du serveur (a partir de son nom) */
	if ((hote = gethostbyname(argv[2])) == NULL) {
		perror("gethostbyname");
		exit(2);
	}

	/* preparation de l'adresse du serveur */
	port = (unsigned short) atoi(argv[1]);
	adresse_serveur.sin_family = AF_INET;
	adresse_serveur.sin_port = htons(port);
	bcopy(hote->h_addr, &adresse_serveur.sin_addr, hote->h_length);
	printf("L'adresse du serveur en notation pointee %s\n", inet_ntoa(adresse_serveur.sin_addr));
	fflush(stdout);

	/* demande de connexion au serveur */
	if (connect(sock, (struct sockaddr *) &adresse_serveur, sizeof(adresse_serveur))==-1) {
		perror("connect");
		exit(3);
	}

	/* le serveur a accepte la connexion */
	printf("connexion acceptee\n");
	fflush(stdout);

	//ouverture source
	int fsrc;
	fsrc = open(argv[3], O_RDONLY);
	if (fsrc == -1) {
		perror("open");
		return 1;
	}

	int lus;
	char buf[30];

	lus = read(fsrc, buf, sizeof(buf));

	while((lus!=0)&&(lus!=-1)) {

		write(sock, buf, lus);
		printf("Nb d'octets envoyés : %d\n", lus);
		fflush(stdout);

		lus = read(fsrc, buf, sizeof(buf));
	}

	if (lus < 0) {
		perror("read");
		close(fsrc);
		return 1;
	}

	close(fsrc);

	return 0;
}