
int fatorial(int num)
{
	int fatorial = 1;
	while(num!=1)
	{
		fatorial *= num;
	}

	return fatorial;
}

int menosUmElevadoAn(int n)
{
	if(n%2 == 0)
	{
		return 1;
	}
	return -1;
}

int potencia(int base, int pot)
{
	int i;
	int resultado;
	for(i = 0; i<pot; i++)
	{
		resultado *=base;
	}
	return resultado;
}