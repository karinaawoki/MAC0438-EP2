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

mpz_class potencia(mpz_class base, int pot){
  int i;
  mpz_class resultado = 1;
  for(i = 0; i<pot; i++) resultado = resultado * base;
  
  return resultado;
}
