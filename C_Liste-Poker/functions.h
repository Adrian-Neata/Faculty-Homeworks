/*NEATA Adrian - 311CB*/
#include <ctype.h>
#include <stdio.h>
#include "info.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void GasesteMasa(ALista ant, ALista aux, Sala sala, char *numeMasa)
{
	Masa masa = NULL;
	*ant = NULL;
	*aux = sala->masa;
	while(*aux != NULL){
		masa = (Masa)(*aux)->info;
		if(strcmp(masa->numeMasa, numeMasa) == 0)
			return;
		*ant = *aux;
		*aux = (*aux)->urm;
	}
}

void StergeMasa(TLista ant, TLista aux, Sala sala)
{
	Masa masa = NULL;
	masa = (Masa)aux->info;
	sala->nrLocMax -= masa->nrMaxJucatori;
	free(masa->numeMasa);
	free(masa->jucatori);
	free(masa);
	if(ant)
		ant->urm = aux->urm;
	else sala->masa = aux->urm;
	sala->nrMese--;
	free(aux);
}

void print(FILE * out, TLista masa)
{
	TLista aux = NULL, celula = NULL;
	Masa m = NULL;
	Jucator juc;
	for(aux = masa; aux != NULL; aux = aux->urm){
		m = (Masa)aux->info;
		fprintf(out, "%s:", m->numeMasa);
		for(celula = m->jucatori->urm; celula->urm != m->jucatori; celula = celula->urm){
			juc = (Jucator)celula->info;
			fprintf(out, " %s - %d;", juc->nume, juc->nrMaini);
		}
		juc = (Jucator)celula->info;
		fprintf(out, " %s - %d.\n", juc->nume, juc->nrMaini);
	}
}

int TuraMasa(Masa masa) 
{
	TLista aux = NULL, ant = NULL, elim = NULL;
	Jucator juc = NULL;
	int ok = 1, nrJucElim = 0;
	ant = masa->jucatori;
	aux = masa->jucatori->urm;
	while(aux != masa->jucatori){
		juc = (Jucator)aux->info;
		juc->nrMaini--;
		if(juc->nrMaini <= 0){
			if(ant == masa->jucatori)
				ok = 0;
			//verifica daca primul jucator este cel eliminat
			ant->urm = aux->urm;
			free(juc->nume);
			free(juc);
			elim = aux;
			aux = aux->urm;
			free(elim);
			masa->nrCrtJucatori--;
			nrJucElim++;
		} else{
			ant = aux;
			aux = aux->urm;
		}
		//sterge jucatorii care au pierdut
	}
	if(ok){
		aux = masa->jucatori->urm;
		ant->urm = aux;
		masa->jucatori->urm = aux->urm;
		aux->urm = masa->jucatori;
	}
	//schimba ordinea jucatorilor daca primul jucator nu a fost eliminat
	return nrJucElim;
	//returneaza numarul de jucatori eliminati
}

int tura(Sala sala, char* numeMasa)
{
	Masa masa = NULL;
	TLista aux = NULL, ant = NULL;
	int nrJucElim = 0;
	GasesteMasa(&ant, &aux, sala, numeMasa);
	if(!aux) return 0;
	masa = (Masa)aux->info;
	nrJucElim = TuraMasa(masa);
	//scade cu 1 mainile jucatorilor si elimina
	sala->nrLocCrt -= nrJucElim;
	if(masa->jucatori == masa->jucatori->urm){
		StergeMasa(ant, aux, sala);
	}
	//elimina masa daca nu mai sunt jucatori
	return 1;
}

