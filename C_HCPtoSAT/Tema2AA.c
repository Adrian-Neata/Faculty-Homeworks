#include<stdio.h>
#include<stdlib.h>

struct neighbourNodes{
	int* neighbours;
	int len;
}*nodes;

int cmpfunc(const void* a, const void* b){
	return (*(int*) a - *(int*) b);
}

void printPossiblePaths(FILE* output, int* neighbours, int len, int crtNode, int pathLength, int nrNodes){
	int k;
	for(k = 0; k < len; k++){
		if(neighbours[k] != 1){
			fprintf(output, "(a%d-%d&", pathLength - 1, neighbours[k]);
			fprintf(output, "x%d-%d)", neighbours[k], crtNode);
			if(k != len - 1){
				fprintf(output, "|");
			}
		}
	}
	fprintf(output, ")&~(");
	for(k = 1; k <= pathLength - 1 && k < nrNodes/2 + 1; k++){
		fprintf(output, "a%d-%d", k, crtNode);
		if(k != pathLength - 1 && k != nrNodes/2)
			fprintf(output, "|");
	}
	fprintf(output, ")))");
}

int main(){
	int nrNodes, a, b;
	int *nodeOrder;
	int* neighbours;
	int len, i, j, k, crtNode, pathLength;

	FILE *input = fopen("graph.in", "rt");
	FILE *output = fopen("bexpr.out", "wt");

	fscanf(input, "%d", &nrNodes);
	nodes = calloc(nrNodes + 1, sizeof(struct neighbourNodes));
	if(nodes == NULL){
		printf("Not enough memory space\n");
		return 0;		
	}
	for(i = 1; i <= nrNodes; i++){
		nodes[i].neighbours = malloc(nrNodes * sizeof(int));
		if(nodes[i].neighbours == NULL){
			for(j = i - 1; j >= 1; j--)
				free(nodes[j].neighbours);
			printf("Not enough memory space\n");
			return 0;
		}
	}
	nodeOrder = calloc(nrNodes, sizeof(int));
	if(nodeOrder == NULL){
		for(i = 1; i <= nrNodes; i++)
			free(nodes[i].neighbours);
		free(nodes);
		printf("Not enough memory space\n");
		return 0;
	}
	//initializes the variables

	fscanf(input, "%d", &a);
	while(a != -1){
		fscanf(input, "%d", &b);
		nodeOrder[b - 1]++;
		nodeOrder[a - 1]++;
		nodes[a].neighbours[nodes[a].len] = b;
		nodes[a].len++;
		nodes[b].neighbours[nodes[b].len] = a;
		nodes[b].len++;
		fscanf(input, "%d", &a);
	}
	//reads the edges and calculates each node's order

	for(i = 0; i < nrNodes; i++){
		if(nodeOrder[i] < 2){
			fprintf(output, "x1-1 & ~x1-1");
			return 0;
		}
	}//if the order of a node is less than 2 then it can't be a Hamiltonian graph

	for(i = 1; i <= nrNodes; i++){
		qsort(nodes[i].neighbours, nodes[i].len, sizeof(int), cmpfunc);
	} //sorts the nodes in order

	for(crtNode = 1; crtNode <= nrNodes; crtNode++){ //for1

		len = nodes[crtNode].len;
		neighbours = nodes[crtNode].neighbours;
		fprintf(output, "(");
		for(i = 0; i < len; i++){ //for2
			for(j = i + 1; j < len; j++){ //for3
				fprintf(output, "(");
				for(k = 0; k < len; k++){ //for4
					if(k == i || k == j){
						fprintf(output, "x%d-%d", crtNode, neighbours[k]);
					} else{
						fprintf(output, "~x%d-%d", crtNode, neighbours[k]);
					}
					if(k != len - 1){
						fprintf(output, "&");
					}
				}
				if(i == len -2 && j == len - 1){
					fprintf(output, ")");
				} else{
					fprintf(output, ")|");
				}
			}
		}//step 1A, (check README)
		//picks every 2 egdes coming from a node and negates the others (divides the graph in subgraphs each having only 2 vertices coming from a node)
		fprintf(output, ")&");
		if(crtNode != 1){
			fprintf(output, "(");
			for(i = 1; i <= nrNodes/2 + 1; i++){ //for5
				fprintf(output, "a%d-%d", i, crtNode);
				if(i != nrNodes/2 + 1){
					fprintf(output, "|");
				}	
			}
			fprintf(output, ")&");
		}//step 1B, (check README)
		//makes sure every node is connected to node 1 and thus connected with any other node
	}

	for(crtNode = 1; crtNode <= nrNodes; crtNode++){
		len = nodes[crtNode].len;
		neighbours = nodes[crtNode].neighbours;
		for(i = 0; i < len; i++){
			if(neighbours[i] > crtNode){
				fprintf(output, "((x%d-%d|", crtNode, neighbours[i]);
				fprintf(output, "~x%d-%d)&", neighbours[i], crtNode);
				fprintf(output, "(~x%d-%d|", crtNode, neighbours[i]);
				fprintf(output, "x%d-%d))&", neighbours[i], crtNode);
			}
		}//step 1C, (check README)
	}//takes in consideration the orientation of the edge and makes sure no two oriented edges have the same nodes

	len = nodes[1].len;
	neighbours = nodes[1].neighbours;
	for(i = 0; i < len; i++){
		fprintf(output, "((a1-%d|", neighbours[i]);
		fprintf(output, "~x1-%d)&", neighbours[i]);
		fprintf(output, "(~a1-%d|", neighbours[i]);
		fprintf(output, "x1-%d))&", neighbours[i]);
	}//checks if node 1 is connected with any other node except the ones given as input

	i = 0;
	for(crtNode = 1; crtNode <= nrNodes; crtNode++){
		if(neighbours[i] != crtNode){
			fprintf(output, "~a1-%d&", crtNode);
		} else {
			if(i < len - 1)
				i++;
		}//step 1D, (check README)
	}
	//makes sure no other edges except the ones given from input are connected to node 1

	for(pathLength = 2; pathLength <= nrNodes/2 + 1; pathLength++){
		for(crtNode = 2; crtNode <= nrNodes; crtNode++){
			if(pathLength != 2 || crtNode != 2)
				fprintf(output, "&");
			len = nodes[crtNode].len;
			neighbours = nodes[crtNode].neighbours;

			fprintf(output, "((a%d-%d|~((", pathLength, crtNode);
			printPossiblePaths(output, neighbours, len, crtNode, pathLength, nrNodes);
			//makes sure you are in the right subpath

			fprintf(output, "&(~a%d-%d|((", pathLength, crtNode);
			printPossiblePaths(output, neighbours, len, crtNode, pathLength, nrNodes);
			fprintf(output, ")");
			//makes sure you are in the right subpath (other case)
		}//step 2, (check README)
	}
	//checks every possible path depending on the subgraph
	for(i = 1; i <= nrNodes; i++){
		free(nodes[i].neighbours);
	}
	free(nodes);
	free(nodeOrder);
	fclose(input);
	fclose(output);
	return 0;
}
