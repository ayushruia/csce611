
/*frame pool manager written by ayush ruia for csce 611*/

#include "frame_pool.H"
#include "console.H"


#define GET_ADDR(frame) (frame<<12)


FramePool::FramePool(unsigned long _base_frame_no,
             unsigned long _nframes,
             unsigned long _info_frame_no)
{
    base_frame_no = _base_frame_no; //variable declared in frame_pool.H
    nframes = _nframes;
    if(_info_frame_no<base_frame_no || _info_frame_no>base_frame_no+nframes-1)
        info_frame_no = 0;
    else
        info_frame_no = _info_frame_no; //representing the info frame
    unsigned long temp = info_frame_no;
    temp <<=12; //converting addresses
    bit_map = (unsigned long *)temp; //pointer to the info frame location
    sizeof_bit_map = sizeof(unsigned long)*8;
    for(int i=0; i<nframes/sizeof_bit_map+1;i++)
        *(bit_map + i) = 0; //initializing all the frames to available. The boundary of this bitmap is a 4 byte boundary.
    mark_inaccessible(info_frame_no+base_frame_no,1); // marking the info frame as inaccessible
    mark_inaccessible(base_frame_no + nframes, sizeof_bit_map - nframes%sizeof_bit_map); //This is necessary for my implementation of bit map
    // In my implementation, I keep on checking for a frame till the end of the 4 byte boundary, that is the size of long. Thus I need to mark those frames not present in the frame pool manager as unavailable.
    //Console::puts("FramePool constructor\n");
}

unsigned long FramePool::get_frame()
{
    unsigned int count = 0;
    for(int i=0; i<nframes/sizeof_bit_map+1;i++)
    {
        unsigned long temp = ~(*(bit_map + i)); //Checking if any of the bits are 0
        unsigned long bit_mask = 1;
        count += sizeof_bit_map; //incrementing the frame offset count to the next 4 byte boundary
        if(temp > 0)        //if any frame is present then checking for which bit
        {
            while(temp != 1) //searching for the first non zero bit in the whole word
            {
                temp = temp >>1;
                bit_mask = bit_mask << 1;
                count--;    //Decrementing the earlier estimate as we had already crossed over to the next word boundary of 32 frames
            }
            *(bit_map + i) = *(bit_map + i) | bit_mask;     //marking the bit as unavailable
            break;
        }
    }
    //Console::puts("FramePool get_frame\n");
    if(count > nframes)
        return 0;
    return base_frame_no + count; //Returning the frame count
}

void FramePool::mark_inaccessible(unsigned long _base_frame_no, unsigned long _nframes)
{
    unsigned long offset_frame_no = _base_frame_no - base_frame_no;
    unsigned long offset = offset_frame_no/sizeof_bit_map;
    unsigned long bitmask = 1;
    unsigned long count=0;
    bitmask = bitmask<<(sizeof_bit_map - offset_frame_no%sizeof_bit_map -1); //setting the bit mask to the appropriate mask
    while(count<_nframes)
    {
        *(bit_map + offset) |= bitmask; //marking the requisite location as 1
        count++;
        bitmask >>= 1;
        if(bitmask == 0)
        {
            bitmask = 1;
            bitmask <<= sizeof_bit_map -1;
            offset++;
        }
    }
    //Console::puts("FramePool mark_inaccesible\n");
}

void FramePool::release_frame(unsigned long _frame_no)
{
    unsigned long offset_frame_no = _frame_no - base_frame_no;
    unsigned long offset = offset_frame_no/sizeof_bit_map;
    unsigned long bitmask = 1;
    bitmask = bitmask<<(sizeof_bit_map - offset_frame_no%sizeof_bit_map -1); //setting the bit mask to the appropriate mask
    bitmask = ~bitmask;
    *(bit_map + offset) &= bitmask; //marking the requisite location as 0
}

