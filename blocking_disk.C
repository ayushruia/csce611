/*
File        : simple_disk.c

Author      : Riccardo Bettati
Modified    : 10/04/01

Description : Block-level READ/WRITE operations on a simple LBA28 disk 
using Programmed I/O.

The disk must be MASTER or SLAVE on the PRIMARY IDE controller.

The code is derived from the "LBA HDD Access via PIO" 
tutorial by Dragoniz3r. (google it for details.)
*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "assert.H"
#include "utils.H"
#include "console.H"
#include "blocking_disk.H"

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR */
/*--------------------------------------------------------------------------*/
unsigned int BlockingDisk::wait_queue = 0;

BlockingDisk::BlockingDisk(DISK_ID _disk_id, unsigned int _size):SimpleDisk(_disk_id, _size){
}



void BlockingDisk::read(unsigned long _block_no, unsigned char * _buf) {
    /* Reads 512 Bytes in the given block of the given disk drive and copies them 
       to the given buffer. No error check! */

    if(BlockingDisk::wait_queue > 0)
        BlockingDisk::wait_queue++;
    SYSTEM_SCHEDULER->wait(Thread::CurrentThread());
    SYSTEM_SCHEDULER->yield();

   // while(!is_ready())
   // {
   //     SYSTEM_SCHEDULER->reinsert();
   //     SYSTEM_SCHEDULER->yield();
   // }

    issue_operation(READ, _block_no);

    while(!is_ready())
    {
        SYSTEM_SCHEDULER->reinsert(Thread::CurrentThread());
        SYSTEM_SCHEDULER->yield();
    }
    BlockingDisk::wait_queue--;


    /* read data from port */
    int i;
    unsigned short tmpw;
    for (i = 0; i < 256; i++) {
        tmpw = inportw(0x1F0);
        _buf[i*2]   = (unsigned char)tmpw;
        _buf[i*2+1] = (unsigned char)(tmpw >> 8);
    }
}

void BlockingDisk::write(unsigned long _block_no, unsigned char * _buf) {
    /* Writes 512 Bytes from the buffer to the given block on the given disk drive. */

    if(BlockingDisk::wait_queue > 0)
        BlockingDisk::wait_queue++;
    SYSTEM_SCHEDULER->wait(Thread::CurrentThread());

   // while(!is_ready())
   // {
   //     SYSTEM_SCHEDULER->reinsert();
   //     SYSTEM_SCHEDULER->yield();
   // }
    issue_operation(WRITE, _block_no);

    while(!is_ready())
    {
        SYSTEM_SCHEDULER->reinsert(Thread::CurrentThread());
        SYSTEM_SCHEDULER->yield();
    }
    BlockingDisk::wait_queue--;

    /* write data to port */
    int i; 
    unsigned short tmpw;
    for (i = 0; i < 256; i++) {
        tmpw = _buf[2*i] | (_buf[2*i+1] << 8);
        outportw(0x1F0, tmpw);
    }

}
