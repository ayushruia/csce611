/* 
Author: Joshua Capehart
Based on code by:
R. Bettati
Department of Computer Science
Texas A&M University

A thread scheduler.

*/
#ifndef SCHEDULER_H
#define SCHEDULER_H

/*--------------------------------------------------------------------------*/
/* DEFINES 
 *--------------------------------------------------------------------------*/

#define MAX_THREADS 100
/*--------------------------------------------------------------------------*/
/* INCLUDES 
 *--------------------------------------------------------------------------*/

#include "machine.H"
#include "thread.H"
#include "queue.H"
#include "utils.H"

//extern void * operator new(unsigned int);

//struct qapi
//{
//    char * start_add;
//    unsigned int head;
//    unsigned int tail;
//    unsigned int block_size;
//    unsigned int max_size;
//    unsigned int no_blocks;
//};
//

/*--------------------------------------------------------------------------*/
/* SCHEDULER
 *--------------------------------------------------------------------------*/

class Scheduler {

    public:
    class Queue *ready_q;
    class Queue *block_q;
    BOOLEAN flag;
    unsigned int thread_count;
    /* The scheduler may need private members... */


    Scheduler();
    /* Setup the scheduler. This sets up the ready queue, for example.
       If the scheduler implements some sort of round-robin scheme, then the the CPU in response to a preemption. */

    virtual void yield();
    /* Called by the currently running thread in order to give up the CPU. 
       The scheduler selects the next thread from the ready queue to load onto 
       the CPU, and calls the dispatcher function defined in 'threads.h' to
       do the context switch. */

    virtual void resume(Thread * _thread);
    /* Add the given thread to the ready queue of the scheduler. This is called
       for threads that were waiting for an event to happen, or that have 
       to give up the CPU in response to a preemption. */

    virtual void wait(Thread * _thread);

    virtual void reinsert(Thread * _thread);

    virtual void add(Thread * _thread);
    /* Make the given thread runnable by the scheduler. This function is called
       typically after thread creation. Depending on the
       implementation, this may not entail more than simply adding the 
       thread to the ready queue (see scheduler_resume). */

    virtual void terminate(Thread * _thread);
    /* Remove the given thread from the scheduler in preparation for destruction
       of the thread. */

//    int enqueue(struct qapi * qwrap, char * enqthread);
//    int dequeue(struct qapi * qwrap, char * deqthread);
//    int pushqueue(struct qapi * qwrap, char * enqthread);

};






#endif
