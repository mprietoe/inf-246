#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/threads.h"

struct thread threads[MAX_THREADS];
struct thread *current_thread;

// Prepara la biblioteca: todo LIBRE, y main pasa a ser threads[0]
void
thread_init(void)
{
  for (int i = 0; i < MAX_THREADS; i++)
    threads[i].state = LIBRE;
  threads[0].state = CORRIENDO;
  current_thread = &threads[0];
}

// Requerimiento B
int
thread_create(void (*func)())
{
  struct thread *t = 0;

  for (int i = 0; i < MAX_THREADS; i++) {
    if (threads[i].state == LIBRE) {
      t = &threads[i];
      break;
    }
  }
  if (t == 0)
    return -1;

  memset(&t->context, 0, sizeof(t->context));
  t->context.ra = (uint64)func;                    // el "ret" saltara aqui
  t->context.sp = (uint64)t->stack + STACK_SIZE;   // la pila crece hacia abajo
  t->state = LISTA_PARA_CORRER;
  return 0;
}

// Requerimiento C: round-robin circular, la hebra actual es la ultima candidata
void
thread_schedule(void)
{
  struct thread *t;
  struct thread *siguiente = 0;
  struct thread *anterior;

  t = current_thread + 1;
  for (int i = 0; i < MAX_THREADS; i++) {
    if (t >= threads + MAX_THREADS)
      t = threads;
    if (t->state == LISTA_PARA_CORRER) {
      siguiente = t;
      break;
    }
    t++;
  }

  if (siguiente == 0) {
    printf("thread_schedule: no quedan hebras listas para correr\n");
    exit(0);
  }

  // La unica lista es la actual: simplemente continua
  if (siguiente == current_thread) {
    current_thread->state = CORRIENDO;
    return;
  }

  anterior = current_thread;
  siguiente->state = CORRIENDO;
  current_thread = siguiente;
  thread_switch(&anterior->context, &siguiente->context);
}

// Requerimiento D
int
thread_yield(void)
{
  struct thread *t = current_thread;
  if (t == 0)
    return -1;
  t->state = LISTA_PARA_CORRER;
  thread_schedule();
  return 0;
}

// La hebra muere: su ficha queda LIBRE y no vuelve nunca
void
thread_exit(void)
{
  current_thread->state = LIBRE;
  thread_schedule();
}