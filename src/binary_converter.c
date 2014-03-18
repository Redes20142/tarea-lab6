#include "binary_converter.h"

/*
 *
 * Convierte un número entero a binario.
 *
 * Autor: Manuel Ignacio Castillo López; 3-0801743-8.
 * manuel_castillo_cc@ciencias.unam.mx
 * Autor: Mijail Gutiérrez Valdéz; 3-423250-3.
 * mij000@ciencias.unam.mx
 * Version 1, marzo 2014
 */

/*
 * Convierte un entero de 16, 32 ó 64 bits; dependiendo el primer argumento,
 * en una cadena de caracteres que contiene la representación binaria del
 * número
 */
void itobin(unsigned short int bitnum, long long number, char *res,
	unsigned int ressize)
{
	if(bitnum != 16 && bitnum != 32 && bitnum != 64)
	{
		printf("itobin: Error. No se reconoce el n\u00FAmero de bits ");
		printf("del entero a convertir a binario\n");
		return;
	}//comprueba que el número de bits dado esté en el rango esperado
	unsigned short int sign;
	memset(res, 0, ressize);
	if(number >= 0)
	{
		sign = 0;
	}
	else
	{
		sign = 1;
		number *= -1;
	}//recuerda el signo original del número y usa su valor absoluto
	unsigned short int i =  bitnum -1;
	while(number > 1)
	{
		if(number %2)
		{
			strcat(res, "1");
		}
		else
		{
			strcat(res, "0");
		}//coloca el dígito binario correspondiente
		number /= 2;
		i--;
	}//reduce el número a su expresión binaria
	if(number %2)
	{
		strcat(res, "1");
	}
	else
	{
		strcat(res, "0");
	}//asigna el último resiuo
	while(i > 0)
	{
		strcat(res, "0");
		i--;
	}//rellena con 0
	strrev(res);
	if(sign)
	{
		sign = 0;
		char *bits = malloc(sizeof(char*) *bitnum);
		for(i = bitnum -1; i >= 0; i--)
		{
			if(res[i] == '0')
			{
				if(sign)
				{
					strcat(bits, "1");
				}
				else
				{
					strcat(bits, "0");
				}//si ya se encontró el primer 1, invierte el 0
			}
			else
			{
				if(sign)
				{
					strcat(bits, "0");
				}
				else
				{
					sign = 1;
					strcat(bits, "1");
				}//si ya encontró el primer 1, invierte el 1. Si encuentra
				//el primer 1, lo deja tal cual e indica que encontró el
				//prier 1
			}//actua dependiendo si encuentra 1 ó 0
		}//recorre la cadena sacando el complemento a 2
		strrev(bits);
		strcpy(res, bits);
	}//si el signo era negativo, obtiene su complemento a 2
}//dectobin

/*
 * Convierte una cadena de caracteres binarios en un entero de 16 bits
 */
short int bintos(char *binstr)
{
	if(strlen(binstr) < 16)
	{
		printf("La cadena debe tener al menos 16 caracteres de longitud\n");
		return -1;
	}//si la cadena no tiene una longitud válida
	short int result = 0;
	short int i;
	if(binstr[15] == '1')
	{
		result = 1;
	}
	for(i = 14; i >= 0; i--)
	{
		if(binstr[i] == '1')
		{
			result += 2 << (14 -i);
		} else if(binstr[i] != '0') {
			printf("La cadena debe contener \u00FAnicamente caracteres binarios\n");
			return -1;
		}//si vale la pena hacer el cálculo y verifica que sean binarios
	}//recorre la cadena sumando potencias de 2
	return result;
}//bintos

/*
 * Convierte una cadena de caracteres binarios en un entero de 32 bits
 */
int bintoi(char *binstr)
{
	if(strlen(binstr) < 32)
	{
		printf("La cadena debe tener al menos 32 caracteres de longitud\n");
		return -1;
	}//si la cadena no tiene una longitud válida
	int result = 0;
	short int i;
	if(binstr[31] == '1')
	{
		result = 1;
	}
	for(i = 30; i >= 0; i--)
	{
		if(binstr[i] == '1')
		{
			result += 2 << (30 -i);
		} else if(binstr[i] != '0') {
			printf("La cadena debe contener \u00FAnicamente caracteres binarios\n");
			return -1;
		}//si vale la pena hacer el cálculo y verifica que sean binarios
	}//recorre la cadena sumando potencias de 2
	return result;
}//bintoi

/*
 * Convierte una cadena de caracteres binarios en un entero de 64 bits
 */
long long int bintol(char *binstr)
{
	if(strlen(binstr) < 64)
	{
		printf("La cadena debe tener al menos 64 caracteres de longitud\n");
		return -1;
	}//si la cadena no tiene una longitud válida
	int result = 0;
	short int i;
	if(binstr[63] == '1')
	{
		result = 1;
	}
	for(i = 62; i >= 0; i--)
	{
		if(binstr[i] == '1')
		{
			result += 2 << (62 -i);
		} else if(binstr[i] != '0') {
			printf("La cadena debe contener \u00FAnicamente caracteres binarios\n");
			return -1;
		}//si vale la pena hacer el cálculo y verifica que sean binarios
	}//recorre la cadena sumando potencias de 2
	return result;
}//bintol

/*
 * Invierte una cadena
 */
void strrev(char *string)
{
	int length, c;
	char *begin, *end, temp;
	length = strlen(string); 
	begin = string;
	end = string; 
	for ( c = 0 ; c < ( length - 1 ) ; c++ ) end++;
	for ( c = 0 ; c < length/2 ; c++ ) 
	{        
		temp = *end;
		*end = *begin;
		*begin = temp; 
		begin++;
    	end--;
   }//invierte la cadena
}//strrev

