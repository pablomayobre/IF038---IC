/* Ejercicio 2 - TP1 de Introducción a la Concurrencia
 *
 * Inicializa un arreglo con N valores random, y N hilos
 * Los hilos añaden a una variable global contador,
 * el valor que corresponde a su posicion en el arreglo.
 *
 * Compilar con:
 *
 * make
 *
 * Ejecutar con:
 *
 * ./ej3
 *
 * Para medir tiempo completo de ejecución:
 *
 * time ./ej3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define N 10
pthread_t hilos[N];
int posiciones[N];

int contador = 0;

void *hilo(void *arg)
{
  int i = (int) arg;
  
  contador += posiciones[i];
  printf("El hilo %d sumó %d al contador, valor actual: %d\n", i, posiciones[i], contador);

  return 0;
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  for (int i = 0; i < N; i++) {
    // Uso modulo 1000, para evitar posibles overflows en enteros.
    posiciones[i] = rand() % 1000;
  }

  for (int i=0; i < N; i++) {
    int rc = pthread_create(&hilos[i], NULL, &hilo, (void *)i);
      
    if (rc)
    {
      printf("ERROR; codigo de retorno de pthread_create() es %d\n", rc);
      perror("pthread_create");
      exit(-1);
    }
  }

  for (int i=0; i < N; i++) {
    pthread_join(hilos[i], NULL);
  }

  printf("Valor final: %d\n", contador);

  pthread_exit(NULL);
}