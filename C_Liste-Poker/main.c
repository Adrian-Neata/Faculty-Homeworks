/*NEATA Adrian - 311CB*/
#include <stdio.h>
#include "info.h"
#include "functions.h"
#include <string.h>
#include "liste.h"

int main(int argc, char *argv[])
{
	Sala sala = NULL;
	int citire;
	char* linie = NULL;
	size_t lungime;
	FILE * config = fopen(argv[1], "rt");
	FILE * eveniment = fopen(argv[2], "rt");
	FILE * out = fopen(argv[3], "wt");
	//declarare variabile
	GenerareSala(config, &sala);
	//initializare sala
	while ((citire = getline(&linie, &lungime, eveniment)) != -1) 
		parsare(linie, out, sala);
	//parsare input + executarea comenzii
	free(linie);
	EliberareMese(&sala->masa);
	free(sala);
	fclose(config);
	fclose(eveniment);
	fclose(out);
	//eliberare zone de memorie alocate
	return 0;
}
