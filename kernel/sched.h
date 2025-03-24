#ifndef _SCHED_H_
#define _SCHED_H_

#include "process.h"
#include "spike_interface/atomic.h"

//length of a time slice, in number of ticks
#define TIME_SLICE_LEN  2

typedef struct semaphore_t {
    uint64 count; // resources remained
    process *queue;
    spinlock_t sem_lock;
}semaphore;

void insert_to_ready_queue( process* proc );
void schedule();

uint64 do_semNew(int free_count);
uint64 do_semP(uint64 sem_id);
uint64 do_semV(uint64 sem_id);

#endif
