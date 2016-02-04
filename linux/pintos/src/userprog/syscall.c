#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  printf ("system call!\n");

  int syscall_id = *((int*)f->esp); //Stack points to the address after the stack 

  void* stack_ptr = f->esp;
  
  printf("sycall id: %i\n", syscall_id);

  stack_ptr += sizeof(void*);

  switch(syscall_id) 
  {
    case  SYS_HALT:
    {
      power_off();
      break;
    }
    case SYS_EXIT:
    {
      thread_exit();
      break;
    }
    case SYS_EXEC:
    {
      break;
    }
    case SYS_WAIT:
    {
      break;
    }
    case SYS_CREATE:
    {
      //Fetch the filename and size
      char* filename = *(char**)stack_ptr;
      stack_ptr += sizeof(char*);
      unsigned int size = *((unsigned int*)stack_ptr);

      bool result = filesys_create(filename, size);

      f->eax = result;
      //f->esp += sizeof(bool);
      break;
    }
    case SYS_REMOVE:
    {
    
      break;
    }
    case SYS_OPEN:
    {
      break;
    }
    case SYS_FILESIZE:
    {
      break;
    }
    case SYS_READ:
    {
      break;
    }
    case SYS_WRITE:
    {
      printf("Write syscall\n");
      break;
    }
    case SYS_SEEK:
    {
      break;
    }
    case SYS_TELL:
    {
      break;
    }
    case SYS_CLOSE:
    {
      break;
    }
    default:
      break;
  }
 
  printf("Done syscall\n");
  //thread_exit ();
}
