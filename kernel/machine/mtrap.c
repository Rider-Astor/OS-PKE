#include "kernel/riscv.h"
#include "kernel/process.h"
#include "spike_interface/spike_utils.h"

#define USER_DEBUG_LINE 0x81400000

#define USER_SRC 0x81500000

#define SRC_BUFFER_SIZE 256

//
// 读取并打印指定文件行
//
static void print_source_line(const char *filename, int line_number) {
  spike_file_t *file = spike_file_open(filename, O_RDONLY, 0);
  if (!file) {
    sprint("Spike: Fail to open the file.\n");
    return; // 打开失败
  }
  char * buf = (char *)USER_SRC;
  int current_line = 0;
  size_t bytes_read, end = 0;
  while ((bytes_read = spike_file_read(file, buf, SRC_BUFFER_SIZE)) > 0) {
    end = bytes_read < SRC_BUFFER_SIZE ? bytes_read : SRC_BUFFER_SIZE;
    for(int i = 0; i < end;i ++){
      if(current_line == line_number - 1){
        sprint("%c", buf[i]);
      }
      if(buf[i] == '\n'){
        current_line ++;
      }
      if(current_line == line_number){
        spike_file_close(file);
        return;
      }
    }
  }
  spike_file_close(file);
  sprint("The line id is too big!\n");
}

static void handle_instruction_access_fault() { panic("Instruction access fault!"); }

static void handle_load_access_fault() { panic("Load access fault!"); }

static void handle_store_access_fault() { panic("Store/AMO access fault!"); }

static void handle_illegal_instruction() {
  process *p = current;
  uint64 mepc = read_csr(mepc);
  // sprint("mepc = %lld\n", mepc);
  // for (int i = 0; i < p->line_ind; i++)
  //   sprint("%lld %d %d\n", p->line[i].addr, p->line[i].line, p->line[i].file);
  uint64 left = 0, right = p->line_ind - 1, erraddr = 0;
  while (left <= right) {
    uint64 mid = left + (right - left) / 2;
    if (p->line[mid].addr == mepc) {
        erraddr = mid;
        break;
    } else if (p->line[mid].addr < mepc) {
        left = mid + 1;  // 在右半部分继续搜索
    } else {
        right = mid - 1; // 在左半部分继续搜索
    }
  }
  if(p->line[erraddr].addr != mepc) sprint("ERROR in locating the line of code!\n");

  code_file *file = &(p->file[p->line[erraddr].file]);
  char *dir = p -> dir[file->dir];
  sprint("Runtime error at %s/%s:%d\n", dir, file->file, p->line[erraddr].line);
  int len = 0;
  char *path = (char *)(USER_SRC - SRC_BUFFER_SIZE);
  for(int i = 0;dir[i] != '\0'; i ++){
    path[len ++] = dir[i];
  }
  path[len ++] = '/';
  for(int i = 0;file->file[i] != '\0'; i ++){
    path[len ++] =file->file[i];
  }
  path[len] = '\0';
  print_source_line(path, p->line[erraddr].line);
  panic("Illegal instruction!");
}

static void handle_misaligned_load() { panic("Misaligned Load!"); }

static void handle_misaligned_store() { panic("Misaligned AMO!"); }

// added @lab1_3
static void handle_timer() {
  int cpuid = 0;
  // setup the timer fired at next time (TIMER_INTERVAL from now)
  *(uint64*)CLINT_MTIMECMP(cpuid) = *(uint64*)CLINT_MTIMECMP(cpuid) + TIMER_INTERVAL;

  // setup a soft interrupt in sip (S-mode Interrupt Pending) to be handled in S-mode
  write_csr(sip, SIP_SSIP);
}

//
// handle_mtrap calls a handling function according to the type of a machine mode interrupt (trap).
//
void handle_mtrap() {
  uint64 mcause = read_csr(mcause);
  switch (mcause) {
    case CAUSE_MTIMER:
      handle_timer();
      break;
    case CAUSE_FETCH_ACCESS:
      handle_instruction_access_fault();
      break;
    case CAUSE_LOAD_ACCESS:
      handle_load_access_fault();
    case CAUSE_STORE_ACCESS:
      handle_store_access_fault();
      break;
    case CAUSE_ILLEGAL_INSTRUCTION:
      // TODO (lab1_2): call handle_illegal_instruction to implement illegal instruction
      // interception, and finish lab1_2.
      handle_illegal_instruction();

      break;
    case CAUSE_MISALIGNED_LOAD:
      handle_misaligned_load();
      break;
    case CAUSE_MISALIGNED_STORE:
      handle_misaligned_store();
      break;
    default:
      sprint("machine trap(): unexpected mscause %p\n", mcause);
      sprint("            mepc=%p mtval=%p\n", read_csr(mepc), read_csr(mtval));
      panic( "unexpected exception happened in M-mode.\n" );
      break;
  }
}
