#include "uthread.h"

/*GLOBALS*/

ucontext_t* contexto_hub;

/*TEMP -- just for testin*/
TCB* temp_tcb;
void foo()
{
	printf("\nomg essa é a func da thread\n");
}


/*PRIVATE*/
/*
	Hub function, where all the threads go after they finish
*/
void hub_function()
{
	printf("\nHub function\n");
	free(temp_tcb);
}

/* Initialize the context hub, which is the next step after each thread
*/
ucontext_t* context_hub_init()
{
	contexto_hub = (ucontext_t*)malloc(sizeof(ucontext_t));

	getcontext(contexto_hub);	// Criando o contexto de saída das threads
		contexto_hub->uc_stack.ss_sp = mmap(NULL,STACKSIZE,PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_GROWSDOWN|MAP_ANONYMOUS,-1,0);
		contexto_hub->uc_stack.ss_size = STACKSIZE;   
		contexto_hub->uc_link = NULL;
		makecontext(contexto_hub, (void (*)(void))hub_function, 0);
	return contexto_hub;
}

/* Creates a new tcb object, incrementing the tid
		ucontext_t* context = the context that will be executed in the thread
*/
TCB* create_tcb(ucontext_t* context)
{
	TCB *newThread = (TCB*)malloc(sizeof(TCB));
	if(newThread != NULL)
	{
		newThread->tid = get_tid();
		newThread->context = context;
		newThread->waiting_for_me = NULL;
		increment_tid();
	}
	return newThread;
}

/*PUBLIC*/

int uth_init()
{
	set_tid(0);
	ucontext_t* p1 = context_hub_init();

/*Just for testing*/
	ucontext_t* p2 = contexto_hub;
	printf("p1 == p2? %d\n", p1 == p2);
	printf("%d should be 0 \n", get_tid());
	set_tid(3);
	printf("%d should be 3 \n",get_tid());
	printf("%d should be 4 \n",increment_tid());
	printf("%d should be 3 \n",decrement_tid());
	set_tid(0);
	uth_create(foo, NULL);
	printf("%d is the next tid and should be 1", get_tid());
	setcontext(temp_tcb->context);
/*Just for testing end*/

	return OK;
}

int uth_create(void * (*start_routine)(void*), void * arg)
{
	int varSaida = OK;
	//Aloca contexto
	ucontext_t* thread_context = (ucontext_t*)malloc(sizeof(ucontext_t));
	

	if(thread_context == NULL)
	{
		varSaida = ERROR; //Não conseguiu alocar contexto para a thread;
	}
	else
	{
		//Cria o contexto da thread;
		getcontext(thread_context);
		thread_context->uc_stack.ss_sp = mmap(NULL,STACKSIZE,PROT_WRITE|PROT_READ,MAP_PRIVATE|MAP_GROWSDOWN|MAP_ANONYMOUS,-1,0);
		thread_context->uc_stack.ss_size =STACKSIZE;   
		thread_context->uc_link = contexto_hub;
		makecontext(thread_context, (void (*)(void))start_routine, 1, arg);

		temp_tcb = create_tcb(thread_context);
		if(temp_tcb == NULL)
		{
			varSaida = ERROR; //Não conseguiu alocar espaço para o TCB
		}
		else
		{
			varSaida = temp_tcb->tid;
		}
	}
	if(varSaida == ERROR)
	{
		//Desaloca tudo, porque deu erro
		free(thread_context);
		free(temp_tcb);
	}
	return varSaida;
}

int uth_yield(void)
{
	return OK;
}

int uth_wait(int thr)
{
	return OK;
}
