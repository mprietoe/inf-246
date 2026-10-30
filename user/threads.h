#ifndef THREADS_H
#define THREADS_H

#define STACK_SIZE 4096
#define MAX_THREADS 8

enum thread_state { LIBRE, LISTA_PARA_CORRER, CORRIENDO };

// El orden importa: threads_switch.S usa estos offsets
// (ra=0, sp=8, s0=16, s1=24 ... s11=104)
struct thread_context {
  uint64 ra;
  uint64 sp;
  uint64 s0;
  uint64 s1;
  uint64 s2;
  uint64 s3;
  uint64 s4;
  uint64 s5;
  uint64 s6;
  uint64 s7;
  uint64 s8;
  uint64 s9;
  uint64 s10;
  uint64 s11;
};

struct thread {
  struct thread_context context;
  char stack[STACK_SIZE] __attribute__((aligned(16)));
  enum thread_state state;
};

extern struct thread threads[MAX_THREADS];
extern struct thread *current_thread;

// Definida en threads_switch.S
extern void thread_switch(struct thread_context *viejo,
                          struct thread_context *nuevo);

void thread_init(void);
int  thread_create(void (*func)());
void thread_schedule(void);
int  thread_yield(void);
void thread_exit(void);

#endif