#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    char *frame;
    int refcnt;
  } shm_pages[64];
} shm_table;

void shminit() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< 64; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
  }
  release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {

//you write this
  int i;
  uint sz;
  struct proc *curproc = myproc();
  sz = curproc->sz;

  for (i = 0; i < 64; i++) {
    acquire(&(shm_table.lock));

    if (shm_table.shm_pages[i].id == id) {
      shm_table.shm_pages[i].refcnt ++;
      release(&(shm_table.lock));
      sz = PGROUNDUP(sz);
      if (mappages(curproc->pgdir, (char*) sz, PGSIZE, V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U) < 0) {
        cprintf("allocuvm out of memory! \n");
        return -1;
      }
      *pointer = (char*) sz;
      return 0;
    }
    release(&(shm_table.lock));
  }

  for (i = 0; i < 64; i++) {
    acquire(&(shm_table.lock));

    if (shm_table.shm_pages[i].id == 0) {
      shm_table.shm_pages[i].id = id;
      shm_table.shm_pages[i].frame = kmalloc(curproc->pgdir, sz);
      shm_table.shm_pages[i].refcnt = 1;
      release(&(shm_table.lock));
      sz = PGROUNDUP(sz);
      if (mappages(curproc->pgdir, (char*) sz, PGSIZE, V2P(shm_table.shm_pages[i].frame), PTE_W|PTE_U) < 0) {
        cprintf("allocuvm out of memory! \n");
        return -1;
      }
      *pointer = (char*) sz;
      return 0;
    }
    release(&(shm_table.lock));
  }

return -1; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
//you write this too!
  int i;

  for (i = 0; i < 64; i++) {
    acquire(&(shm_table.lock));

    if (shm_table.shm_pages[i].id == id) {
      shm_table.shm_pages[i].refcnt --;
    }
    if (shm_table.shm_pages[i].refcnt == 0) {
      shm_table.shm_pages[i].id = 0;
      shm_table.shm_pages[i].frame = 0;
    }
    break;
  }
  release(&(shm_table.lock));
return 0; //added to remove compiler warning -- you should decide what to return
}
