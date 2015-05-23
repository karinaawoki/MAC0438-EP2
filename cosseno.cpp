#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
/*#include <gmpxx.h>*/
#include <vector>
#include "mathFunctions.hpp"
#include <stdlib.h>
#include <stdio.h>

#define SHARED 1

using namespace std;

void cosseno();
void *calculaTermo(void*);
float modulo(float i);
int diferencaMenorQueM(int thread1);

vector<pthread_t> threads;
pthread_barrier_t barreira; 
pthread_barrier_t barreira2; 

sem_t mutexSoma;

unsigned int numCores = 0;

vector<float> termo;

int numThreads;
float somaTermos;
float x; /*GMP*/
float parada; /*GMP*/
char opcao;
float ultimo;

int parar;

int main (int argc, char *argv[]){
  numCores = thread::hardware_concurrency();

  printf("numCores = %d\n",numCores);
/*
  mpf_set_default_prec(1000000);*/
  /* Define precisão */
/*
  mpf_t resp,base;
  mpf_init(resp);
  mpf_init_set_si(base,40320);
  mpf_pow_ui(resp,base,40320);
  cout << resp << endl;*/

  /*---------*/
  x = 1.3;
  numThreads = 3;
  opcao = 'f';
  parada = 0.13;
  /*--------*/

  cosseno();
  return 0;
}



void cosseno(){
  int i;
  vector<int> thread_args;
  parar = 0;
  ultimo = 10;
  threads.resize(numThreads);
  termo.resize(numThreads);
  thread_args.resize(numThreads);

  pthread_barrier_init(&barreira,NULL,numThreads);
  pthread_barrier_init(&barreira2,NULL,numThreads);
  
  sem_init(&mutexSoma , SHARED, 1);

  for(i=0; i<numThreads; i++) 
  {
    thread_args[i] = i;
    termo[i] = 5;
  }
  
  for(i = 0; i < numThreads; i++)
    if(pthread_create(&threads[i], NULL, calculaTermo,(void*)&thread_args[i]))
      abort();

  for (i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);

  printf("\n\n %f\n", somaTermos);
}



void *calculaTermo(void *i)
{
  int num = *((int *) i);
  int rodada = 0, n;

  while(!parar)
  {
    n = rodada*numThreads + num;

    termo[num] = menosUmElevadoAnINT(n)*potenciaINT(x, 2*n)*1.0 /fatorialINT(2*n);

    pthread_barrier_wait(&barreira2);
    if(opcao == 'f' && num == 0 && modulo(ultimo - termo[num]) < parada)
    {
      parar = 1;
    }
    else if (opcao == 'f' && num!=0 && modulo(termo[num-1] -termo[num])<parada)
    {
      parar = 1;
    }

    if(opcao == 'm' && modulo(termo[num])< parada)
    {
      parar = 1;
    }

    pthread_barrier_wait(&barreira);

    if(num == numThreads-1)
      ultimo = termo[num];

    sem_wait(&mutexSoma);
      somaTermos += termo[num];
    sem_post(&mutexSoma);

    rodada++;

  }
  return NULL;
}



float modulo(float i)
{
  if (i>=0)
    return i;
  return -1*i;
}
