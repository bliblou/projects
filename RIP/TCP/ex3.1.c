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
# include <sys/stat.h>
# include <fcntl.h>

int main (int argc, char *argv[]) {

	int fdsrc, fddst;

	fdsrc = open(argv[1], O_RDONLY);
	if (fdsrc == -1) {
		perror("open"); 
		return 1;
	}

	fddst = creat(argv[2], 0666);
	if (fddst == -1) {
		perror("open");
		close (fdsrc);
		return 2;
	}

	int lus;
	char buf[30];

	lus = read(fdsrc, buf, 30);

	while ((lus!=0) && (lus!=-1)) {

		write(fddst, buf, lus);
		lus = read(fdsrc, buf, 30);
	}

	if (lus ==-1) {

		perror("read");
		close(fdsrc);
		close(fddst);
		return 3;
	}

	close(fdsrc);
	close(fddst);

	return 0;
}