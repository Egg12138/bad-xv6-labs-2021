#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


uint64
sys_trace(void)
{
  // first arg is the mask -> arg(0)    
  // in case of mask = SYS_1 | SYS_2 ?
  // if proc->trapframe->a7 == SYS_TRACE_ID
  //  turn on 
  int mask; 
  if (argint(0, &mask) < 0)
    return -1;
  struct proc *p = myproc(); 
  // set the trigger
  p->trapframe->a6 = (1 << 22);
  p->trace_mask = mask;
  return   1;
}

/// @brief load freememory and proc_num && copy sysinfo to user space
/// @param p(from_argaddr): struct sysinfo *p, (传入一个空的info进来)
/// @return 
uint64 
sys_sysinfo(void)
{
  uint64 sinfo_va;  
  struct sysinfo sinfo;
  struct proc *p = myproc();
  if (argaddr(0, &sinfo_va) < 0) 
    return -1;
  // load info to the empty sysinfo struct.
  sinfo.freemem = free_bytes();
  sinfo.nproc = proc_num();
  // copyout
  if (copyout(p->pagetable, sinfo_va, (char *)&sinfo, sizeof(sinfo)) < 0)
    return -1;
  return 0;
}

// static 
// uint64
// sysstat(struct proc *p, uint64 va)
// {
//   struct sysinfo si;
//   if (copyout(p->pagetable, ..., (char *)va, sizeof(si) ))
//   return 0;
// }


