#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <gmpxx.h>
#include <vector>

#include "mathFunctions.hpp"

using namespace std;

void cosseno(int numThreads);
void *calculaTermo(void*);

vector<pthread_t> threads;
pthread_barrier_t barreira; 

unsigned int numCores = 0;


int numThreads;
int parar;
int somaTermos;
int quantosPassaram;
int valorUltimaThread;
int valorPenultimaThread;

int main (int argc, char *argv[]){
  numCores = thread::hardware_concurrency();
  printf("numCores = %d\n",numCores);

  mpf_set_default_prec(1000000);

  mpf_t resp,base;
  mpf_init(resp);
  mpf_init_set_si(base,40320);
  mpf_pow_ui(resp,base,40320);
  cout << resp << endl;
  
  numThreads = 10;
  return 0;
}

void cosseno(int numThreads){
  int i;
  vector<int> thread_args;

  threads.resize(numThreads);
  thread_args.resize(numThreads);

  quantosPassaram = 0;
  

  for(i=0; i<numThreads; i++) thread_args[i] = i;

  for(i = 0; i < numThreads; i++)
    if(pthread_create(&threads[i], NULL, calculaTermo,(void*)&thread_args[i]))
      abort();
  
  for (i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);
}


void *calculaTermo(void *i){
  int num = *((int *) i);
  int rodada = 0, n;
  int termo;

  while(1){
    
    n = rodada*numThreads + num;


    termo[num] = 1.0*menosUmElevadoAn(x)*potencia(x, 2*n) /fatorial(2*n);
    pthread_barrier_wait(&barreira);

    if(num == numThreads-2) valorPenultimaThread = termo[num];
    else if (num == numThreads-1) valorUltimaThread = termo[num];
    sem_wait(mutexSoma);
    somaTermos += termo[num];
    sem_post(mutexSoma);
  
    sem_wait(&mutexQuantosPassaram);
    quantosPassaram++;
    sem_post(&mutexQuantosPassaram);
 
    if(quantosPassaram == numThreads){
      /* condição de parada: */
      if(opcao == 'f' && )
	quantosPassaram = 0;
    }

    /* pthread_barrier_wait(&barreira); */

  }
  return NULL;
}
