#define SHARED 1
#define DEBUG 1
#define _XOPEN_SOURCE 600

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <gmp.h>
#include <gmpxx.h>

#include "mathFunctions.hpp"

using namespace std;

void cosseno();
void *calculaTermo(void*);
void sequencial();

vector<pthread_t> threads;
pthread_barrier_t barreira,barreira2; 
sem_t mutexSoma;

unsigned int numCores = 0;

int numThreads,parar,quantosPassaram;
f parada,x,somaTermos,valorUltimaThread,valorPenultimaThread,ultimo;
char opcao,impressao;
vector<f> termo;

int main (int argc, char *argv[]){
  numCores = thread::hardware_concurrency();

  mpf_set_default_prec(100000);
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

  if(argc >= 6) impressao = argv[5][0];
  else impressao = 'n';


  if(DEBUG){
    printf("numThreads %d\n", numThreads);
    printf("opcao %c\n", opcao);
    cout << "parada " << parada << endl;
    cout << "x " << x << endl;
    printf("%c\n", impressao);
  }

  somaTermos = 0; /* AQUI */

  cosseno();
  return 0;
}



void cosseno(){
  vector<int> thread_args;
  
  threads.clear();
  termo.clear();
  thread_args.clear();
  parar = 0;
  ultimo = 10;
  threads.resize(numThreads);
  termo.resize(numThreads);
  thread_args.resize(numThreads);
  
  pthread_barrier_init(&barreira,NULL,numThreads);
  pthread_barrier_init(&barreira2,NULL,numThreads);
  
  sem_init(&mutexSoma , SHARED, 1);

  for(int i = 0; i < numThreads; i++){
    thread_args[i] = i;
    termo[i] = 5;
  }
  if(impressao == 's')
    sequencial();  
  else
  {
    for(int i = 0; i < numThreads; i++)
      if(pthread_create(&threads[i], NULL, calculaTermo,(void*)&thread_args[i]))
        abort();
    
    for(int i = 0; i < numThreads; i++) pthread_join(threads[i], NULL);
  }

  //cout << "cos(" << x << ") = " << somaTermos << endl;
  cout << "cos(" << x <<") = ";  
  gmp_printf("%.*Ff\n",100000,somaTermos.get_mpf_t());
}


void *calculaTermo(void *i){
  int num = *((int *) i);
  int rodada = 0;
  int n = 0;
  f resp = 0.0;

  while(!parar){
    n = rodada*numThreads + num;
    
    mpf_pow_ui(resp.get_mpf_t(),x.get_mpf_t(),2*n);
    cout << resp << endl;
    termo[num] = (menosUmElevadoAn(n)*resp)/fatorial(2*n);

    /* BARREIRA AUXILIAR */
    pthread_barrier_wait(&barreira2);

    if(opcao == 'f' && num == 0 && modulo(ultimo - termo[num]) < parada) parar = 1;
    else if (opcao == 'f' && num!=0 && modulo(termo[num-1] -termo[num])<parada) parar = 1;

    if(opcao == 'm' && modulo(termo[num])< parada) parar = 1;

    if(num == numThreads-1) ultimo = termo[num];

    sem_wait(&mutexSoma);
    /*>>>*/somaTermos = somaTermos + termo[num];
    sem_post(&mutexSoma);
    

    if(impressao == 'd') printf("Thread %d chegou na barreira! \n", num);
    /* BARREIRA */
    pthread_barrier_wait(&barreira);
    
    if(impressao=='d' && num == 0) cout << "Valor parcial do cos(" << x << "): " << somaTermos << endl;

    rodada++;

  }
  return NULL;
}


void sequencial() /* AQUI */
{
  float termo1;
  float ultimoTermo = 10;
  int n = 0;
  while(1)
  {
    termo1 = menosUmElevadoAnINT(n)*potenciaINT(x, 2*n)*1.0 /fatorialINT(2*n);
    somaTermos+=termo1;
    printf("Valor parcial de cos(x): %f\n", somaTermos);
    if(opcao == 'm' && modulo(termo1) < parada)N
      break;
    else if(opcao == 'f' && modulo(ultimoTermo - termo1)<parada)
      break;

    ultimoTermo = termo1;
    n++;
  }

  printf("Número de termos: %d\n", n);
}


