/*NEATA Adrian - 311CB*/
#include<stdio.h>
#include<stdlib.h>
#include "info.h"
#include<stdint.h>
#include<string.h>

void* InitS(size_t dim)
{
	ASt a = malloc(sizeof(TStiva));
	if(!a) return NULL;
	a->dim = dim;
	a->vf = NULL;
	return a;
}

void* InitQ(size_t dim)
{
	AQ a = malloc(sizeof(TCoada));
	if(!a) return NULL;
	a->dim = dim;
	a->ic = NULL;
	a->sc = NULL;
	return a;
}

int Pop(ASt a, ALista ae)
{
	if(a->vf == NULL) return 0;
	*ae = a->vf;
	a->vf = a->vf->urm;
	a->dim--;
	return 1;
}

void Push(ASt a, TLista ae)
{
	ae->urm = a->vf;
	a->vf = ae; 
	a->dim++;
}

int ExtrQ(AQ a, ALista ae)
{
	if(a->ic == NULL) return 0;
	*ae = a->ic;
	a->ic = a->ic->urm;
	a->dim--;
	if(a->dim == 0)
		a->sc = NULL;
	return 1;
}

void IntrQ(AQ a, TLista ae)
{
	if(a->sc) 
		a->sc->urm = ae;
	else a->ic = ae;
	a->sc = ae;
	a->dim++;
}

void delSt(ASt stiva)
{
	TLista ae = NULL;
	while(stiva->vf != NULL){
		Pop(stiva, &ae);
		free(ae->info);
		free(ae);
	}
}

void delQ(AQ coada)
{
	TLista ae = NULL;
	while(coada->ic != NULL){
		ExtrQ(coada, &ae);
		free(ae->info);
		free(ae);
	}
}

void DelSist(ALista Hanoi)
{
	ASist a = NULL;
	TLista L = NULL, ant = NULL;
	L = *Hanoi;
	while(L != NULL){
		a = (ASist)L->info;
		free(a->culoare);
		delSt(a->A);
		delSt(a->B);
		delSt(a->C);
		free(a->A);
		free(a->B);
		free(a->C);
		delQ(a->moves);
		free(a->moves);
		free(a);
		ant = L;
		L = L->urm;
		free(ant);
	}
	free(Hanoi);
}

TLista IntrSist(char* culoare)
{
	TLista Hanoi = NULL;
	Hanoi = (TLista)malloc(sizeof(Celula));
	if(Hanoi == NULL) return NULL;
	ASist a = (ASist)malloc(sizeof(Sist));
	if(a == NULL){
		free(Hanoi);
		return NULL;
	}
	a->A = (ASt)InitS(0);
	if(a->A == NULL){
		free(a);
		free(Hanoi);
		return NULL;
	}
	a->B = (ASt)InitS(0);
	if(a->B == NULL){
		free(a->A);
		free(a);
		free(Hanoi);
		return NULL;
	}
	a->C = (ASt)InitS(0);
	if(a->C == NULL){
		free(a->B);
		free(a->A);
		free(a);
		free(Hanoi);
		return NULL;
	}
	a->moves = (AQ)InitQ(0);
	if(a->moves == NULL){
		free(a->C);
		free(a->B);
		free(a->A);
		free(a);
		free(Hanoi);
		return NULL;
	}
	a->culoare = strdup(culoare);
	if(a->culoare == NULL){
		free(a->moves);
		free(a->C);
		free(a->B);
		free(a->A);
		free(a);
		free(Hanoi);
		return NULL;
	}
	a->A->vf = NULL;
	a->B->vf = NULL;
	a->C->vf = NULL;
	a->nrMoves = 0;
	Hanoi->info = a;
	Hanoi->urm = NULL;
	return Hanoi;
}

