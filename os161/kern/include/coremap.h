#ifndef _COREMAP_H_
#define _COREMAP_H_

/*
 * The coremap paddr->vaddr is used for paging out.
 * 
 */

/*
 * coremap_entry: paddr | used | pid | npages
 *                20bits| 1bit |5bits| 6bits
 */
struct coremap_entry {

        pid_t           pid;                    /* identify process */
//      vaddr_t         vaddr;
        paddr_t         paddr;                  /* aligned */
        bool            used; 
        int                     npages;                 /* for free() */
};

paddr_t coremap_alloc(unsigned long npages);
void coremap_free(paddr_t paddr);


#endif /* _COREMAP_H_ */
