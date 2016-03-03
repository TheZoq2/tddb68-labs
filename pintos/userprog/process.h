#ifndef USERPROG_PROCESS_H
#define USERPROG_PROCESS_H

#include "threads/thread.h"

tid_t process_execute (const char *file_name);
int process_wait (tid_t);
void process_exit (void);
void process_activate (void);

void check_valid_user_pointer(void* pointer);
void check_valid_user_string(char* pointer);
void check_valid_user_array(void* pointer, size_t size);

#endif /* userprog/process.h */
