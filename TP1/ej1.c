/* Ejercicio 1 - TP1 de Introduccion a la Concurrencia
 *
 * Este programa demuestra el problema de las condiciones de carrera
 * Este programa cuenta con un contador, dos hilos que incrementan el contador en uno
 * y un hilo que decrementa en dos el contador.
 *
 * Compilar con:
 *
 * make
 *
 * Ejecutar con:
 *
 * ./ej1
 *
 * Para medir tiempo completo de ejecución:
 *
 * time ./ej1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define ITERACIONES 100
#define HILOS 3
int contador = 0;
pthread_t hilos[HILOS];


void *incrementador(void *arg)
{
  int hilo = (int) arg;
  for (int i=0; i < ITERACIONES; i++) {
    // Esto puede causar condiciones de carrera
    // Modifica el estado global contador.
    contador++;

	  printf("Hilo %d incremento el contador a %d\n", hilo, contador);
  }
  
  return 0;
}

void *decrementador(void *arg)
{
  int hilo = (int) arg;
  for (int i=0; i < ITERACIONES; i++) {
    // Esto puede causar condiciones de carrera
    // Modifica el estado global contador.
    contador -= 2;

	  printf("Hilo %d incremento el contador a %d\n", hilo, contador);
  }
  
  return 0;
}

int main()
{
  int retval = pthread_create(&hilos[0], NULL, &decrementador, (void *) 0);
  if(retval != 0)
  {
    perror("pthread_create");
    exit(1);
  }

  for (int i=1; i < HILOS; i++) {
    retval = pthread_create(&hilos[i], NULL, &incrementador, (void *) i);
    if(retval != 0)
    {
      printf("ERROR; codigo de retorno de pthread_create() es %d\n", retval);
      perror("pthread_create");
      exit(1);
    }
  }

  pthread_exit(NULL);
}