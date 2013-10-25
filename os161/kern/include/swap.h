/*struct swap_space{
	struct vnode *swap_file;
	struct swap_page_table *spt;
	struct swap_page_table *fst;
}

struct swap_page_table{
	struct swap_page sft[20];
}

struct swap_page {
	int pid;
	off_t offset;
	v_addr vaddr;
	p_addr paddr;
}

void swap_space_init(struct swap_space *sp){
	
}
*/

#include <types.h>
#include <kern/errno.h>
#include <kern/fcntl.h>
#include <lib.h>
#include <uio.h>
#include <thread.h>
#include <current.h>
#include <addrspace.h>
#include <vnode.h>
#include <elf.h>
#include <vm.h>
#include <vfs.h>
#include <syscall.h>
#include <test.h>

#define TABLE_SZ 128
#define SEG_SZ 1024

struct vnode* swap_file;
vaddr_t addrptr[TABLE_SZ];
int offset[TABLE_SZ];
int free[TABLE_SZ]; // o for free, 1 for occupied
int pid[TABLE_SZ]; // -1 for unoccupied

static void swap_init(void){
	
	kprintf("\n\n\n<-swap space initialization starting->\n");
	Elf_Phdr ph;
	size_t segment_size = ph.p_filesz;
	segment_size = SEG_SZ;
	
	kprintf("swap seg size: %lu\n",(unsigned long) segment_size);
	
	int result;
	result =vfs_open("swap",O_WRONLY, 0664, &swap_file);
	
	if (result) {
		kprintf("unable to open swap file for writing\n");
	}
	else kprintf("swap file opened\n");
	
	int i;
	for(i=0;i<TABLE_SZ;i++){
		offset[i]=i*SEG_SZ;
		//kprintf("offset %d \n",offset[i]);
		free[i]=0;	
		pid[i]=-1;
		addrptr[i]=-1;
	}
	kprintf("swap segments offset set. last segment offset: %lu\n",(unsigned long) offset[TABLE_SZ-1]);
	kprintf("<-swap space initialization end->\n\n\n");
	
}

static void write_to_swap(struct vnode* v, int segmentsize, off_t seg_offset, vaddr_t vaddr){
	
	Elf_Ehdr eh;   /* Executable header */
	Elf_Phdr ph;   /* "Program header" = segment header */
	int result;
	struct iovec iov;
	struct uio ku;

	//write_to_swap(entrypoint);

	/*
	 * Read the executable header from offset 0 in the file.
	 */

	//struct vnode *v;
	//struct iovec iov;
	struct uio u;
	
	//int result;
/*
	result = vfs_open(progname, O_RDONLY, 0, &v);
	
	if (result) {
		kprintf("unable to open exec file for reading\n");
	}
	else kprintf("exec file opened\n");
*/
	/*
	uio_kinit(&iov, &ku, &eh, sizeof(eh), 0, UIO_READ);
	result = VOP_READ(v, &ku);
	kprintf("ASDASDAAAAAAAAAAAAAAAAAAAAAAAAAAAA%lu",(unsigned long)sizeof(eh));
	*/
	int i;
	for(i = 0;i<=4*segmentsize/SEG_SZ+4;i++){	
		u.uio_iovcnt = 1;
    	u.uio_resid = SEG_SZ;          // amount to read from the file
    	u.uio_offset = seg_offset+SEG_SZ*i;
    //u.uio_segflg = is_executable ? UIO_USERISPACE : UIO_USERSPACE;
    	u.uio_rw = UIO_READ;
    	u.uio_space = curthread->t_addrspace;

    	char buf[SEG_SZ];
    	uio_kinit(&iov, &u, buf, SEG_SZ, 0, UIO_READ);
    	result = VOP_READ(v, &u);

		int page_num = find_free_pagenum();

		kprintf("writing segment no: %d ", i);
		kprintf("to offset : %d ",offset[page_num]);
		kprintf("and pageNum : %d ",page_num);
		free[page_num]=1;
		addrptr[page_num]=vaddr-seg_offset+i*SEG_SZ;

		kprintf("svaddr of this swap page seg is %lu\n", (unsigned long)addrptr[page_num]);
		
		u.uio_offset = offset[page_num];
    	u.uio_resid = SEG_SZ;
    	u.uio_iovcnt = 1;
    	uio_kinit(&iov, &u, buf, SEG_SZ, 0, UIO_WRITE);
    //u.uio_segflg = is_executable ? UIO_USERISPACE : UIO_USERSPACE;
    	u.uio_rw = UIO_WRITE;
    
    	result = VOP_WRITE(swap_file, &u);
    }
	
}

char* read_seg_from_swap(vaddr_t vaddr);

int find_free_pagenum();

void write_seg_to_swap(char* buf, vaddr_t vaddr);

