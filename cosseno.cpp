#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <gmpxx.h>
#include <vector>

using namespace std;

void cosseno(int numThreads);
void *calculaTermo(void*);

vector<pthread_t> threads;

unsigned int numCores = 0;

int main (int argc, char *argv[])
{
  numCores = std::thread::hardware_concurrency();
  printf("numCores = %d\n",numCores);
  return 0;
}


void cosseno(int numThreads)
{
  int i;
  vector<int> thread_args;

  threads.resize(numThreads);
  thread_args.resize(numThreads);
  

  for(i=0; i<numThreads; i++) thread_args[i] = i;

  for(i = 0; i < numThreads; i++)
    if(pthread_create(&threads[i], NULL, calculaTermo,(void*)&thread_args[i]))
      abort();
  
  for (i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);
}


void *calculaTermo(void *i)
{
  int num = *((int *) i);
  printf("%d\n", num);

  return NULL;
}
