#include <types.h>
#include <kern/errno.h>
#include <kern/fcntl.h>
#include <kern/syscall.h>
#include <lib.h>
#include <uio.h>
#include <mips/trapframe.h>
#include <thread.h>
#include <current.h>
#include <syscall.h>
#include <addrspace.h>
#include <vfs.h>
#include <vnode.h>
#include <swap.h>

//#define SEG_SZ 24

int sys_helloworld(){
int res=kprintf("Hello World \n");
/*
struct vnode *v;
struct vnode *out;
struct iovec iov;
struct uio u;

res = vfs_open("swap", O_RDONLY, 0, &v);
if (res) {
	kprintf("open failed\n");	
	return res;
}

kprintf("open read\n");
res = vfs_open("swap1", O_WRONLY, 0, &out);
if (res) {
        kprintf("open failed\n");	
	return res;
}
kprintf("open write\n");


       u.uio_iovcnt = 1;
       u.uio_resid = SEG_SZ;          // amount to read from the file
       u.uio_offset = 0;
       //u.uio_segflg = is_executable ? UIO_USERISPACE : UIO_USERSPACE;
       u.uio_rw = UIO_READ;
       u.uio_space = curthread->t_addrspace;


kprintf("asdfasdf\n");
	char buf[24];
	uio_kinit(&iov, &u, buf, SEG_SZ, 0, UIO_READ);
       res = VOP_READ(v, &u);
kprintf("asdfasdf\n");
       u.uio_offset = 0;
       u.uio_resid = SEG_SZ;
       u.uio_iovcnt = 1;
	uio_kinit(&iov, &u, buf, SEG_SZ, 0, UIO_WRITE);
       //u.uio_segflg = is_executable ? UIO_USERISPACE : UIO_USERSPACE;
       u.uio_rw = UIO_WRITE;
kprintf("asdfasdf\n");       
       res = VOP_WRITE(out, &u);
*/

return res;
}


int sys_printint(int n){
int res=kprintf("%d",n);
return res;
}

int sys_printstring(char* s,int n){
//finding legth
int res=0;
int l=0;
int i=0;
for (i=0;s[i]!='\0';i++){l++;}
if (n>l) {for (i=0;i<l;i++) res=kprintf("%c",s[i]); res=100;}
else {for (i=0;i<n;i++) res=kprintf("%c",s[i]);}
return res;
}

int sys_mysleep(int n){
int res=kprintf("went into sleep %d\n",n);
res=kprintf("out of sleep\n");
return res;
}


paddr_t sys_faultman(struct thread* t,vaddr_t v,int type){
int res;//=kprintf("Fault Manager\n");
int i;
int freeno;
bool freefound=false;
int index;
paddr_t replace;
bool replaced=false;


   for (i=0;i<MEM_PAGES;i++)
     if (freepagelist[i].free==0) {freeno=i;freefound=true;break;}

   if (!freefound)
   {kprintf("No free page found ,applying FIFO Page Replacement Algo\n");
    replace=sys_pagereplace(t);
    kprintf("replacing page %d\n",replace);
    replaced=true;
   }


   kprintf("swapin\n");
   //freeno=1;
   char* buf=(char*)kmalloc(4096*sizeof(char));
   buf=read_seg_from_swap(v);

	paddr_t pa;
	//pa = getppages(1);
	pa=startp+freeno*PAGE_SIZE;
        buf=pa;
	if (replaced) {
	pa=replace;
        freeno=(pa-startp)/PAGE_SIZE;
	}
   vaddr_t base;
   kprintf("phya is %d : \n" ,pa);

   	
   

   if (type==1){
   base=t->t_addrspace->as_vbase1;
   index=(v-base)/4096;
   freepagelist[freeno].free=1;
   t->pagetable1[index].valid=1;
   t->pagetable1[index].p=pa;
   Enqueue(t->Q,index);
   }

   else if (type==2){
   base=t->t_addrspace->as_vbase2;
   index=(v-base)/4096;
   freepagelist[freeno].free=1;
   t->pagetable2[index].valid=1;
   t->pagetable2[index].p=pa;
   Enqueue(t->Q,index);
   }

   else if (type==0){
   freepagelist[freeno].free=1;
   t->pagetable[v].valid=1;
   t->pagetable[v].p=pa;
   Enqueue(t->Q,v);
   }
return pa;
}



int sys_pagereplace(struct thread* t){
int res=t->pagetable[front(t->Q)].p;
t->pagetable[front(t->Q)].valid=0;
vaddr_t vaddr=0,base;
//base=t->t_addrspace->as_vbase1;
//vaddr=base;
char* buf=(char*)kmalloc(PAGE_SIZE*sizeof(char));
kprintf("swapout\n");
write_seg_to_swap(buf, vaddr);
kprintf("Front element is %d , swapping out from physical page %d \n",front(t->Q),res);
Dequeue(t->Q);
return res;
}

