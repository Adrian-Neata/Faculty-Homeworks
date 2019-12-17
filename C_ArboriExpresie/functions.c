/*NEATA Adrian - 311CB*/
#include "info.h"

TArb ConstrFr(char* x)
{
	TArb aux = (TArb)malloc (sizeof(TNod));
	if (!aux) return NULL;
	aux->info = x;
	aux->st = NULL;
	aux->dr = NULL;
	return aux;
}

void ConstrArb(char** element, AArb Arb, int n, int* i, int* error)
{
	int aux;
	if(*i == n)
		return;
	*Arb = ConstrFr(element[*i]);
	if(*Arb == NULL){
		*error = 1;
		return;
	}


	if(comp(element[*i]) == 0)
		return;
	aux = *i;
	(*i)++;
	ConstrArb(element, &(*Arb)->st, n, i, error);


	if(comp(element[aux]) < 6){
		(*i)++;
		ConstrArb(element, &(*Arb)->dr, n, i, error);
	}
}

void DelArb(AArb a)
{
	if(!(*a)) return;
	DelArb(&(*a)->st);
	DelArb(&(*a)->dr);
	if(*a) free(*a);
	return;
}

int citireVar(FILE* in, TVar Var, int nrVar)
{
	int i, n, j;
	char nume[15];
	for(i = 1; i <= nrVar; i++){
		fscanf(in, "%s", nume);
		Var[i - 1].nume = strdup(nume);
		//adauga numele variabilei in lista 


		if(Var[i - 1].nume == NULL){
			for(j = i - 1; j > 0; j--)
				free(Var[j - 1].nume);
			return 0;
		}
		//verifica daca s-a alocat cu succes


		fscanf(in, "%s %d", nume, &n);
		Var[i - 1].val = n;
		//adauga valoarea variabilei
	}
	return 1;
}


char** Parsare(char* ecuatie, int* n)
{
	int i = 0;
	char *p = NULL, *linie = NULL, **element;
	*n = 0;
	linie = (char*)malloc((strlen(ecuatie) + 1)* sizeof(char));
	if(linie == NULL) return NULL;
	strcpy(linie, ecuatie);
	//copie in linie ecuatia initiala



	p = strtok(linie, " \n");
	while(p != NULL){
		(*n)++;
		p = strtok(NULL, " \n");
	}
	//numara cate elemente se afla in ecuatie



	element = (char**)malloc((*n) * sizeof(char*));
	if(element == NULL){
		free(linie);
		return NULL;
	}
	p = strtok(ecuatie, " \n");
	while(p != NULL){
		element[i] = p;
		i++;
		p = strtok(NULL, " \n");
	}
	//retine in locuri diferite fiecare element din ecuatie


	free(linie);
	return element;
}

int comp(char* element)
{
	if(strlen(element) == 1){
		if(*element == '+')
			return 1;
		if(*element == '-')
			return 2;
		if(*element == '/')
			return 3;
		if(*element == '*')
			return 4;
		if(*element == '^')
			return 5;
	}
	if(strcmp(element, "sqrt") == 0)
		return 6;
	return 0;
}



int search(FILE* out, char *s, TVar Var, int n, int *isNumber)
{
	int i;
	for(i = 0; i < n; i++)
		if(strcmp(s, Var[i].nume) == 0)
			return Var[i].val;
	//cauta in lista de variabile elementul

	for(i = 0; i < strlen(s); i++)
		if(s[i] < '0' || s[i] > '9'){
			if(i == 0 && s[i] == '-')
				break;
			*isNumber = 2;
			fprintf(out, "Variable %s undeclared\n", s);
			return 2;
		}
	//verifica daca string-ul este un numar

	return atoi(s);
}



int RSD(FILE* out, TArb a, TVar Var, int n, int* error)
{
	if(*error != 0) return 1;
	if(a->st == NULL && a->dr == NULL){
		int b = search(out, a->info, Var, n, error);
		return b;



	}else if(comp(a->info) == 1)
		return RSD(out, a->st, Var, n, error) + RSD(out, a->dr, Var, n, error);



	else if(comp(a->info) == 2)
		return RSD(out, a->st, Var, n, error) - RSD(out, a->dr, Var, n, error);



	else if(comp(a->info) == 3){
		if(RSD(out, a->dr, Var, n, error) == 0){
			*error = 1; 
			return 1;
		} else return RSD(out, a->st, Var, n, error) / RSD(out, a->dr, Var, n, error);



	} else if(comp(a->info) == 4)
		return RSD(out, a->st, Var, n, error) * RSD(out, a->dr, Var, n, error);



	else if(comp(a->info) == 5)
		return pow(RSD(out, a->st, Var, n, error), RSD(out, a->dr, Var, n, error));



	else {
		int c = RSD(out, a->st, Var, n, error);
		if(c < 0){
			*error = 1; 
			return 1;
		} else return (int)(sqrt(c));
	}
}

