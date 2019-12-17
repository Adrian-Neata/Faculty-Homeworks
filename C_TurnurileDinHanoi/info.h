/*NEATA Adrian - 311CB*/
#include <stdio.h>

#ifndef _HANOI_
#define _HANOI_

typedef struct disc
{
	int diametru;
} TDisc, *ADisc;

typedef struct celulag
{
	void *info;
	struct celulag *urm;
} Celula, *TLista, **ALista;

typedef struct stiva
{ 
	size_t dim;        
	TLista vf;         
} TStiva, *ASt;

typedef struct coada
{ 
	size_t dim;   
	TLista ic, sc;   
} TCoada, *AQ;

typedef struct hanoi
{
	int nrMoves;
	char* culoare;
	ASt A, B, C;
	AQ moves;
} Sist, *ASist;

#endif
