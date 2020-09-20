/* Ejercicio 5 - TP1 de Introducción a la Concurrencia
 *
 * Este programa es una modificación del Multiplicador de Matrices V2 dado por la catedra
 * Divide el trabajo en H hilos (parametro ingresado a traves de los argumentos)
 * donde H es divisor de N (el tamano de la matriz) y divide las operaciones en partes iguales
 *
 * Compilar con:
 *
 * make
 *
 * Ejecutar con:
 *
 * ./ej5 H
 *       |
 *       |> Donde H es la cantidad de hilos a utilizar
 *
 * Para medir tiempo completo de ejecución:
 *
 * time ./ej5
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

// Hasta N = 381 funciona bien en esta VERSION CONCURRENTE del programa en mi notebook 32bits,
// usando el tamaño por defecto para la pila de los hilos (8192, ver ulimits -s). Este
// tamaño se puede reducir utilizando pthread_attr_setstacksize()
//
// En una PC 64 bits, cuad core, con 6GB de RAM, el limite es mucho mas grande. Por ejemplo, con 1024 funcionó bien, y tardó unos 6,7 segundos en resolver correctamente la multiplicación.
#define N 1024
//#define N 8
//#define N 381

double a[N][N];
double b[N][N];
double c[N][N];

pthread_t hilos[N];

int H = 0;

void iniMat(double mat[][N], int len, double iniVal)
{
	int i, j;

	for(i = 0; i < len; ++i)
		for(j = 0; j < len; ++j)
			mat[i][j] = iniVal;
}

/* Check1Mat: Chequea que la matriz resultante quede con
 *            el mismo valor en todas sus celdas.
 * Es un chequeo básico para el caso particular en que
 * se sabe de ante mano que el resultado tendrá esa forma.
 * Retorna: 0 en caso correcto (todas las celdas contiene
 *          finVal. 1 En caso de error (al menos una celda
 * contiene un valor diferente de finVal).
 */
int Check1Mat(double mat[][N], int len, double finVal)
{
	int i, j;

	for(i = 0; i < len; ++i)
		for(j = 0; j < len; ++j)
			if(mat[i][j] != finVal)
				return 1;
	return 0;
}

void printMat(double mat[][N], int len)
{
	int i, j;

	for(i = 0; i < len; ++i)
	{
		for(j = 0; j < len; ++j)
			printf( "%.1f ", mat[i][j]);
		printf("\n");
	}
}

void *worker(void *arg)
{
	int h, i, j, k;

	h = (int) arg;

  for (i = h * (N/H); i < (h + 1) * (N/H); ++i)
    for(j = 0; j < N; ++j)
      for(k = 0; k < N; ++k)
        c[i][j] += a[i][k] * b[k][j];
  
  return 0;
}

void mm()
{
	int i, retval;

	for(i = 0; i < H; ++i)
	{
    printf("Creando hilo: %d\n", i);
		retval = pthread_create(&hilos[i], NULL, &worker, (void *) i);
		if(retval != 0)
		{
      printf("ERROR; codigo de retorno de pthread_create() es %d\n", retval);
			perror("pthread_create");
			exit(1);
		}
	}

	for(i = 0; i < H; ++i)
	{
    printf("Uniendo hilo: %d\n", i);
		pthread_join(hilos[i], NULL);
	}
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    return 0;
  }

  H = atoi(argv[1]);
  if (N % H != 0) {
    printf("N (%d) no es divisible por H (%d)\n", N, H);
    return 0;
  }

	printf("Comienzo ...\n");
	iniMat(a, N, 1.0);
	iniMat(b, N, 1.0);
	iniMat(c, N, 0.0);

	printf("Multiplicando ...\n");

	mm();

	if(Check1Mat(c, N, N) == 0)
		printf("Fin Multiplicación (Resultado correcto)\n");
	else
		printf("Fin Multiplicación (Resultado INCORRECTO!)\n");

/*
	printf("Matriz a:\n");
	printMat(a, N);
	printf("Matriz b:\n");
	printMat(b, N);
	printf("Matriz c:\n");
	printMat(c, N);
*/

	printf("Fin del programa\n");
}