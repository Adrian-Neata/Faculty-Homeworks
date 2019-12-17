#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <pthread.h>

int numSamples, numThreads;
double *samples;
double complex *freq;

void * ft(void * args) {
	int i, j, start, finish;
	int thread_id = *(int *)args;
	start = thread_id * (numSamples / numThreads);
	if(thread_id == numThreads - 1) {
		finish = numSamples;
	} else {
		finish = (thread_id + 1) * (numSamples / numThreads);
	}
	//splits the calculations
	for(i = start; i < finish; i++) {
		for(j = 0; j < numSamples; j++) {
			freq[i] += samples[j] * cexp(((-2) * M_PI * I) / 
					numSamples * i * j);
		}
	}
	//calculates the frequencies
	return NULL;
}
int main(int argc, char * argv[]) {
	
	int ret, i;

	if (argc < 3) {
		fprintf(stdout, "Usage %s <input file> <output file> <num threads>\n", argv[0]);
		exit(1);
	}
	
	FILE *input = fopen(argv[1], "rt");
	FILE *output = fopen(argv[2], "wt");
	numThreads = atoi(argv[3]);

	if (input == NULL) {
		fprintf(stdout, "Failed to open the input file.\n");
		exit(1);
	}

	if (output == NULL) {
		fprintf(stdout, "Failed to open the output file.\n");
		exit(1);
	}
	//opens the input and output files

	ret = fscanf(input, "%d", &numSamples);
	if (ret != 1) {
		fprintf(stdout, "Failed to read numSamples from the input file.\n");
		exit(1);
	}

	samples = (double*)malloc(numSamples * sizeof(double));
	if(samples == NULL) {
		fprintf(stdout, "Failed to allocate memory for vector samples.\n");
		exit(1);
	}

	freq = (double complex*)calloc(numSamples, sizeof(double complex));
	if(freq == NULL) {
		free(samples);
		fprintf(stdout, "Failed to allocate memory for vector freq.\n");
		exit(1);
	}
	
	for(i = 0; i < numSamples; i++){
		ret = fscanf(input, "%lf", samples + i);
		if (ret != 1) {
			fprintf(stdout, "Failed to read samples[%d] from the input file.\n", i);
			exit(1);
		}
	}
	//reads samples from input file

	pthread_t tid[numThreads];
	int thread_id[numThreads];
	for(i = 0;i < numThreads; i++) {
		thread_id[i] = i;
	}

	for(i = 0; i < numThreads; i++) {
		pthread_create(&(tid[i]), NULL, ft, &(thread_id[i]));
	}

	for(i = 0; i < numThreads; i++) {
		pthread_join(tid[i], NULL);
	}
	//runs threads

	fprintf(output, "%d\n", numSamples);
	for(i = 0; i < numSamples; i++) {
		fprintf(output, "%lf %lf\n", creal(freq[i]), cimag(freq[i]));
	}
	//prints the output (frequencies)

	free(samples);
	free(freq);
	return 0;
}
