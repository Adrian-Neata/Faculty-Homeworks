/*NEATA Adrian - 311CB*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<math.h>

#ifndef _ARBORE_
#define _ARBORE_

typedef struct variabila {
	int val;
	char* nume;
} *TVar;

typedef struct nod {
	char* info;
	struct nod *st, *dr;
} TNod, *TArb, **AArb;

int citireVar(FILE* in, TVar Var, int nrVar);
char** Parsare(char* ecuatie, int* n);
void ConstrArb(char** element, AArb Arb, int n, int* i, int* error);
int RSD(FILE* out, TArb a, TVar Var, int n, int* error);
void DelArb(AArb a);
int comp(char* element);
#endif
