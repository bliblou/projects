#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define TRUE 1
#define BUFSIZE 512
#define NB_CLIENTS_MAX 10

int main ( int argc, char **argv) {

	int socket_RV, socket_service;
	char buf[BUFSIZE];
	struct sockaddr_in adr;
	int lgadr;
	struct sockaddr_in adresse;
	int lg_adresse;
	struct hostent *hote;
	u_short port;
	int n, max, i;
	fd_set fdset;

	int clients[NB_CLIENTS_MAX];/* tableau des sockets des clients   */
	int nbclients;

	if (argc != 2) {
		printf("Usage : executable port \n");
		exit(2);
	}

	lgadr = sizeof(adr);
	lg_adresse=sizeof(adresse);

	/* creation de la socket */
	if ((socket_RV = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* conversion du port passe en argument */
	port = atoi(argv[1]);
	/* attachement de la socket de RV au port */
	adr.sin_family = AF_INET;
	adr.sin_port = htons(port);
	adr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ((bind(socket_RV, (struct sockaddr *) &adr, lgadr)) == -1) {
		perror("bind");
		exit(1);
	}

	/* declaration d'ouverture du service */
	if (listen(socket_RV, 10)==-1) {
		perror("listen");
		exit(2);
	}

	/* On doit toujours scruter la socket de rendez-vous, on la met dans la case 0 */
	clients[0] = socket_RV;
	nbclients = 1;

	while (1) { //MacDo ouvert 24h/24

		/* construction de l'ens a surveiller */
		FD_ZERO(&fdset);
		/* calcul du maximum  et construction du FD_SET */max = 0;
		for (i=0;i<nbclients;i++) {

			FD_SET(clients[i], &fdset);

			if (max < clients[i]) {
				max = clients[i];
			}
		}

		/* select avec attente infinie */
		select(max+1, &fdset, NULL, NULL, NULL);

		/* retour du select */
		if (FD_ISSET(clients[0], &fdset)) { /* chouette, un nouveau client  : acceptation du nouveau client */

			socket_service = accept(socket_RV, (struct sockaddr *)  &adresse, &lg_adresse);

			if (socket_service==-1) {
				perror("accept");
				exit(2);
			}
			printf("connexion acceptee\n");
			fflush(stdout);

			clients[nbclients]=socket_service;
			nbclients++;
		} 

		/* test des clients actuels */
		for (i=1;i<nbclients;i++){

			if (FD_ISSET(clients[i], &fdset)){ /* c'est ce client qui veut qqch => on lit sa requete */

				if (read(clients[i], buf, BUFSIZE) == -1) { 
					perror("read");
					exit(1);
				}
				printf("le client %d veut un %s\n", i, buf);
				fflush(stdout);

				/* reponse a la requete */
				if (write(clients[i], buf, strlen(buf)+1) == -1) { 
					perror("write");
					exit(1);
				}
				printf("j'ai envoye un %s\n", buf);
				fflush(stdout);
			}	   
		} 
	// MacDo never stops
	}

	return 1;
}