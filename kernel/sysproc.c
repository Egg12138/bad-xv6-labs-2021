#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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

/// @param: uint64(as addr), int(number), uint64(as addr)
/// @brief:  detecting and reporting which pages have bee accessed.
/// @note: (recursive)by inspecting the access bits in the RISC-V pagetable . Hardware page walker marks these bits in the PTE. (dont worry about the TLB miss) 
#ifdef LAB_PGTBL
uint64
sys_pgaccess(void)
{
  // ToBeImpl: Implement this syscall .....
  // lab pgtbl: your code here
  // takes thress arguments: start va of 1st user page; 
  // using p->trampframe->aX to get args.
  // using argstr, argaddr, argraw..., etc.
  // struct proc *p = myproc();
  // TODO: make sure the type of three args are right
  uint64 buf_add;                          // arg0:=base_va to store arg0=>intput buf(pointer to (char *)buffer),  
  int pgnum;                               // arg1:=pgnum to store num of pages 2b check=>input (int num), the value of num, itself.
  uint64 mask;                             // arg2:=mask to store access bit mask=>input &abit(pointer to (uint64) abits) 
  // we will modified *mask => 1<<1 | 1<<2 | 1<<30
  //char buffer[CHCKPGNUM];  // ont bit, ont status.
  if (((argaddr(0, &buf_add)) < 0) ||  (argint(1, &pgnum) < 0) || (argaddr(2, &mask)))
    return -1;
  // TODO: Find PTEs. using `vm::walk` 
  return pgaccess((void *)buf_add, pgnum, (void *)mask);
}
#endif

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
