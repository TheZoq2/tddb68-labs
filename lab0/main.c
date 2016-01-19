#include <stdio.h>

int main(int argc, char ** argv)
{
  char str[] = "sihtgubed"; //Create weird array
  char *stri = &str[8]; //stri points to the address of the 8th element
  char buf[9]; //Array of char ptrs length 9
  char *bufi, *bufend; //Pointers to pointers for incrementation
  bufi = buf; //Set buffi to beginning
  bufend = &buf[9]; //Set buffend outside buff

  //Reverse str
  while (bufi != bufend){
    *bufi = *stri; 
    bufi++;
    stri--;
  }

  
  //Convert to upper  case
  while (bufi != buf){
    (*bufi) -= 32;
    bufi--;
  }

  //Print result
  while (bufi != bufend){
    printf("%c", *bufi);
    bufi++;
  }
}
