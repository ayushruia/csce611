
/*page table written by ayush ruia*/
#include "page_table.H"
#include "paging_low.H"
#include "console.H"

FramePool*  PageTable::kernel_mem_pool;
FramePool*  PageTable::process_mem_pool;
unsigned int PageTable::paging_enabled;
unsigned long PageTable::shared_size;
const unsigned int PageTable::PAGE_SIZE;
const unsigned int PageTable::ENTRIES_PER_PAGE;
PageTable* PageTable::current_page_table;

void PageTable::init_paging(FramePool * _kernel_mem_pool, FramePool * _process_mem_pool, const unsigned long _shared_size)
{
    PageTable::kernel_mem_pool = _kernel_mem_pool;
    PageTable::process_mem_pool = _process_mem_pool;
    PageTable::shared_size = _shared_size;
    //Console::puts("init_paging\n");
}

PageTable::PageTable()
{
    unsigned long page_dir = (PageTable::process_mem_pool->get_frame()); //holds the page directory address
    unsigned long page_tab = (PageTable::process_mem_pool->get_frame()); //holds the page table
    unsigned long *page_table;
    unsigned long address=0; // holds the physical address of where a page is
    Console::puts("\nAddress of the page directory is ");
    Console::putui(page_dir);
    page_dir <<=12; //bit shifting to get the physical address
    page_tab <<=12; //bit shifting to get the physical address
    Console::puts("\nAddress of the page directory is after shift ");
    Console::putui(page_dir);
    page_directory = (unsigned long *) page_dir;
    page_table = (unsigned long *) page_tab;
    page_directory[0] = page_tab | 3; //setting the value in the page directory to present and supervisor
    // map the first 4MB of memory
    for(unsigned int i=0; i<PageTable::ENTRIES_PER_PAGE; i++)
    {
        page_table[i] = address | 3; // attribute set to: supervisor level, read/write, present(011 in binary)
        address = address + 4096; // 4096 = 4kb
    }
    for(unsigned int i=1; i<PageTable::ENTRIES_PER_PAGE-1; i++)
    {
        page_directory[i] = 0;
    }
    page_directory[PageTable::ENTRIES_PER_PAGE-1] = page_dir | 3; // implementing recursive lookup
    //Console::puts("exiting constructor\n");
}

void PageTable::load()
{
    current_page_table = this; //pointing to the current page table
    write_cr3((unsigned long)page_directory); // put that page directory address into CR3
    //Console::puts("in the load part\n");
}

void PageTable::enable_paging()
{
    //Console::puts("in the enable paging\n");
    PageTable::paging_enabled = 1;
    write_cr0(read_cr0() | 0x80000000); // set the paging bit in CR0 to 1
    //Console::puts("in the enable paging\n");
}

void PageTable::handle_fault(REGS * _r)
{

    unsigned long pd_index;
    unsigned long pt_index;
    const unsigned long pt_bitmask = 0x3ff000;
    const unsigned long pd_bitmask = 0xffc00000;
    const unsigned long add_bitmask = 0xfffff000;
    unsigned long *page_tb;
    unsigned int error_code = (_r->err_code&7);
    unsigned long pf_address = read_cr2();
    unsigned long pd_frame;
    unsigned long pt_frame;
    unsigned int fld_val;
    unsigned long *page_d;
    unsigned long page_d_map = add_bitmask;
    page_d = (unsigned long *) page_d_map;
    if((error_code & 1) == 0)   //page not present
    {
        pd_index = pf_address>>22;
        page_tb = (unsigned long *)((pd_bitmask)|(pd_index<<12));
        if((page_d[pd_index] & 1) == 1)     //page directory is present
        {
            pt_frame = PageTable::process_mem_pool->get_frame();
            pt_frame <<=12;
            fld_val = (error_code&4>0)?7:3;
            pt_frame |=fld_val;
            pt_index = (pf_address&pt_bitmask)>>12;
            page_tb[pt_index] = pt_frame;
        }
        else        //page directory not present
        {
            pd_frame = PageTable::process_mem_pool->get_frame();
            pd_frame <<=12;
            fld_val = (error_code&4>0)?7:3; //checking for kernel memory fault or user memory fault
            pd_frame |=fld_val;
            page_d[pd_index] = pd_frame;
            pt_frame = PageTable::process_mem_pool->get_frame();
            pt_frame <<=12;
            pt_frame |=fld_val;
            pt_index = (pf_address&pt_bitmask)>>12;
            for(int i=0;i<1024;i++)
                page_tb[i] = 0;
            page_tb[pt_index] = pt_frame;
        }
    }
    else
        Console::puts("Protection fault detected\n");
}

