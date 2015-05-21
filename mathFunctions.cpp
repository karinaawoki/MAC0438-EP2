#include"mathFunctions.hpp"

mpz_class fatorial( mpz_class num){
  mpz_class fatorial = 1;
  while(num!=1){
    fatorial = fatorial * num;
    num = num -1;
  }

  return fatorial;
}

int menosUmElevadoAn(int n){
  if(n%2 == 0) return 1;
  return -1;
}



int fatorialINT(int i)
{
  int resultado = 1;
  while(i>1)
  {
    resultado *= i;
    i--;
  }
  return resultado;
}

int menosUmElevadoAnINT(int i)
{
  if(i%2 == 0) return 1;
  return -1;
}


int potenciaINT(int base, int pot)
{
  int resultado = 1;
  while(pot>0)
  {
    resultado *=base;
    pot--;
  }

  return resultado;
}