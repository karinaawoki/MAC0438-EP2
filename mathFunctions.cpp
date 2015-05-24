#include"mathFunctions.hpp"

z fatorial(z num){
  z fat = 1;
  while(num!=1){
    fat = fat * num--;
  }
  return fat;
}

int menosUmElevadoAn(int n){
  if(n%2 == 0) return 1;
  return -1;
}


float potenciaErro(int k)
{
  /* A diferença tem que ser 10 elevado a - k */
  float resultado, valor = 1;
  int i;

  for(i = 0; i<k; i++)
  {
    valor = valor*10;
  }
  resultado = 1.0/valor;
  return resultado;
}

f modulo(f i){
  if(i>=0) return i;
  return -1*i;
}
