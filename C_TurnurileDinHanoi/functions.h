/*NEATA Adrian - 311CB*/
#include<stdio.h>
#include<stdlib.h>
#include "info.h"
#include<stdint.h>
#include<string.h>
#include"auxfunc.h"

int add(char *culoare, int dim, ALista Hanoi)
{
	TLista ae = NULL, aux = NULL, L = NULL;
	ADisc disc = NULL, disc_aux = NULL;
	ASist sist = NULL;
	ASt auxSt = NULL;
	//declarare si initializare variabile
	disc = (ADisc)malloc(sizeof(TDisc));
	if(disc == NULL){
		DelSist(Hanoi);
		return 0;
	}
	disc->diametru = dim;
	//creaza discul care trebuie adaugat

	ae = (TLista)malloc(sizeof(Celula));
	if(ae == NULL){
		free(disc);
		DelSist(Hanoi);
		return 0;
	}
	ae->info = disc;
	ae->urm = NULL;
	//creaza celula in care se afla discul

	L = GasesteSist(Hanoi, culoare);
	if(L == NULL){
		free(disc);
		free(ae);
		return 0;
	}
	sist = (ASist)L->info;
	auxSt = (ASt)InitS(0);
	if(auxSt == NULL){
		free(disc);
		free(ae);
		DelSist(Hanoi);
		return 0;
	}
	//creaza stiva auxiliara pentru mutat discuri
	if(sist->A->vf != NULL)
		disc_aux = (ADisc)sist->A->vf->info;
	while(sist->A->vf != NULL && disc_aux->diametru < dim){
		Pop(sist->A, &aux);
		Push(auxSt, aux);
		if(sist->A->vf)
			disc_aux = (ADisc)sist->A->vf->info;
	}
	Push(sist->A, ae);
	//gaseste locul unde trebuie adaugat discul in sistem si il adauga

	while(auxSt->vf != NULL){
		Pop(auxSt, &aux);
		Push(sist->A, aux);
	}
	//pune inapoi elementele in stiva
	free(auxSt);
	return 1;
}

int show(FILE* out, char* culoare, ALista Hanoi)
{
	TLista L = NULL;
	ASist sist = NULL;
	L = GasesteSist(Hanoi, culoare);
	if(L == NULL) return 0;
	sist = (ASist)L->info;
	fprintf(out, "A_%s:", culoare);
	PrintStiva(out, sist->A, Hanoi);
	fprintf(out, "\n");
	fprintf(out, "B_%s:", culoare);
	PrintStiva(out, sist->B, Hanoi);
	fprintf(out, "\n");
	fprintf(out, "C_%s:", culoare);
	PrintStiva(out, sist->C, Hanoi);
	fprintf(out, "\n");
	return 1;
}

int play(int nrMutari, ASt A, ASt B, ASt C, ASist sist)
{
	int totalMoves;
	TLista ae = NULL;
	totalMoves = (1 << (A->dim + B->dim + C->dim)) - 1;
	//retine numarul total de mutari care trebuie facut pentru a rezolva sistemul

	if(sist->moves->ic == NULL){
		if(!solve(totalMoves, A, B, C, sist))
			return 0;
		while(C->vf != NULL){
			Pop(C, &ae);
			Push(B, ae);
		}
		while(B->vf != NULL){
			Pop(B, &ae);
			Push(A, ae);
		}
		sist->nrMoves = 0;
	}
	//pune toate mutarile necesare rezolvarii sistemului in coada

	if(!solve(nrMutari, A, B, C, sist))
		return 0;
	//face ce ar fi trebuit sa faca de la inceput :)
	return 1;
}

int show_moves(FILE* out, char* culoare, int nrMutari, ALista Hanoi)
{
	int i = 0, totalMoves;
	AQ coada = NULL;
	TLista L = NULL, ae = NULL;
	ASist sist = NULL;
	L = GasesteSist(Hanoi, culoare);
	if(L == NULL){
		DelSist(Hanoi);
		return 0;
	}
	sist = (ASist)L->info;
	totalMoves = (1 << (sist->A->dim + sist->B->dim + sist->C->dim)) - 1;
	//retine numarul total de mutari care trebuie facut pentru a rezolva sistemul

	if(sist->moves->ic == NULL){
		if(!solve(totalMoves, sist->A, sist->B, sist->C, sist))
			return 0;
		while(sist->C->vf != NULL){
			Pop(sist->C, &ae);
			Push(sist->B, ae);
		}
		while(sist->B->vf != NULL){
			Pop(sist->B, &ae);
			Push(sist->A, ae);
		}
		sist->nrMoves = 0;
	}
	//pune toate mutarile necesare rezolvarii sistemului in coada

	fprintf(out, "M_%s:", culoare);
	coada = InitQ(0);
	if(coada == NULL){
		DelSist(Hanoi);
		return 0;
	}
	while(ExtrQ(sist->moves, &ae)){
		i++;
		if(i > sist->nrMoves && i <= sist->nrMoves + nrMutari && i <= totalMoves)
			fprintf(out, " %s", (char*)ae->info);
		IntrQ(coada, ae);
	}
	//parcurge coada si afiseaza toate mutarile necesare

	while(ExtrQ(coada, &ae)){
		IntrQ(sist->moves, ae);
	}
	//pune la loc elementele in coada de mutari
	free(coada);
	fprintf(out, "\n");
	return 1;
}

void parsare(char* cmd, FILE* out, ALista Hanoi)
{ 
	const char* delims = " \n";
	char* comanda = strtok(cmd, delims);
	if(comanda){
		if (strcmp(comanda, "add") == 0) {
			char* culoare = strtok(NULL, delims);
			char* dimensiune = strtok(NULL, delims);
			int dim = atoi(dimensiune);
			if(!add(culoare, dim, Hanoi)) return;
		} else if (strcmp(comanda, "play") == 0) {
			char* culoare = strtok(NULL, delims);
			char* mutari = strtok(NULL, delims);
			int nrMutari = atoi(mutari);
			TLista L = NULL;
			ASist sist = NULL;
			L = GasesteSist(Hanoi, culoare);
			if(L == NULL){
				DelSist(Hanoi);
				return;
			}
			sist = (ASist)L->info;
			if(!play(nrMutari, sist->A, sist->B, sist->C, sist)){
				DelSist(Hanoi);
				return;
			}
		} else if (strcmp(comanda, "show") == 0) {
			char* culoare = strtok(NULL, delims);
			if(!show(out, culoare, Hanoi)) return;
		} else if (strcmp(comanda, "show_moves") == 0) {
			char* culoare = strtok(NULL, delims);
			char* mutari = strtok(NULL, delims);
			int nrMutari = atoi(mutari);
			if(!show_moves(out, culoare, nrMutari, Hanoi)) return;
		}
	}
}
