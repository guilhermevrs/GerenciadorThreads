#include "../include/uthread.h"

/*GLOBALS*/

ucontext_t* contexto_hub;
ItemList* READY;
TCB* EXEC;
ItemList* BLOCKED;
TCB* MAIN;


/*PRIVATE*/
void change_exec()
{
	EXEC = removeFirstList(&READY);
	if(EXEC != NULL)
	{
		EXEC->state = st_RUNNING;
		setcontext(EXEC->context);
	}
}
/*
	Hub function, where all the threads go after they finish
*/
void hub_function()
{
	if(EXEC->waiting_for_me != NULL)
	{
		EXEC->waiting_for_me->state = st_READY;
		removeFromList(&BLOCKED, EXEC->waiting_for_me->tid);
		insertList(&READY, EXEC->waiting_for_me);
	}
	free(EXEC);
	change_exec();
}

/* Initialize the context hub, which is the next step after each thread
*/
ucontext_t* context_hub_init()
{
	contexto_hub = (ucontext_t*)malloc(sizeof(ucontext_t));
	
	if (contexto_hub == NULL)
		return NULL;

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
		newThread->state = st_READY;
		newThread->waiting_for_me = NULL;
		increment_tid();
	}
	return newThread;
}

/*PUBLIC*/

int uth_init()
{
	int retorno = OK;
	ucontext_t* main_Context = (ucontext_t*)malloc(sizeof(ucontext_t));

	set_tid(0);
	if (context_hub_init() == NULL)
		retorno = ERROR;

	if (retorno == OK)
	{
		EXEC = NULL;
		READY = createList();
		BLOCKED = createList();

		getcontext(main_Context);

		EXEC = create_tcb(main_Context);
		if(EXEC != NULL) 
			EXEC->state = st_RUNNING;
		else
			retorno = ERROR;
	}
	return retorno;
}

int uth_create(void * (*start_routine)(void*), void * arg)
{
	int varSaida = OK;
	TCB* newThread;
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

		newThread = create_tcb(thread_context);
		if(newThread == NULL)
		{
			varSaida = ERROR; //Não conseguiu alocar espaço para o TCB
		}
		else
		{
			varSaida = newThread->tid;
			insertList(&READY,newThread);
		}
	}
	if(varSaida == ERROR)
	{
		//Desaloca tudo, porque deu erro
		free(thread_context);
		free(newThread);
	}
	return varSaida;
}

int uth_yield(void)
{
	insertList(&READY,EXEC);
	EXEC->state = st_READY;
	getcontext(EXEC->context);
	if(EXEC->state == st_RUNNING)
		return OK;
	change_exec();
	return OK;
}

int uth_wait(int thr)
{
	TCB* esperado = getItemById(READY, thr);
	if (esperado == NULL)
		esperado = getItemById(BLOCKED, thr);
	if (esperado == NULL)
		return ERROR;
	if (esperado->waiting_for_me == NULL)
	{
		esperado->waiting_for_me = EXEC;
		EXEC->state = st_BLOCKED;
		insertList(&BLOCKED,EXEC);
		change_exec();
		return OK;
	}
	else
	{
		return ERROR;
	}

}
