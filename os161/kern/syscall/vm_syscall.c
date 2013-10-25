#include <types.h>
#include <kern/errno.h>
#include <kern/syscall.h>
#include <lib.h>
#include <mips/trapframe.h>
#include <thread.h>
#include <current.h>
#include <syscall.h>

int startS();
int process();

int sys_testvm(){
int res=kprintf("VM Sim startd... \n");
int result;

result = thread_fork_id("VM Simulator",1,100,startS,NULL,0,NULL);
if (result) 
 {kprintf("thread_fork failed: %s\n", strerror(result));
  return result;
 }

return res;
}


int startS()
{
 kprintf("Started reading specs... \n");

 int result;
 result = thread_fork_id("process1",2,10,process,NULL,0,NULL);
 //result = thread_fork_id("process1",3,250,process,NULL,0,NULL);
 //result = thread_fork_id("process1",4,200,process,NULL,0,NULL);
 return 0;
}

int process(){
 struct thread *cur;
 cur = curthread;
 int id =cur->t_id;
 kprintf("process %d started...Memory required : %d \n\n\n",id,cur->maxpages);


 kprintf("Requested page 1\n");
 int p=translate(1,1);
 kprintf("Translated address : %d \n\n",p);
 
 kprintf("Requested page 3\n");
 p=translate(3,1);
 kprintf("Translated address : %d \n\n",p);

 kprintf("Requested page 12\n");
 p=translate(12,1);
 kprintf("Translated address : %d \n\n",p);

 kprintf("Requested page 1\n");
 p=translate(1,1);
 kprintf("Translated address : %d \n\n",p);

 kprintf("Requested page 5\n");
 p=translate(5,1);
 kprintf("Translated address : %d \n\n",p);

 kprintf("Requested page 7\n");
 p=translate(7,1);
 kprintf("Translated address : %d \n\n",p);

 kprintf("Requested page 5\n");
 p=translate(5,1);
 kprintf("Translated address : %d \n\n",p);
 //p=translate(1);
 //kprintf("Translated address : %d \n",p);
	//if (curthread->t_addrspace==NULL) {
	//	return ENOMEM;
	//}
	//as_activate(curthread->t_addrspace);
}
