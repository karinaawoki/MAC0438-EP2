#define SHARED 1
#define DEBUG 0
#define _XOPEN_SOURCE 600

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <gmpxx.h>
#include <vector>
#include "mathFunctions.hpp"

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

int numThreads;
f parada,x,somaTermos,valorUltimaThread,valorPenultimaThread,ultimo;
int quantosPassaram;
vector<f> termo;
char opcao;
char impressao;

int main (int argc, char *argv[]){
  numCores = thread::hardware_concurrency();

  mpf_set_default_prec(1000000);
  /* Define precisão */
  
  x = 1.3;
  numThreads = 3;
  opcao = 'f';
  parada = 0.13;
  /*--------*/
  
  if(DEBUG) printf("numCores = %d\n",numCores);
  
  numThreads = atoi(argv[1]);
  if(numThreads == 0) numThreads = numCores;

  opcao = argv[2][0];
  parada = potenciaErro(atoi(argv[3]));
  x = atof(argv[4]);

  if(argc >= 6)
    impressao = argv[5][0];
  else
    impressao = 'n';

  if (DEBUG){
    printf("numThreads %d\n", numThreads);
    printf("opcao %c\n", opcao);
    printf("parada %f\n", parada);
    printf("x %f\n", x);
    printf("%c\n", impressao);
  }

  cosseno();
  return 0;
}



void cosseno(){
  int i;
  vector<int> thread_args;
  
  threads.clear();
  termo.clear();
  thread_args.clear();
  parar = 0;
  ultimo = 10;
  threads.resize(numThreads);
  termo.resize(numThreads);
  thread_args.resize(numThreads);

  quantosPassaram = 0;
  
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

  printf("cos(%f) = %f\n", x, somaTermos);
}



void *calculaTermo(void *i)
{
  int num = *((int *) i);
  int rodada = 0;
  int n = 0;
  //mpf_class termo = 0;
  f resp = 0;

  while(!parar)
  {
    n = rodada*numThreads + num;
    
    mpf_pow_ui(resp.get_mpf_t(),x.get_mpf_t(),2*n);
    termo[num] = (menosUmElevadoAn(n)*resp)/fatorial(2*n);
    sem_wait(&parar);
    sem_post(&parar);

    termo[num] = menosUmElevadoAnINT(n)*potenciaINT(x, 2*n)*1.0 /fatorialINT(2*n);

    /* BARREIRA AUXILIAR */
    pthread_barrier_wait(&barreira2);

    if(opcao == 'f' && num == 0 && modulo(ultimo - termo[num]) < parada)
      parar = 1;
    else if (opcao == 'f' && num!=0 && modulo(termo[num-1] -termo[num])<parada)
      parar = 1;

    if(opcao == 'm' && modulo(termo[num])< parada)
      parar = 1;


    if(num == numThreads-1)
      ultimo = termo[num];

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
    /*>>>*/somaTermos += termo[num];
    /*>>>*/if(impressao == 's') printf("Valor parcial de cos(x) - a cada termo: %f\n", somaTermos);
    sem_post(&mutexSoma);

    

    if(impressao == 'd')
      printf("Thread %d chegou na barreira! \n", num);
    /* BARREIRA */
    pthread_barrier_wait(&barreira);
    
    if(impressao=='d' && num == 0)
      printf("Valor parcial do cos(x): %f\n\n", somaTermos);

    rodada++;

  }
  if(impressao!='s' && num == 0)
    printf("\nNúmero de rodadas: %d\n", rodada);
  else if(impressao == 's' && num==0)
    printf("\nNúmero de termos calculados: %d\n", rodada*numThreads);

  return NULL;
}



float modulo(float i)
{
  if (i>=0)
    return i;
  return -1*i;
}
