#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <uio.h>
#include <thread.h>
#include <current.h>
#include <addrspace.h>
#include <vnode.h>
#include <elf.h>
#include <swap.h>


/*
struct vnode* swap_file;

vaddr_t start_vaddr[TABLE_SZ];
int free[TABLE_SZ];

static void swap_init(void){
	kprintf("I AM HERE3\n");
	Elf_Phdr ph;
	size_t segment_size = ph.p_filesz;
	kprintf("swap seg size: %lu\n\n\n",(unsigned long) segment_size);
}
*/   

int find_free_pagenum(){
	int i;
	for(i=0;i<TABLE_SZ;i++){
		if(free[i]==0){return i;}
	}
	
	return -1;
}

char* read_seg_from_swap(vaddr_t vaddr){
	kprintf("About to read segment from swap. given vaddress: %lu \n", (unsigned long)vaddr);
	char buf[SEG_SZ];
		
	struct iovec iov;
	struct uio u;

	int i, result, segNum= -1;
	
	for(i=0;i<TABLE_SZ;i++){
		if(addrptr[i]!=-1 && vaddr>=addrptr[i] && vaddr<addrptr[i]+SEG_SZ){
			segNum = i;
		}
	}

	
	if(segNum==-1){}//kprintf("segment not found\n");}
	else kprintf("found seg no %d \n",segNum);
	
	u.uio_iovcnt = 1;
	u.uio_resid = SEG_SZ;          // amount to read from the file
	u.uio_offset = offset[segNum];
	//u.uio_segflg = is_executable ? UIO_USERISPACE : UIO_USERSPACE;
	u.uio_rw = UIO_READ;
	u.uio_space = curthread->t_addrspace;

	uio_kinit(&iov, &u, buf, SEG_SZ, 0, UIO_READ);
	result = VOP_READ(swap_file, &u);
	return buf;

}

void write_seg_to_swap(char* buf, vaddr_t vaddr){
		
	struct iovec iov;
	struct uio u;

	int i, result, segNum = -1;
	
	for(i=0;i<TABLE_SZ;i++){
		if(addrptr[i]!=-1 && vaddr>=addrptr[i] && vaddr<addrptr[i]+SEG_SZ){
			segNum = i;
		}
	}	
	
	if(segNum==-1){kprintf("segment not found\n");}

	u.uio_iovcnt = 1;
    	u.uio_resid = SEG_SZ;          // amount to read from the file
    	u.uio_offset = offset[segNum];
    	//u.uio_segflg = is_executable ? UIO_USERISPACE : UIO_USERSPACE;
    	u.uio_rw = UIO_READ;
    	u.uio_space = curthread->t_addrspace;

    	uio_kinit(&iov, &u, buf, SEG_SZ, 0, UIO_WRITE);
    	result = VOP_WRITE(swap_file, &u);

	return buf;

}
