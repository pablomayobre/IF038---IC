/* Ejercicio 4 - TP1 de Introducción a la Concurrencia
 *
 * Ejecuta un hilo productor que inserta caracteres random en un buffer en cada iteración.
 * Ejecuta un hilo consumidor que muestra y remueve un caracter del buffer en cada iteración.
 * El productor no inserta mas datos en el buffer cuando este se llena, y espera a que el consumidor los consuma.
 * El consumidor no borra mas datos del buffer cuando este está vacio, y espera a que el consumidor inserte nuevos caracteres.
 *
 * El tamaño del buffer se puede cambiar, modificando la variable MAX antes de Compilar
 *
 * Compilar con:
 *
 * make
 *
 * Ejecutar con:
 *
 * ./ej4
 *    Cuidado los hilos son bucles infinitos y se debe terminar el comando forzosamente.
 *
 * Para medir tiempo completo de ejecución:
 *
 * time ./ej4
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>

#define MAX 1024
char buffer[MAX];
int disponibles = 0;

const char ALPHANUM[62] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

void *productor_hilo(void *arg)
{
  while(1) {
    if (disponibles == MAX) {
      sched_yield();
    } else {
      buffer[disponibles++] = ALPHANUM[rand() % (sizeof(ALPHANUM)-1)];
    }
  }

  return 0;
}

void *consumidor_hilo(void *arg)
{
  while(1) {
    if (disponibles == 0) {
      sched_yield();
    } else {
      printf("Caracter: %c\n", buffer[--disponibles]);
    }
  }
}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  pthread_t productor;
  int rc = pthread_create(&productor, NULL, &productor_hilo, NULL);

  if (rc)
  {
    printf("ERROR; codigo de retorno de pthread_create() es %d\n", rc);
    perror("pthread_create");
    exit(-1);
  }

  pthread_t consumidor;
  rc = pthread_create(&consumidor, NULL, &consumidor_hilo, NULL);

  if (rc)
  {
    printf("ERROR; codigo de retorno de pthread_create() es %d\n", rc);
    perror("pthread_create");
    exit(-1);
  }

  pthread_exit(NULL);
}