Lab 1
=====

#0 a)
Letting the OS take over and run some code instead of  the user program

#b)
Preventing user programs from breaking things they don't own

#1 a)
You push the ID of the syscall to the stack. 
# b)
On the stack

#3 a)
User: Somewhere in user memory located between 0 to PHYS_BASE, kernel: somewhere in kernel memory after PHYS_BASE

The location of a userprocesses memory is stored in the page table

# b)
Using the esp* (or any other pointer to user memory)

# c)
The kernel and user process(es) are different processes and need separate stacks

#4 a)
The user processes memory

# b)
Using the esp* pointer


#7
2 programs shouldn't be able to write from the same file at the same time (or read when another is writing)

#8
To create file descriptors used later when reading/writing

#9
