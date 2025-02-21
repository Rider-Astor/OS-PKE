/*
 * Supervisor-mode startup codes
 */

#include "riscv.h"
#include "string.h"
#include "elf.h"
#include "process.h"
#include "config.h"
#include "spike_interface/spike_utils.h"

int hartid;
// process is a structure defined in kernel/process.h
process *user_app = (process*) HART_CONTEXT_ADDR + 64;
hart_context* hart = (hart_context*)HART_CONTEXT_ADDR;

//
// load the elf, and construct a "process" (with only a trapframe).
// load_bincode_from_host_elf is defined in elf.c
//
void load_user_program(process *proc) {
  // USER_TRAP_FRAME is a physical address defined in kernel/config.h
  if(hartid == 0){
    proc->trapframe = (trapframe *)USER_TRAP_FRAME1;
    memset(proc->trapframe, 0, sizeof(trapframe));
    // USER_KSTACK is also a physical address defined in kernel/config.h
    proc->kstack = USER_KSTACK1;
    proc->trapframe->regs.sp = USER_STACK1;
  }
  else{
    proc->trapframe = (trapframe *)USER_TRAP_FRAME2;
    memset(proc->trapframe, 0, sizeof(trapframe));
    // USER_KSTACK is also a physical address defined in kernel/config.h
    proc->kstack = USER_KSTACK2;
    proc->trapframe->regs.sp = USER_STACK2;
  }
  hart = (hart_context*) (hart + hartid * 32);
  hart->hartid = hartid;
  hart->proc = proc;
  proc->trapframe->regs.tp = (uint64)hart;
  // sprint("hartid : %d; proc : %lx\n", hart->hartid, hart->proc);
  // load_bincode_from_host_elf() is defined in kernel/elf.c
  load_bincode_from_host_elf(proc);
}

//
// s_start: S-mode entry point of riscv-pke OS kernel.
//
int s_start(void) {
  hartid = read_tp();

  sprint("hartid = %d: Enter supervisor mode...\n", hartid);
  // Note: we use direct (i.e., Bare mode) for memory mapping in lab1.
  // which means: Virtual Address = Physical Address
  // therefore, we need to set satp to be 0 for now. we will enable paging in lab2_x.
  // 
  // write_csr is a macro defined in kernel/riscv.h
  write_csr(satp, 0);

  // the application code (elf) is first loaded into memory, and then put into execution
  load_user_program(user_app + hartid * sizeof(process));

  sprint("hartid = %d: Switch to user mode...\n", hartid);
  // switch_to() is defined in kernel/process.c
  switch_to(user_app + hartid * sizeof(process));

  // we should never reach here.
  return 0;
}
