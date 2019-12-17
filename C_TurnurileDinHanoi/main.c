/*NEATA Adrian - 311CB*/
#include<stdio.h>
#include "functions.h"
#include "info.h"
#include<stdlib.h>

int main(int argc, char *argv[])
{
	int citire;
	char* linie = NULL;
	size_t lungime;
	ALista Hanoi = NULL;
	FILE * in = fopen(argv[1], "rt");
	FILE * out = fopen(argv[2], "wt");
	Hanoi = (ALista)malloc(sizeof(TLista));
	if(!Hanoi) return 0;
	*Hanoi = NULL;
	fscanf(in, "%d", &citire);
	while ((citire = getline(&linie, &lungime, in)) != -1) 
		parsare(linie, out, Hanoi);
	if(Hanoi) DelSist(Hanoi);
	free(linie);
	fclose(in);
	fclose(out);
	return 1;
}
