#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"
#include "devices/input.h"
#include "userprog/process.h"
#include "threads/vaddr.h"
#include "pagedir.h"

#define MIN_FILE_ID 2

static void syscall_handler (struct intr_frame *);

bool is_valid_user_pointer(void* pointer);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

struct file* get_file(unsigned fd)
{
  if(fd < MAX_PROCESS_FILES)
  {
    return thread_current()->open_files[fd];
  }
  return NULL;
}

static void incr_stack_ptr_with_chk(void** ptr, unsigned amount)
{
  *ptr += amount;

  check_valid_user_pointer(*ptr);
}

static void
syscall_handler (struct intr_frame *f)
{
  check_valid_user_pointer(f->esp);

  int syscall_id = *((int*)f->esp);
  void* stack_ptr = f->esp;

  incr_stack_ptr_with_chk(&stack_ptr, sizeof(void*));


  switch(syscall_id)
  {
    case  SYS_HALT:
    {
      power_off();
      break;
    }
    case SYS_EXIT:
    {
      sys_exit(stack_ptr);
      break;
    }
    case SYS_EXEC:
    {
      sys_exec(f, stack_ptr);
      break;
    }
    case SYS_WAIT:
    {
      sys_wait(f, stack_ptr);
      break;
    }
    case SYS_CREATE:
    {
      sys_create(f, stack_ptr);
      break;
    }
    case SYS_REMOVE:
    {
      sys_remove(f, stack_ptr);
      break;
    }
    case SYS_OPEN:
    {
      sys_open(f, stack_ptr);
      break;
    }
    case SYS_FILESIZE:
    {
      sys_filesize(f, stack_ptr);
      break;
    }
    case SYS_READ:
    {
      sys_read(f, stack_ptr);
      break;
    }
    case SYS_WRITE:
    {
      sys_write(f, stack_ptr);

      break;
    }
    case SYS_SEEK:
    {
      sys_seek(stack_ptr);
      break;
    }
    case SYS_TELL:
    {
      sys_tell(stack_ptr);
      break;
    }
    case SYS_CLOSE:
    {
      sys_close(stack_ptr);
      break;
    }
    default:
      thread_exit_with_status(-1);
  }
}

void sys_create(struct intr_frame *f, void* stack_ptr)
{
  //Fetch the filename and size
  char* filename = *(char**)stack_ptr;
  check_valid_user_string(filename);

  incr_stack_ptr_with_chk(&stack_ptr, sizeof(char*));
  unsigned int size = *((unsigned int*)stack_ptr);

  bool result = filesys_create(filename, size);

  f->eax = result;
}

void sys_remove(struct intr_frame* f, void* stack_ptr)
{
  //Fetch the filename and size
  char* filename = *(char**)stack_ptr;
  check_valid_user_string(filename);

  bool result = filesys_remove(filename);

  f->eax = result;
}

void sys_open(struct intr_frame* f, void* stack_ptr)
{
  struct thread* curr_thread = thread_current();

  //Kod skriven tillsammans med Hannes Tukalla
  char* filename = *(char**)stack_ptr;
  check_valid_user_string(filename);

  int file_descriptor = -1;
  struct file* opened_file = filesys_open(filename);

  if (opened_file != NULL) {
    size_t i;
    //Add the new file to the open_files of the thread
    for(i = MIN_FILE_ID; i < MAX_PROCESS_FILES; ++i)
    {
      //Is this slot is avalilable  for opening a file
      if(curr_thread->open_files[i] == NULL)
      {
        curr_thread->open_files[i] = opened_file;
        file_descriptor = i;
        break;
      }
    }
  }

  f->eax = file_descriptor;
}

