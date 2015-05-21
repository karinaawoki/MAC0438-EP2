#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <gmpxx.h>
#include <vector>
#include "mathFunctions.h"

#define SHARED 1

using namespace std;

void cosseno(int numThreads);
void *calculaTermo(void*);
int modulo(int i);
int diferencaMenorQueM(int thread1);

vector<pthread_t> threads;
pthread_barrier_t barreira; 

sem_t mutexSoma;

unsigned int numCores = 0;

int *termo;

int numThreads;
int somaTermos;
int valorUltimaThread;
int valorPenultimaThread;
int x; /*GMP*/
int parada; /*GMP*/
char opcao;

int parar;

int main (int argc, char *argv[])
{
  numCores = std::thread::hardware_concurrency();
  printf("numCores = %d\n",numCores);

  mpf_set_default_prec(1000000);
  /* Define precisão */

  mpf_t resp,base;
  mpf_init(resp);
  mpf_init_set_si(base,40320);
  mpf_pow_ui(resp,base,40320);
  cout << resp << endl;
  x = 0.0254;  
  numThreads = 10;
  opcao = 'f';
  return 0;
}

void cosseno(int numThreads)
{
  int i;
  vector<int> thread_args;

  threads.resize(numThreads);
  thread_args.resize(numThreads);

  pthread_barrier_init(&barreira,NULL,numThreads);
  
  sem_init(&mutexSoma , SHARED, 1);

  for(i=0; i<numThreads; i++) thread_args[i] = i;

  for(i = 0; i < numThreads; i++)
    if(pthread_create(&threads[i], NULL, calculaTermo,(void*)&thread_args[i]))
      abort();
  
  for (i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);
}


void *calculaTermo(void *i)
{
  int num = *((int *) i);
  int rodada = 0, n;

  while(!parar)
  {
    n = rodada*numThreads + num;


    termo[num] = 1.0*menosUmElevadoAn(n)*potencia(x, 2*n) /fatorial(2*n);

    if(opcao == 'f' && diferencaMenorQueM(num))
    {
      parar = 1;
    }
    if(opcao == 'm' && modulo(termo[num])< parada)
    {
      parar = 1;
    }

    pthread_barrier_wait(&barreira);

    if(num == numThreads-2) valorPenultimaThread = termo[num];
    else if (num == numThreads-1) valorUltimaThread = termo[num];

    sem_wait(&mutexSoma);
      somaTermos += termo[num];
    sem_post(&mutexSoma);
  }
  return NULL;
}



int modulo(int i)
{
  if (i>=0)
    return i;
  return -1*i;
}


int diferencaMenorQueM(int thread1)
{
  int thread2 = thread1+1;
  int thread3 = thread1-1;
  /* Sabe-se que termos consecutivos tem sinais diferentes */
  if (thread2<numThreads && 
        ((termo[thread1]>0 && termo[thread2]<0) || 
         (termo[thread1]<0 && termo[thread2]>0))
     )
  {
    if(modulo(termo[thread1] - termo[thread2])<parada)
      return 1;
  }

  if (thread3>=0 && 
        ((termo[thread1]>0 && termo[thread3]<0) || 
         (termo[thread1]<0 && termo[thread3]>0) )
     )
  {
    if(modulo(termo[thread1] - termo[thread3])<parada)
      return 1;
  }
  return 0;
}