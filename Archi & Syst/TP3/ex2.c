#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NBCOEUR 12

int main(int argc, char const *argv[]) {

	cpu_set_t s;
	CPU_ZERO(&s);

	unsigned int i;
	if (sscanf(argv[1], "%d", &i) != 1) {
		perror("sscanf");
		exit(1);
	}

	if ((i > NBCOEUR) || (i<=0)) {
		printf("Erreur, nombre max de coeur : %d\n", NBCOEUR);
		exit(1);
	}

	CPU_SET(i-1, &s);
	printf("Coeur n°%d insere\n", i);

	if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &s) != 0) {
		perror("setaffinity");
		exit (1);
	}

	printf("Coeur: %d\n", sched_getcpu()+1);

	sleep(5);

	printf("Coeur: %d\n", sched_getcpu()+1);

	CPU_ZERO(&s);

	return 1;
}