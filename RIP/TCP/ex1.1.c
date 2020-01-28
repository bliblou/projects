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

	struct hostent *h;
	struct in_addr addr;

	if (gethostname(argv[1], 30)<0) {
		perror("gethostname");
	} else {
		printf("Nom : %s\n", argv[1]);
	}

	h = gethostbyname(argv[1]);

	if (h == NULL) {
		perror("gethostbyname");
		exit (-1);
	}

	bcopy(h->h_addr, &addr, h->h_length);

	printf("IP entier :%u\n", addr.s_addr);
	printf("IP pointé :%s\n", inet_ntoa(addr));

	return 0;
}