/*NEATA Adrian - 311CB*/
#include "info.h"

int main(int argc, char *argv[])
{
	int nrVar, nrOp, nrEl, val, i, error;
	TVar Var = NULL;
	char *linie = NULL, **element;
	size_t lungime;
	TArb Arb = NULL;
	FILE *in = fopen(argv[1], "rt");
	FILE *out = fopen(argv[2], "wt");
	//declarare variabile si initializare


	fscanf(in, "%d", &nrVar);
	Var = (TVar)malloc(nrVar * sizeof(struct variabila));
	if(!Var && nrVar){
		fclose(in);
		fclose(out);
		return 0;
	}
	getline(&linie, &lungime, in);
	//alocare lista de variabile pentru arbore


	if(!citireVar(in, Var, nrVar)){
		fclose(in);
		fclose(out);
		free(Var);
		return 0;
	}
	//initializare lista de variabile pentru arbore


	fscanf(in, "%d", &nrOp);
	getline(&linie, &lungime, in);
	for(i = 1; i <= nrOp; i++){
 		getline(&linie, &lungime, in);
		element = Parsare(linie, &nrEl);
		val = 0;
		error = 0;

		ConstrArb(element, &Arb, nrEl, &val, &error);
		if(error){
			DelArb(&Arb);
			free(element);
			for(i = 1; i <= nrVar; i++)
				free(Var[i - 1].nume);
			free(Var);
			free(linie);
			fclose(in);
			fclose(out);
			return 0;
		}

		error = 0;
		val = RSD(out, Arb, Var, nrVar, &error);
		if(!error)
			fprintf(out, "%d\n", val);
		if(error == 1)
			fprintf(out, "Invalid expression\n");

		DelArb(&Arb);
		free(element);
	}
	//construieste arborii si calculeaza rezultatul final

	for(i = 1; i <= nrVar; i++)
		free(Var[i - 1].nume);
	//elibereaza lista de variabile

	free(Var);
	free(linie);
	fclose(in);
	fclose(out);
	return 0;
}
