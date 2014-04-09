


#include "scheduler.H"
#include "console.H"
#include "assert.H"
#include "utils.H"

/*Queue function protocols*/
int Scheduler::enqueue(struct qapi * qwrap, Thread *enqthread)
{
    if(qwrap == 0)
        assert(FALSE);
    if(((qwrap->tail + 1)%MAX_THREADS) == qwrap->head)
        return -1;
    thread_ptrs[qwrap->tail] = enqthread;
    qwrap->tail = (qwrap->tail + 1)%MAX_THREADS;
    return 0;
}

int Scheduler::dequeue(struct qapi * qwrap, Thread **deqthread)
{
    if(qwrap == 0)
        assert(FALSE);
    if(qwrap->head == qwrap->tail)
    {
        *deqthread = 0;
        return -1;
    }
    *deqthread = thread_ptrs[qwrap->head];
    qwrap->head = (qwrap->head + 1)%MAX_THREADS;
    return 0;
}



Scheduler::Scheduler()
{
    memset(&thread_ptrs,0,sizeof(Thread *)*MAX_THREADS);
    memset(&thrdq,0,sizeof(qapi));
}

void Scheduler::yield()
{
    Thread *next_thread;
    if(dequeue(&thrdq,&next_thread)==-1)
    {
        Console::puts("The queue is empty, no thread left");
        assert(FALSE);
    }
    Thread::dispatch_to(next_thread);
}


void Scheduler::resume(Thread * _thread)
{
    if(enqueue(&thrdq, _thread) == -1)
    {
        Console::puts("The array of threads is full\n");
        assert(FALSE);
    }
}

void Scheduler::add(Thread * _thread)
{
    if(enqueue(&thrdq, _thread) == -1)
    {
        Console::puts("The array of threads is full\n");
        assert(FALSE);
    }
}
    
void Scheduler::terminate(Thread * _thread)
{
    //assert(FALSE);
    delete(_thread);
    yield();
}

