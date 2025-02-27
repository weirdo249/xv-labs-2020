#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)//终止当前进程，并返回状态码 n
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)//返回当前进程的 PID。
{
  return myproc()->pid;
}

uint64
sys_fork(void)//创建一个子进程。
{
  return fork();
}

uint64
sys_wait(void)//等待子进程退出，并获取其退出状态
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;//通过 argaddr(0, &p) 获取用户态传递的地址，用于存储子进程的退出状态
  return wait(p);
}

uint64
sys_sbrk(void)//调整进程的堆大小。
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
sys_sleep(void)//让当前进程休眠指定的时钟滴答数。
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
