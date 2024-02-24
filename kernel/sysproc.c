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
  // return myproc()->pid;
  struct usyscall *u = (struct usyscall *)USYSCALL;
  return u->pid;
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


/// tree args:
/// 1. (addr)VA of the FIRST User page to check
/// 2. (int)number of pages to check
/// 3. (addr)UserVA of a buffer to store the result? into a bitmask
/// result: (bitmap) 1 accessed, 0 havn't been accessed yet
#ifdef LAB_PGTBL
// typedef struct PageAccessBitmap {
//   uint64 start_va;
//   uint64 bitmap;
// } Bitmap;
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 usr_pg_va, bufp;
  int max_pgnum;
  uint64 bitmap;
  struct proc *p = myproc();
#define BITMAP_SET(i)  (bitmap |= (1 << (i)))
#define PAGE_ACCESSED(pte) (PTE_FLAGS(*(pte)) & PTE_A)
#define CLEAR_ACCESSED(pte) (*(pte) &= (~PTE_A))
#define NOT_ALLOCATE 0

  if (argaddr(0, &usr_pg_va) < 0 
      || argaddr(2, &bufp) < 0
      || argint(1, &max_pgnum) < 0)
    return -1;


  // store temp bitmap 
  for (int pgnum = 0; pgnum < max_pgnum; pgnum ++) {
    pte_t *pte = walk(
                    p->pagetable, 
                    usr_pg_va + pgnum * PGSIZE, 
                    NOT_ALLOCATE);
    if (PAGE_ACCESSED(pte)) {
      BITMAP_SET(pgnum);
      CLEAR_ACCESSED(pte);
    }
  }
  
  // copyout to user space
  if (copyout(p->pagetable, bufp, (char *)&bitmap, sizeof(bitmap)) < 0)
    return -1;

  return 0;
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
