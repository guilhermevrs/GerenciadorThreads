/*
* uthread.h: arquivo de inclusão com os prototypes das funções a serem
* implementadas na realização do trabalho.
*
* NÃO MODIFIQUE ESTE ARQUIVO.
*/
#ifndef __uthread__
#define __uthread__ 1

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <ucontext.h>
#include "typedefs.h"
#include "listas.h"
#include "tid.h"

int uth_init();
int uth_create(void * (*start_routine)(void*), void * arg);
int uth_yield(void);
int uth_wait(int thr);

#endif
