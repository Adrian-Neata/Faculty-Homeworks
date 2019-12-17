/*NEATA Adrian - 311CB*/
#include <stdio.h>
#include "info.h"
#include <stdlib.h>
#include <string.h>

void EliberareJucatori(ALista L)
{
	TLista aux =NULL, ant= NULL;
	Jucator juc = NULL;
	ant = (*L)->urm;
	aux = ant;
	//incepe stergerea de la celula de dupa santinela

	while(aux != *L){
		aux = aux->urm;
		juc = (Jucator)ant->info;
		free(juc->nume);
		free(juc);
		free(ant);
		ant = aux;
	}
	//sterge celulele jucatorilor

	free(*L);
	//sterge santinela
}

void EliberareMese(ALista L)
{
	TLista aux = NULL, ant = NULL;
	Masa masa = NULL;
	ant = *L;
	aux = ant;
	while(ant != NULL){
		aux = aux->urm;
		masa = (Masa)ant->info;
		free(masa->numeMasa);
		EliberareJucatori(&(masa->jucatori));
		free(masa);
		free(ant);
		ant = aux;
	}
}

TLista GenerareJucatori(FILE * f, int nrJuc)
{
	char numeJuc[100];
	int i, nrMaini;
	TLista aux = NULL, santinela = NULL, ant = NULL;
	Jucator juc = NULL;
	//declarare variabile

	santinela = (TLista)malloc(sizeof(Celula));
	if(!santinela) return NULL;
	ant = santinela;
	for(i = 0; i < nrJuc; i++)
	{
		aux = (TLista)malloc(sizeof(Celula));
		if(!aux){
			ant->urm = santinela;
			break;
		}
		juc = (Jucator)malloc(sizeof(struct jucator));
		if(!juc){
			free(aux);
			ant->urm = santinela;
			break;
		}
		fscanf(f, "%s %d", numeJuc, &nrMaini);
		juc->nume = strdup(numeJuc);
		if(juc->nume == NULL){
			free(juc);
			free(aux);
			ant->urm = santinela;
			break;
		}
		juc->nrMaini = nrMaini;
		aux->info = juc;
		ant->urm = aux;
		ant = aux;
	}
	//alocare celule jucatori

	if(i < nrJuc){
		EliberareJucatori(&santinela);
		return NULL;
	}
	aux->urm = santinela;
	//finalizare lista circulara

	return santinela;
}

void GenerareSala(FILE * f, Sala* sala)
{
	int nrCrtJuc, nrMaxJuc, i, N;
	char numeMasa[100];
	TLista aux = NULL, ant = NULL, inc = NULL;
	Masa masa = NULL;
	fscanf(f, "%d", &N);
	*sala = (Sala)malloc(sizeof(struct sala));
	if(!(*sala)) return;
	(*sala)->nrLocCrt = 0;
	(*sala)->nrLocMax = 0;
	(*sala)->nrMese = N;
	for(i = 0; i < N; i++)
	{
		aux = (TLista)malloc(sizeof(Celula));
		if(!aux) break;
		masa = (Masa)malloc(sizeof(struct masa));
		if(!masa){
			free(aux);
			break;
		}
		fscanf(f, "%s %d %d", numeMasa, &nrCrtJuc, &nrMaxJuc);
		masa->numeMasa = strdup(numeMasa);
		if(masa->numeMasa == NULL){
			free(masa);
			free(aux);
			break;
		}
		masa->jucatori = GenerareJucatori(f, nrCrtJuc);
		if(masa->jucatori == NULL){
			free(masa->numeMasa);
			free(masa);
			free(aux);
			break;
		}
		masa->nrCrtJucatori = nrCrtJuc;
		masa->nrMaxJucatori = nrMaxJuc;
		(*sala)->nrLocCrt += masa->nrCrtJucatori;
		(*sala)->nrLocMax += masa->nrMaxJucatori;
		aux->info = masa;
		aux->urm = NULL;
		if(ant){
			ant->urm = aux;
			ant = aux;
		} else{
			ant = aux;
			inc = aux;
		}
	}

	if(i != N){
		EliberareMese(&inc);
		return;
	}
	(*sala)->masa = inc;
}

