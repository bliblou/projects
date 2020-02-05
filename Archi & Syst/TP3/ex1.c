#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>

void main() {

	int c = sched_getcpu();
	printf("Coeur %d\n",c);

	int x;

	while(1) {

		x = sched_getcpu();

		if(x != c) {
			printf("Coeur %d\n",c);
			c = x;
		}
	}

}