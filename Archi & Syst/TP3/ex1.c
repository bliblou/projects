#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>

void main() {

	while(1) {

		printf("Coeur %d\n",sched_getcpu());
	}

}