int ghinion_noroc(char * numeMasa, char * numeJuc, int noroc, Sala sala)
{
	TLista aux = NULL, ant = NULL;
	TLista masa_ant = NULL, masa_aux = NULL;
	Masa masa = NULL;
	Jucator juc = NULL;
	GasesteMasa(&masa_ant, &masa_aux, sala, numeMasa);
	if(!masa_aux) return -1;
	//returneaza -1 daca numele mesei nu e gasit
	masa = (Masa)masa_aux->info;
	ant = masa->jucatori;
	aux = masa->jucatori->urm;
	while(aux != masa->jucatori){
		juc = (Jucator)aux->info;
		if(strcmp(juc->nume, numeJuc) == 0){
			juc->nrMaini += noroc;
			//schimba norocul
			if(juc->nrMaini <= 0){
				ant->urm = aux->urm;
				free(juc->nume);
				free(juc);
				free(aux);
				masa->nrCrtJucatori--;
				sala->nrLocCrt--;
			}
			//sterge jucatorul daca este necesar
			if(masa->jucatori == masa->jucatori->urm){
				StergeMasa(masa_ant, masa_aux, sala);	
			}
			//sterge masa daca este necesar
			return 1;
			//returneaza 1 daca a avut loc cu succes
		}
		ant = aux;
		aux = aux->urm;
	}
	return 0;
	//returneaza 0 daca numele jucatorului nu e gasit
}

void MutareJucatori(Masa masa1, Masa masa2)
{
	TLista aux = NULL, sf = NULL;
	sf = masa1->jucatori->urm;
	if(masa1->nrCrtJucatori == masa1->nrMaxJucatori)
		return;
	//verifica daca masa1 mai are locuri
	while(sf->urm != masa1->jucatori)
		sf = sf->urm;
	//gaseste ultimul jucator din masa1
	aux = masa2->jucatori;
	while(masa1->nrCrtJucatori < masa1->nrMaxJucatori && masa2->nrCrtJucatori > 0){
		aux = aux->urm;
		masa1->nrCrtJucatori++;
		masa2->nrCrtJucatori--;
	}
	//gaseste ultimul jucator care trebuie mutat din masa2
	sf->urm = masa2->jucatori->urm;
	masa2->jucatori->urm = aux->urm;
	aux->urm = masa1->jucatori;
	//muta jucatorii
}
int inchide(Sala sala, char* numeMasa)
{
	TLista aux = NULL, inc = NULL, ant = NULL;
	Masa masa = NULL;
	inc = sala->masa;
	GasesteMasa(&ant, &aux, sala, numeMasa);
	if(!aux) return -1;
	//returneaza -1 daca nu s-a gasit masa
	masa = (Masa)aux->info;
	if(sala->nrLocMax - masa->nrMaxJucatori < sala->nrLocCrt)
		return 0;
	//returneaza 0 daca nu sunt locuri suficiente in sala
	while(masa->nrCrtJucatori != 0){
		if(inc != aux)
			MutareJucatori(inc->info, masa);
		inc = inc->urm;
	}
	//muta jucatorii la celelalte mese
	StergeMasa(ant, aux, sala);
	//sterge masa din sala
	return 1;
	//returneaza 1 daca a avut loc cu succes mutarea
}

void tura_completa(Sala sala)
{
	TLista aux = NULL, ant = NULL;
	Masa masa = NULL;
	ant = sala->masa;
	aux = ant->urm;
	while(aux != NULL){
		masa = (Masa)ant->info;
		tura(sala, masa->numeMasa);
		ant = aux;
		aux = aux->urm;
	}
	masa = (Masa)ant->info;
	tura(sala, masa->numeMasa);
}

TLista OrdonareMasa(TLista Jucatori)
{
	TLista aux = NULL, inc = NULL, celula = NULL, ant = NULL, L = NULL;
	Jucator juc1 = NULL, juc2 = NULL;
	L = Jucatori->urm;
	while(L != Jucatori){
		celula = (TLista)malloc(sizeof(Celula));
		if(!celula){
			while(inc != NULL){
				ant = inc;
				inc = inc->urm;
				free(ant);
			}
			return NULL;
		}
		//sterge elementele deja alocate daca nu s-a alocat cu succes
		celula->urm = NULL;
		celula->info = L->info;
		juc1 = (Jucator)celula->info;
		if(inc) juc2 = (Jucator)inc->info;
		if(inc == NULL){
			inc = celula;
		} else if(juc2->nrMaini < juc1->nrMaini){
			celula->urm = inc;
			inc = celula;
			//cazul in care jucatorul are numarul de maini mai mare decat primul
		} else{
			if(juc2->nrMaini == juc1->nrMaini && strcmp(juc2->nume, juc1->nume) < 0){
				celula->urm = inc;
				inc = celula;
			} else{
				ant = inc;
				aux = inc->urm;
				for(; aux != NULL; ant = aux, aux = aux->urm){
					juc2 = (Jucator)aux->info;
					if(juc2->nrMaini <= juc1->nrMaini)
						break;
				}
				//cauta in jucatori pe cineva cu un scor mai mic
				for(; aux != NULL && juc2->nrMaini == juc1->nrMaini; ant = aux, aux = aux->urm){
					juc2 = (Jucator)aux->info;
					if(strcmp(juc2->nume, juc1->nume) < 0 || juc2->nrMaini != juc1->nrMaini)
						break;
				}
				//cauta, daca este cazul, pe cineva cu un nume lexicografic mai mic
				celula->urm = ant->urm;
				ant->urm = celula;
			}
		}
		L = L->urm;
	}
	return inc;
}


