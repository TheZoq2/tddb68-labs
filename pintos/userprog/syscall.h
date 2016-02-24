#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

void syscall_init (void);

void sys_create(struct intr_frame* f, void* stack_ptr);
void sys_remove(struct intr_frame* f, void* stack_ptr);
void sys_open(struct intr_frame* f, void* stack_ptr);
void sys_write(struct intr_frame* f, void* stack_ptr);
void sys_read(struct intr_frame* f, void* stack_ptr);
void sys_filesize(struct intr_frame* f, void* stack_ptr);
void sys_close(void* stack_ptr);
void sys_exit(void);
void sys_exec(struct intr_frame* f, void* stack_ptr);
struct file* get_file(unsigned fd);

#endif /* userprog/syscall.h */
