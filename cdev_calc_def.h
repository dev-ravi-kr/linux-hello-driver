/*
    a header file that defines the magic code, the data structure to be used and the operations for the calculator.

*/


#ifndef CHAR_CALC_IOCTL_H
#define CHAR_CALC_IOCTL_H


#include <linux/ioctl.h>

#define CALC_MAGIC 'k'

struct cdev_calc_ {int num1; int num2; int num3;};

#define CALC_ADD _IOWR(CALC_MAGIC, 0, struct cdev_calc_)
#define CALC_SUB _IOWR(CALC_MAGIC, 1, struct cdev_calc_)
#define CALC_MUL _IOWR(CALC_MAGIC, 2, struct cdev_calc_)
#define CALC_DIV _IOWR(CALC_MAGIC, 3, struct cdev_calc_)





#endif