void sys_write(struct intr_frame* f, void* stack_ptr)
{
  int fd = *((int*) stack_ptr);
  incr_stack_ptr_with_chk(&stack_ptr, sizeof(int*));
  void* buffer = *((void**) stack_ptr);
  incr_stack_ptr_with_chk(&stack_ptr, sizeof(void*));
  unsigned size = *((unsigned*) stack_ptr);

  check_valid_user_array(buffer, size);

  int orig_size = size;
  if (fd == STDOUT_FILENO) {
    while (size > 0) {
      int size_to_push = size > 256 ? 256 : size;
      putbuf(buffer, size_to_push);
      size -= size_to_push;
      buffer += size_to_push;
    }
    f->eax = orig_size;
  }
  else
  {
    struct file* file_to_write = get_file(fd);

    if(file_to_write != NULL)
    {
      f->eax = file_write(file_to_write, buffer, size);
    }
    else
    {
      f->eax = -1;
    }
  }
}

void sys_read(struct intr_frame* f, void* stack_ptr)
{
  int fd = *((int*) stack_ptr);
  incr_stack_ptr_with_chk(&stack_ptr, sizeof(int*));
  uint8_t* buffer = *((uint8_t**) stack_ptr);
  incr_stack_ptr_with_chk(&stack_ptr, sizeof(uint8_t*));
  unsigned size = *((unsigned*) stack_ptr);

  check_valid_user_array(buffer, size);

  int orig_size = size;
  if (fd == STDIN_FILENO) {
    // Read size characters from the keyboard
    size_t read = 0;
    while (read < size) {
      ((uint8_t*) buffer)[read] = input_getc();
      read++;
    }

    f->eax = orig_size;
  }
  else
  {
    struct file* file_to_read = get_file(fd);

    if(file_to_read != NULL)
    {
      unsigned amount_read = file_read(file_to_read, buffer, size);

      f->eax = amount_read;
    }
    else
    {
      f->eax = -1;
    }
  }
}

//aparently not required for assignment, will fail if fd is invalid
void sys_filesize(struct intr_frame* f, void* stack_ptr)
{
  //Get the file descriptor
  int fd = *((int*)stack_ptr);

  struct file* file = get_file(fd);
  if (file == NULL) {
    thread_exit_with_status(-1);
  } else {
    off_t size = file_length(file);
    f->eax = size;
  }
}

void sys_seek(void* stack_ptr)
{
  int fd = *((unsigned*)stack_ptr);
  incr_stack_ptr_with_chk(&stack_ptr, sizeof(unsigned));
  unsigned position = *((unsigned*)stack_ptr);

  struct file* f = get_file(fd);
  if (f == NULL) {
    thread_exit_with_status(-1);
  } else {
    file_seek(f, position);
  }
}

void sys_tell(void* stack_ptr)
{
  int fd = *((unsigned*)stack_ptr);

  struct file* f = get_file(fd);
  if (f == NULL) {
    thread_exit_with_status(-1);
  } else {
    file_tell(f);
  }
}

void sys_close(void* stack_ptr)
{
  //Get the file descriptor
  unsigned fd = *((unsigned*)stack_ptr);
  if(fd > 1 && fd < MAX_PROCESS_FILES)
  {
    struct file* file_to_close = get_file(fd);

    //The file was already closed (or never opened)
    if(file_to_close != NULL)
    {
      file_close(file_to_close);

      //Reset the pointer to the file (file_close takes care of freeing the memory)
      thread_current()->open_files[fd] = NULL;
    }
  }
}

void sys_exit(void* stack_ptr)
{
  int exit_status = *((int*) stack_ptr);
  incr_stack_ptr_with_chk(&stack_ptr, sizeof(int*));

  thread_exit_with_status(exit_status);
}

void sys_exec(struct intr_frame* f, void* stack_ptr)
{
  char* parameters = *((char**) stack_ptr);
  incr_stack_ptr_with_chk(&stack_ptr, sizeof(char**));

  check_valid_user_string(parameters);

  tid_t tid = process_execute(parameters);

  f->eax = (unsigned)tid;
}

void sys_wait(struct intr_frame* f, void* stack_ptr)
{
  int tid = *((int*) stack_ptr);
  incr_stack_ptr_with_chk(&stack_ptr, sizeof(int*));

  int status = process_wait(tid);

  f->eax = (unsigned)status;
}
