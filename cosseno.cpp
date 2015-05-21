#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <semaphore.h>
#include <gmpxx.h>

using namespace std;


void cosseno(int numThreads);
void *calculaTermo();

pthread_t *threads;

int main (int argc, char *argv[])
{
  /*int f = 1;
    int numThreads = 10;*/
  return 0;
}


void cosseno(int numThreads)
{
  int i;
  int *thread_args;

  threads = malloc(numThreads*sizeof(pthread_t));
  thread_args = malloc(numThreads*sizeof(int));

  for(i=0; i<numThreads; i++) thread_args[i] = i;

  for(i = 0; i < numThreads; i++)
    if(pthread_create(&threads[i], NULL, calculaTermo, (void *) &thread_args[i])) 
      abort();
  
  for (i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);
}


void *calculaTermo(void *i)
{
  int num = *((int *) i);
  printf("%d\n", num);

  return NULL;
}
