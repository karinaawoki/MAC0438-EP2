#include"mathFunctions.hpp"

z fatorial(z num){
  /*
  mpf_t fatorial;
  mpf_init_set_si(fatorial,1);
  
  while(num!=1){
    mpf_set_si(fatorial,fatorial*num);
    num = num -1;
  }

  mpf_set(res,fatorial);
  */
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