TLista GasesteSist(ALista L, char* culoare)
{
	TLista aux = NULL;
	ASist sist = NULL;
	aux = *L;
	if(*L == NULL){
		*L = IntrSist(culoare);
		return *L;
	}
	//cazul in care lista de sisteme e goala

	while(aux != NULL){
		sist = (ASist)aux->info;
		if(strcmp(sist->culoare, culoare) == 0)
			return aux;
		aux = aux->urm;
	}
	//cauta culoare in lista de sisteme
	if(aux == NULL){
		aux = *L;
		while(aux->urm != NULL)
			aux = aux->urm;
		aux->urm = IntrSist(culoare);
		if(aux->urm == NULL){
			DelSist(L);
			return NULL;
		}
		aux = aux->urm;
	}
	//daca nu gaseste culoare, creeaza un nou sistem
	return aux;
}

void PrintStiva(FILE* out, ASt stiva, ALista Hanoi)
{
	ASt auxSt = NULL;
	TLista ae = NULL;
	auxSt = (ASt)InitS(0);
	if(auxSt == NULL){
		DelSist(Hanoi);
		return;
	}
	auxSt->vf = NULL;
	while(stiva->vf != NULL){
		Pop(stiva, &ae);
		Push(auxSt, ae);
	}
	while(auxSt->vf != NULL){
		Pop(auxSt, &ae);
		fprintf(out, " %d", ((ADisc)(ae->info))->diametru);
		Push(stiva, ae);
	}
	free(auxSt);
}

int Muta(ASt a, ASt b, ASist sist, int which)
{
	int reverse = 0;
	TLista ae = NULL, aux = NULL;
	if(a->vf == NULL && b->vf == NULL) return 1;
	if(a->vf == NULL){
		reverse = 1;
		Pop(b, &ae);
		Push(a, ae);
	} else if(b->vf == NULL){
		Pop(a, &ae);
		Push(b, ae);
	} else if(((ADisc)(a->vf->info))->diametru < ((ADisc)(b->vf->info))->diametru){
		Pop(a, &ae);
		Push(b, ae);
	} else{
		reverse = 1;
		Pop(b, &ae);
		Push(a, ae);
	}
	//muta discul pe o alta stiva

	sist->nrMoves++;
	aux = (TLista)malloc(sizeof(Celula));
	if(!aux) return 0;
	aux->urm = NULL;
	if(which == 0){
		if(!reverse) aux->info = strdup("A->B");
		else aux->info = strdup("B->A");
	}
	if(which == 1){
		if(!reverse) aux->info = strdup("A->C");
		else aux->info = strdup("C->A");
	}
	if(which == 2){
		if(!reverse) aux->info = strdup("B->C");
		else aux->info = strdup("C->B");
	}
	if(aux->info == NULL){
		free(aux);
		return 0;
	}
	IntrQ(sist->moves, aux);
	//introduce o noua mutare in coada cu mutari

	return 1;
}

int solve(int nrMutari, ASt A, ASt B, ASt C, ASist sist)
{
	int totalMoves;
	totalMoves = (1 << (A->dim + B->dim + C->dim)) - 1;
	if((A->dim + B->dim + C->dim) % 2 == 0)
	while(sist->nrMoves <= totalMoves && nrMutari > 0){
		if(sist->nrMoves % 3 == 0 && nrMutari){
			if(!Muta(A, B, sist, 0)) return 0;
			nrMutari--;
		}
		if(sist->nrMoves % 3 == 1 && nrMutari){
			if(!Muta(A, C, sist, 1)) return 0;
			nrMutari--;
		}
		if(sist->nrMoves % 3 == 2 && nrMutari){
			if(!Muta(B, C, sist, 2)) return 0;
			nrMutari--;
		}
		if(nrMutari == 0) break;
	} else 
	while(sist->nrMoves < totalMoves && nrMutari > 0){
		if(sist->nrMoves % 3 == 0 && nrMutari){
			if(!Muta(A, C, sist, 1)) return 0;
			nrMutari--;
		}
		if(sist->nrMoves % 3 == 1 && nrMutari){
			if(!Muta(A, B, sist, 0)) return 0;
			nrMutari--;
		}
		if(sist->nrMoves % 3 == 2 && nrMutari){
			if(!Muta(B, C, sist, 2)) return 0;
			nrMutari--;
		}
		if(nrMutari == 0) break;
	}
	return 1;
}

