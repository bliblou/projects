#define _GNU_SOURCE
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

int main(int argc, char const *argv[]) {

	FILE *fd = fopen(argv[1], "w");

	int a,b,c,d;
	int nbMult;

	char buf[512];
	char s[4];
	
	printf("Nombre d'itérations?\n");
	scanf("%d", &nbMult);
	sprintf(buf, "%d\n", nbMult);

	for(int i=0;i<nbMult;i++) {

		printf("Itération %d\n",i+1);

		printf("Taille matrice 1? (lignes, colonnes)\n");
		scanf("%d", &a);
		sprintf(s, "%d ", a);
		strcat(buf, s);

		scanf("%d", &b);
		sprintf(s, "%d\n", b);
		strcat(buf, s);

		int M1[a][b];

		printf("Taille matrice 2? (lignes, colonne)\n");
		scanf("%d", &c);
		sprintf(s, "%d ", c);
		strcat(buf, s);

		scanf("%d", &d);
		sprintf(s, "%d\n", d);
		strcat(buf, s);

		int M2[c][d];

		for(int i=0;i<a;i++) {
			for(int j=0;j<b;j++) {
				printf("[M1] Valeur(%d,%d) ?\n", i, j);
				scanf("%d", &M1[i][j]);
				sprintf(s, "%d ", M1[i][j]);
				strcat(buf, s);
			}
			strcat(buf, "\n");
		}

		for(int i=0;i<c;i++) {
			for(int j=0;j<d;j++) {
				printf("[M2]Valeur (%d,%d) ?\n", i, j);
				scanf("%d", &M2[i][j]);
				sprintf(s, "%d ", M2[i][j]);
				strcat(buf, s);
			}
			strcat(buf, "\n");
		}

	}

	printf("Done.");

	fputs(buf, fd);
	fclose(fd);

	return 0;
}