#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/threads.h"

void
tarea1(void)
{
  for (int i = 0; i < 5; i++) {
    printf("hebra 1, iteracion %d\n", i);
    thread_yield();
  }
  thread_exit();
}

void
tarea2(void)
{
  for (int i = 0; i < 5; i++) {
    printf("hebra 2, iteracion %d\n", i);
    thread_yield();
  }
  thread_exit();
}

void
tarea3(void)
{
  for (int i = 0; i < 5; i++) {
    printf("hebra 3, iteracion %d\n", i);
    thread_yield();
  }
  thread_exit();
}

int
main(void)
{
  thread_init();                              // 1. inicializar

  thread_create(tarea1);                      // 2. crear 3 hebras
  thread_create(tarea2);
  thread_create(tarea3);

  current_thread->state = LISTA_PARA_CORRER;  // main entra a la rotacion
  thread_schedule();                          // 4. encender el planificador

  // main vuelve aca cada vez que le toca. Mientras queden hebras vivas, cede.
  for (;;) {
    int vivas = 0;
    for (int i = 1; i < MAX_THREADS; i++)
      if (threads[i].state != LIBRE)
        vivas++;
    if (vivas == 0)
      break;
    thread_yield();
  }

  printf("fin\n");
  exit(0);
}