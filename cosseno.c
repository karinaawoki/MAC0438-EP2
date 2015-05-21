#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


void cosseno();
void *calculaTermo();

pthread_t *threads;

int numThreads;
int parar;

int main (int argc, char *argv[])
{
	/*int f = 1; */
	numThreads = 10;
	return 0;
}


void cosseno()
{
	int i;
	int *thread_args;

	threads = malloc(numThreads*sizeof(pthread_t));
	thread_args = malloc(numThreads*sizeof(int));

	for(i=0; i<numThreads; i++)
	{
		thread_args[i] = i;
	}

	for(i = 0; i < numThreads; i++)
    {
        if(pthread_create(&threads[i], NULL, calculaTermo, (void *) &thread_args[i])) 
            abort();
    }
    for (i = 0; i < numThreads; i++)
        pthread_join(threads[i], NULL);
}


void *calculaTermo(void *i)
{
	int num = *((int *) i);
	int rodada = 0, iteracao;
	while(1)
	{
		iteracao = rodada*numThreads + num;
		printf("%d\n", iteracao);

	}
	return NULL;
}

int fatorial(int num)
{
	int fatorial = 1;
	while(num!=1)
	{
		fatorial *= num;
	}

	return fatorial;
}