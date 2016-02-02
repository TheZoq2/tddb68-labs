#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

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

  int syscall_id = *((int*)f->esp);

  switch(syscall_id) 
  {
    case  SYS_HALT:
    {
      printf("Running SYS_HALT");
      power_off();
      break;
    }
    case SYS_EXIT:
    {
      
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
  
  thread_exit ();
}
