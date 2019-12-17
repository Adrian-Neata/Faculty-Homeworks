#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <pthread.h>

double *samples;
double complex *freq;
pthread_barrier_t *barrier;
int numSamples, numThreads;

double complex * recursiveFFT(double *partialSamples, int n, int step) {
	int i;
	double complex t;
	double complex *ret;
	double complex *ret1, *ret2;
	ret = (double complex *)malloc(n * sizeof(double complex)); 
	if(n == 1) {
		ret[0] = partialSamples[0];
	} else {
		ret1 = recursiveFFT(partialSamples, n / 2, 2 * step);
		ret2 = recursiveFFT(partialSamples + step, n / 2, 2 * step);
		for(i = 0; i < n / 2; i++) {
			ret[i] = ret1[i];
		}
		for(i = n / 2; i < n; i++) {
			ret[i] = ret2[i - n / 2];
		}
		free(ret1);
		free(ret2);
		for(i = 0; i < n / 2; i++) {
			t = ret[i];
			ret[i] = t + cexp(((-2) * M_PI * I)  * i / n) * ret[i + n / 2];
			ret[i + n / 2] = t - cexp(((-2) * M_PI * I)  * i / n) * ret[i + n / 2];
		}
	}
	//recursive version of FFT taken from wikipedia
	return ret;
}

void* fft(void *args) {
	int i;
	double complex t;
	double complex *ret;
	int thread_id = *(int*)args;
	switch(numThreads) {
		case 1:
			free(freq);
			freq = recursiveFFT(samples, numSamples, 1);
			break;
		case 2:
			if(thread_id == 0) {
				ret = recursiveFFT(samples, numSamples / 2, 2);
				for(i = 0; i < numSamples / 2; i++) {
					freq[i] = ret[i];
				}
				free(ret);
				pthread_barrier_wait(barrier);
				for(i = 0; i < numSamples / 2; i++) {
					t = freq[i];
					freq[i] = t + cexp(((-2) * M_PI * I)  * i / 
						numSamples) * freq[i + numSamples / 2];
					freq[i + numSamples / 2] = t - cexp(((-2) * M_PI * I)  * i / 
						numSamples) * freq[i + numSamples / 2];
				}
			} else {
				ret = recursiveFFT(samples + 1, numSamples / 2, 2);
				for(i = numSamples / 2; i < numSamples; i++) {
					freq[i] = ret[i - numSamples / 2];
				}
				free(ret);
				pthread_barrier_wait(barrier);
			}
			break;
		case 4:
			switch(thread_id) {
				case 0:
					ret = recursiveFFT(samples, numSamples / 4, 4);
					for(i = 0; i < numSamples / 4; i++) {
						freq[i] = ret[i];
					}
					free(ret);
					pthread_barrier_wait(&(barrier[0]));
					for(i = 0; i < numSamples / 4; i++) {
						t = freq[i];
						freq[i] = t + cexp(((-2) * M_PI * I)  * i / 
							(numSamples / 2)) * freq[i + numSamples / 4];
						freq[i + numSamples / 4] = t - cexp(((-2) * M_PI * I)  * i / 
							(numSamples / 2)) * freq[i + numSamples / 4];
					}
					pthread_barrier_wait(&(barrier[2]));
					break;
				case 1:
					ret = recursiveFFT(samples + 2, numSamples / 4, 4);
					for(i = numSamples / 4; i < numSamples / 2; i++) {
						freq[i] = ret[i - numSamples / 4];
					}
					free(ret);
					pthread_barrier_wait(&(barrier[0]));
					break;
				case 2:
					ret = recursiveFFT(samples + 1, numSamples / 4, 4);
					for(i = numSamples / 2; i < 3 * numSamples / 4; i++) {
						freq[i] = ret[i - numSamples / 2];
					}
					free(ret);
					pthread_barrier_wait(&(barrier[1]));
					for(i = numSamples / 2; i < 3 * numSamples / 4; i++) {
						t = freq[i];
						freq[i] = t + cexp(((-2) * M_PI * I)  * i / 
							(numSamples / 2)) * freq[i + numSamples / 4];
						freq[i + numSamples / 4] = t - cexp(((-2) * M_PI * I)  * i / 
							(numSamples / 2)) * freq[i + numSamples / 4];
					}
					pthread_barrier_wait(&(barrier[2]));
					for(i = 0; i < numSamples / 2; i++) {
						t = freq[i];
						freq[i] = t + cexp(((-2) * M_PI * I)  * i / 
							numSamples) * freq[i + numSamples / 2];
						freq[i + numSamples / 2] = t - cexp(((-2) * M_PI * I)  * i / 
							numSamples) * freq[i + numSamples / 2];
					}
					break;
				case 3:
					ret = recursiveFFT(samples + 3, numSamples / 4, 4);
					for(i = 3 * numSamples / 4; i < numSamples; i++) {
						freq[i] = ret[i - 3 * numSamples / 4];
					}
					free(ret);
					pthread_barrier_wait(&(barrier[1]));
					break;
			}
			break;
	}
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
	
	freq = (double complex *)malloc(numSamples * sizeof(double complex));
	if(freq == NULL) {
		fprintf(stdout, "Failed to allocate memory for vector freq.\n");
		free(samples);
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

	if(numThreads == 2) {
		barrier = (pthread_barrier_t *)malloc(sizeof(pthread_barrier_t));
		pthread_barrier_init(barrier, NULL, 2);
	} else if(numThreads == 4) {
		barrier = (pthread_barrier_t *)malloc(3 * sizeof(pthread_barrier_t));
		for(i = 0; i < 3; i++) {
			pthread_barrier_init(&(barrier[i]), NULL, 2);
		}
	}

	pthread_t tid[numThreads];
	int thread_id[numThreads];
	for(i = 0;i < numThreads; i++) {
		thread_id[i] = i;
	}

	for(i = 0; i < numThreads; i++) {
		pthread_create(&(tid[i]), NULL, fft, &(thread_id[i]));
	}

	for(i = 0; i < numThreads; i++) {
		pthread_join(tid[i], NULL);
	}

	if(numThreads == 2) {
		pthread_barrier_destroy(barrier);
	} else if(numThreads == 4) {
		for(i = 0; i < 3; i++) {
			pthread_barrier_destroy(&(barrier[i]));
		}
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
