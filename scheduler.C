


#include "scheduler.H"
#include "console.H"
#include "assert.H"

//extern void * operator new(unsigned int size);
//extern void * operator new[](unsigned int size);
//extern void operator delete(void * p);

/*Queue function protocols*/
//int Scheduler::enqueue(struct qapi * qwrap,char *enqthread)
//{
//    if(qwrap == 0)
//        assert(FALSE);
//    if(((qwrap->tail + 1)%(qwrap->no_blocks)) == qwrap->head)
//        return -1;
//    char * start_loc;
//    start_loc = qwrap->start_add + (qwrap->block_size)*(qwrap->tail);
//    for(int i=0; i<qwrap->block_size; i++)
//        *(start_loc + i) = *(enqthread + i);
//    qwrap->tail = (qwrap->tail + 1)%(qwrap->no_blocks);
//    return 0;
//}
//
//int Scheduler::dequeue(struct qapi * qwrap, char *deqthread)
//{
//    if(qwrap == 0)
//        assert(FALSE);
//    if(qwrap->head == qwrap->tail)
//    {
//        memset(deqthread,0,(qwrap->block_size));
//        return -1;
//    }
//    char * start_loc;
//    start_loc = qwrap->start_add + (qwrap->block_size)*(qwrap->head);
//    for(int i=0; i<qwrap->block_size; i++)
//        *(deqthread + i) = *(start_loc + i);
//    qwrap->head = (qwrap->head + 1)%(qwrap->no_blocks);
//    return 0;
//}
//
//int Scheduler::pushqueue(struct qapi * qwrap,char *enqthread)
//{
//    if(qwrap == 0)
//        assert(FALSE);
//    if(((qwrap->tail + 1)%(qwrap->no_blocks)) == qwrap->head)
//        return -1;
//    char * start_loc;
//    start_loc = qwrap->start_add + (qwrap->block_size)*((qwrap->head + qwrap->no_blocks -1)%(qwrap->no_blocks));
//    for(int i=0; i<qwrap->block_size; i++)
//        *(start_loc + i) = *(enqthread + i);
//    qwrap->head = (qwrap->head + qwrap->no_blocks - 1)%(qwrap->no_blocks);
//    return 0;
//}
//

extern void delay(unsigned int);

Scheduler::Scheduler()
{
    ready_q = new Queue(sizeof(Thread *), MAX_THREADS);
    block_q = new Queue(sizeof(Thread *), MAX_THREADS);
    flag = TRUE;
}

void Scheduler::yield()
{
    Thread *next_thread;
    if(flag == TRUE)
    {
        flag = FALSE;
        if(ready_q->dequeue((char *)(&next_thread))==-1)
        {
            Console::puts("The ready queue is empty, no thread left\n");
            assert(FALSE);
        }
    }
    else
    {
        flag = TRUE;
        if(block_q->dequeue((char *)(&next_thread))==-1)
        {
            Console::puts("The block queue is empty, no thread left\n");
            if(ready_q->dequeue((char *)(&next_thread))==-1)
            {
                Console::puts("The ready queue is empty, no thread left\n");
                assert(FALSE);
            }
        }
    }

    Thread::dispatch_to(next_thread);
}

void Scheduler::resume(Thread * _thread)
{
    if(ready_q->enqueue((char *)(&_thread)) == -1)
    {
        Console::puts("The array of threads is full\n");
        assert(FALSE);
    }
}

void Scheduler::wait(Thread * _thread)
{
    Console::puts("The in function wait\n");
    delay(4);
    if(block_q->enqueue((char *)(&_thread)) == -1)
    {
        Console::puts("The array of blocked threads is full\n");
        assert(FALSE);
    }
}

void Scheduler::reinsert(Thread * _thread)
{
    Console::puts("The in function reinsert \n");
    delay(4);
    if(block_q->pushqueue((char *)(&_thread)) == -1)
    {
        Console::puts("The array of blocked threads is full\n");
        assert(FALSE);
    }
}


void Scheduler::add(Thread * _thread)
{
    if(ready_q->enqueue((char *)(&_thread)) == -1)
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

