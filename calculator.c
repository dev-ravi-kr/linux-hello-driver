/*
    a program that runs in the user space and tests the ioctl calculator device by accessing the device file in /dev/ folder.


*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "cdev_calc_def.h"

#define A 9
#define B 8



int main(){
  int fd;
  struct cdev_calc_ calc;
  
  fd = open("/dev/calc_dev", O_RDWR); 
  if(fd < 0) {
    perror("open");
    return 1;
  }
  calc.num1 = A; calc.num2 = B; calc.num3 = 0;
  if(ioctl(fd, CALC_ADD, &calc)) perror("add");
  else printf("%d + %d = %d\n",A,B,calc.num3);

  calc.num1 = A; calc.num2 = B; calc.num3 = 0;
  if(ioctl(fd, CALC_SUB, &calc)) perror("sub");
  else printf("%d - %d = %d\n",A,B,calc.num3);

  calc.num1 = A; calc.num2 = B; calc.num3 = 0;
  if(ioctl(fd, CALC_MUL, &calc)) perror("mul");
  else printf("%d * %d = %d\n",A,B,calc.num3);

  calc.num1 = A; calc.num2 = B; calc.num3 = 0;
  if(ioctl(fd, CALC_DIV, &calc)) perror("div");
  else printf("%d / %d = %d\n",A,B,calc.num3);

  close(fd);


  return 0;
}
