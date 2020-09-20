/* Ejercicio 2 - TP1 de Introducción a la Concurrencia
 *
 * Se tiene un arreglo de tamaño N (argumento del programa)
 * En un hilo se llena este arreglo con valores aleatorios
 * Al finalizar la carga, el procedimiento principal muestra los valores en orden.
 *
 * Compilar con:
 *
 * make
 *
 * Ejecutar con:
 *
 * ./ej2 N
 *       |
 *       |> Tamaño del arreglo
 * Para medir tiempo completo de ejecución:
 *
 * time ./ej5
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define MAX 1024
int buffer[MAX];

void *hilo(void *arg)
{
  int tamaño = (int) arg;
  for (int i = 0; i < tamaño; i++) {
    buffer[i] = rand();
  }
  
  return 0;
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    return 0;
  }

  int tamaño = atoi(argv[1]);
  if (tamaño > MAX) {
    printf("El tamaño deseado (%d) excede el tamaño maximo (%d)", tamaño, MAX);
    return 0;
  }

  srand(time(NULL)); //setea valor de random, se ejecuta una sola vez

  pthread_t h;
  int rc = pthread_create(&h, NULL, &hilo, (void *)tamaño);
    
  if (rc)
  {
    printf("ERROR; codigo de retorno de pthread_create() es %d\n", rc);
    perror("pthread_create");
    exit(-1);
  }

  pthread_join(h, NULL);

  for (int i = 0; i < tamaño; i++) {
    printf("Valor N°%d : %d\n", i, buffer[i]);
  }

  pthread_exit(NULL);
}