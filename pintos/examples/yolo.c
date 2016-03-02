#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <syscall.h>

int main()
{
  unsigned tid = exec("swag");
  printf("Started process %i\n", tid);
}
