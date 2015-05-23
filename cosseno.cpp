#define SHARED 1
#define _XOPEN_SOURCE 600

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

sem_t mutexSoma, mutexQuantosPassaram, parar;

unsigned int numCores = 0;

int numThreads;
f x,somaTermos,valorUltimaThread,valorPenultimaThread;
int quantosPassaram;
vector<f> termo;
char opcao;

int main (int argc, char *argv[]){
  numCores = thread::hardware_concurrency();

  printf("numCores = %d\n",numCores);

  mpf_set_default_prec(1000000);

  //mpf_init(somaTermos);
  //mpf_init(valorUltimaThread);
  //mpf_init(valorPenultimaThread);
  

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

  
  threads.clear();
  termo.clear();
  thread_args.clear();
  threads.resize(numThreads);
  termo.resize(numThreads);
  thread_args.resize(numThreads);

  //for(int i = 0; i < numThreads; i++) mpf_init(termo[i]);

  quantosPassaram = 0;
  
  pthread_barrier_init(&barreira,NULL,numThreads);
  
  sem_init(&parar , SHARED, 1);
  sem_init(&mutexSoma , SHARED, 1);
  sem_init(&mutexQuantosPassaram, SHARED, 1);

  for(i=0; i<numThreads; i++) thread_args[i] = i;

  for(i = 0; i < numThreads; i++)
    if(pthread_create(&threads[i], NULL, calculaTermo,(void*)&thread_args[i]))
      abort();
  
  for (i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);
}


void *calculaTermo(void *i){
  int num = *((int *) i);
  int rodada = 0;
  int n = 0;
  //mpf_class termo = 0;
  f resp = 0;

  while(1){
    
    n = rodada*numThreads + num;
    
    mpf_pow_ui(resp.get_mpf_t(),x.get_mpf_t(),2*n);
    termo[num] = (menosUmElevadoAn(n)*resp)/fatorial(2*n);
    sem_wait(&parar);
    sem_post(&parar);

    if(opcao == 'f' && valorUltimaThread-valorUltimaThread < parada) sem_wait(parar);
    if(opcao == 'm' && valorUltimaThread < parada) sem_wait(parar);

    pthread_barrier_wait(&barreira);

    if(num == numThreads-2) valorPenultimaThread = termo[num];
    else if (num == numThreads-1) valorUltimaThread = termo[num];

    sem_wait(&mutexSoma);
    /*>>>*/somaTermos += termo[num];
    sem_post(&mutexSoma);
  
    sem_wait(&mutexQuantosPassaram);
    quantosPassaram++;
 
    if(quantosPassaram == numThreads){
        /* condição de parada: */
        if(opcao == 'f' && valorUltimaThread-valorUltimaThread < parada) sem_wait(parar);
        if(opcao == 'm' && valorUltimaThread < parada) sem_wait(parar);
	
        quantosPassaram = 0;
      }
    sem_post(&mutexQuantosPassaram);
    /* pthread_barrier_wait(&barreira); */

  }
  return NULL;
}



int modulo(int i){
  if(i>=0) return i;
  return -1*i;
}