int clasament(FILE * f, Sala sala, char* numeMasa)
{
	Masa masa = NULL;
	TLista ant = NULL, L = NULL, aux = NULL;
	Jucator juc = NULL;
	GasesteMasa(&ant, &aux, sala, numeMasa);
	if(!aux) return 0;
	masa = (Masa)aux->info;
	L = (TLista)OrdonareMasa(masa->jucatori);
	if(!L) return -1;
	//daca nu este suficient loc pentru o noua lista ordonata
	fprintf(f, "Clasament %s:", numeMasa);
	while(L != NULL){
		juc = (Jucator)L->info;
		fprintf(f, "\n%s %d", juc->nume, juc->nrMaini);
		ant = L;
		L = L->urm;
		free(ant);
	}
	fprintf(f, "\n");
	return 1;
}

void parsare(char* cmd, FILE* out, Sala sala)
{ 
	const char* delims = " \n";
	char* comanda = strtok(cmd, delims);
	if(comanda){
		if (strcmp(comanda, "print") == 0) {
			if(sala->masa)
				print(out, sala->masa);
			else fprintf(out, "%s\n", "Sala este inchisa!");
		} else if (strcmp(comanda, "tura") == 0) {
			char* numeMasa = strtok(NULL, delims);
			if(!tura(sala, numeMasa))
				fprintf(out, "Masa %s nu exista!\n", numeMasa);  
		} else if (strcmp(comanda, "ghinion") == 0) {
			int ghinion = 0;
			char* numeMasa = strtok(NULL, delims);
			char* numeJuc = strtok(NULL, delims);
			char* index = strtok(NULL, delims);
			if(index)
				ghinion = atoi(index);
			int rez = ghinion_noroc(numeMasa, numeJuc, -ghinion, sala);
			if(rez == -1)
				fprintf(out, "Masa %s nu exista!\n", numeMasa);
			if(rez == 0)
				fprintf(out, "Jucatorul %s nu exista la masa %s!\n", numeJuc, numeMasa);
		} else if (strcmp(comanda, "noroc") == 0) {
			int noroc = 0;
			char* numeMasa = strtok(NULL, delims);
			char* numeJuc = strtok(NULL, delims);
			char* index = strtok(NULL, delims);
			if(index)
				noroc = atoi(index);
			int rez = ghinion_noroc(numeMasa, numeJuc, noroc, sala);
			if(rez == -1)
				fprintf(out, "Masa %s nu exista!\n", numeMasa);
			else if(rez == 0)
				fprintf(out, "Jucatorul %s nu exista la masa %s!\n", numeJuc, numeMasa);
		} else if (strcmp(comanda, "tura_completa") == 0) {
			tura_completa(sala);
		} else if (strcmp(comanda, "clasament") == 0) {
			char* numeMasa = strtok(NULL, delims);
			if(!clasament(out, sala, numeMasa))
				fprintf(out, "Masa %s nu exista!\n", numeMasa);
		} else if (strcmp(comanda, "inchide") == 0) {
			char* numeMasa = strtok(NULL, delims);
			int rez = inchide(sala, numeMasa);
			if(rez == -1)
				fprintf(out, "Masa %s nu exista!\n", numeMasa);
			else if(rez == 0)
				fprintf(out, "Nu exista suficiente locuri in sala!\n");
			
		}
	}
}
