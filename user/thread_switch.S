 # ---------------------------------------------------------------------------
# Requerimiento C: cambio fisico de contexto (RISC-V)
#
#   void thread_switch(struct thread_context *viejo,
#                      struct thread_context *nuevo);
#
#   a0 = puntero al contexto de la hebra que se pausa  -> se GUARDA
#   a1 = puntero al contexto de la hebra que despierta -> se CARGA
#
# Solo se guardan ra, sp y s0..s11 (los registros "callee-saved" del ABI de
# RISC-V). Los demas (a0..a7, t0..t6) son "caller-saved": el compilador ya
# los dejo a salvo en la pila antes de llamar a esta funcion, por eso no
# hace falta tocarlos.
#
# Los desplazamientos corresponden a struct thread_context, que son 14
# campos uint64 (8 bytes cada uno) en este orden:
#   ra=0  sp=8  s0=16  s1=24  s2=32  s3=40  s4=48  s5=56
#   s6=64 s7=72 s8=80  s9=88  s10=96 s11=104
# Si cambia el orden de los campos de la struct, hay que cambiar estos
# numeros.
# ---------------------------------------------------------------------------

    .text
    .globl thread_switch
    .align 4
thread_switch:
    # --- guardar la "fotografia" de la hebra actual en *a0 ---
    sd ra,   0(a0)
    sd sp,   8(a0)
    sd s0,  16(a0)
    sd s1,  24(a0)
    sd s2,  32(a0)
    sd s3,  40(a0)
    sd s4,  48(a0)
    sd s5,  56(a0)
    sd s6,  64(a0)
    sd s7,  72(a0)
    sd s8,  80(a0)
    sd s9,  88(a0)
    sd s10, 96(a0)
    sd s11,104(a0)

    # --- cargar la "fotografia" de la hebra que despierta desde *a1 ---
    ld ra,   0(a1)
    ld sp,   8(a1)
    ld s0,  16(a1)
    ld s1,  24(a1)
    ld s2,  32(a1)
    ld s3,  40(a1)
    ld s4,  48(a1)
    ld s5,  56(a1)
    ld s6,  64(a1)
    ld s7,  72(a1)
    ld s8,  80(a1)
    ld s9,  88(a1)
    ld s10, 96(a1)
    ld s11,104(a1)

    # `ret` salta a ra, que ahora es el ra de la hebra nueva:
    # ahi es donde esa hebra quedo pausada (o, si es su primera vez,
    # la funcion que le asigno thread_create).
    ret