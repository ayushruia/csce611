#include "scheduler.H"
#include "console.H"
#include "assert.H"

/*Queue function protocols*/



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
        puts("The queue is empty, no thread left");
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
    
void Scheduler::terminate(Thread * _thread);
{
    assert(FALSE);
}

