
#ifndef QUEUE_H
#define QUEUE_H

#include "utils.H"

extern void * operator new(unsigned int size);
extern void * operator new[](unsigned int size);
extern void operator delete(void * p);


class Queue
{
    private:
    char * start_add;
    unsigned int head;
    unsigned int tail;
    unsigned int block_size;
    unsigned int max_size;
    unsigned int no_blocks;
    
    public:

    Queue(unsigned int, unsigned int);
    int enqueue(char * enqthread);
    int dequeue(char * deqthread);
    int pushqueue(char * enqthread);
    int no_elements();
    BOOLEAN is_full();
};

#endif
