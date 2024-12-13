/*
 * contains the implementation of all syscalls.
 */

#include <stdint.h>
#include <errno.h>

#include "util/types.h"
#include "syscall.h"
#include "string.h"
#include "process.h"
#include "util/functions.h"
#include "elf.h"

#include "spike_interface/spike_utils.h"

extern elf_symbol elf_symbols[64];
extern char sym_names[64][32];
extern int sym_count;


//
// implement the SYS_user_print syscall
//
ssize_t sys_user_print(const char* buf, size_t n) {
  sprint(buf);
  return 0;
}

//
// implement the SYS_user_exit syscall
//
ssize_t sys_user_exit(uint64 code) {
  sprint("User exit with code:%d.\n", code);
  // in lab1, PKE considers only one app (one process). 
  // therefore, shutdown the system when the app calls exit()
  shutdown(code);
}

//stop when meet func 'main'
int func_print_name(uint64 ret_addr){
  // sprint("%d\n", sym_count);
  for(int i = 0;i < sym_count;i ++){
    if(ret_addr >= elf_symbols[i].st_value && ret_addr < elf_symbols[i].st_value + elf_symbols[i].st_size){
      sprint("%s\n", sym_names[i]);
      if(strcmp(sym_names[i], "main") == 0) return 0;
      return 1;
    }
  }
  return 1;
}

ssize_t sys_print_backtrace(uint64 depth){
  uint64 trace_sp = current->trapframe->regs.sp + 32;
  uint64 trace_ra = trace_sp + 8;
  uint64 tracer = 0;
  for(; tracer < depth; tracer ++){
    if(func_print_name(*(uint64 *) trace_ra) == 0) return tracer;
    trace_ra += 16;
  }
  return tracer;
}

//
// [a0]: the syscall number; [a1] ... [a7]: arguments to the syscalls.
// returns the code of success, (e.g., 0 means success, fail for otherwise)
//
long do_syscall(long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7) {
  switch (a0) {
    case SYS_user_print:
      return sys_user_print((const char*)a1, a2);
    case SYS_user_exit:
      return sys_user_exit(a1);
    case SYS_print_backtrace:
      return sys_print_backtrace(a1);
    default:
      panic("Unknown syscall %ld \n", a0);
  }
}
