#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include "cdev_calc_def.h"


#define DEVICE_NAME "CDEV_CALC"
#define BUFFER_SIZE 1024


static int major_number;
static char device_buffer[BUFFER_SIZE];
static int buffer_offset = 0;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);
static long device_ioctl(struct file *, unsigned int, unsigned long);

static struct file_operations fops = {

  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release,
  .unlocked_ioctl = device_ioctl,
};

static int device_open(struct inode *dnode, struct file *dfile){
  printk(KERN_INFO "DEVICE OPENED\n");
  return 0;
}

static int device_release(struct inode *dnode, struct file *dfile){
  printk(KERN_INFO "DEVICE CLOSED\n");
  return 0;
}
static ssize_t device_read(struct file *dfilep, char __user *buffer, size_t length, loff_t *offset){
  if (*offset >= buffer_offset) return 0;
  if (*offset + length > buffer_offset) length = buffer_offset - *offset;
  if (copy_to_user(buffer, device_buffer + *offset, length)) return -EFAULT;
  *offset += length;
  return length;
}
static ssize_t device_write(struct file *dfilep, const char __user *buffer, size_t length, loff_t *offset){
  if(buffer_offset + length >= BUFFER_SIZE) return -ENOMEM;
  if(copy_from_user(device_buffer + buffer_offset, buffer, length )) return -EFAULT;
  buffer_offset += length;
  return length;
}
static long device_ioctl(struct file *dfilep, unsigned int cmd, unsigned long arg){
  struct cdev_calc_ calc;
  if(copy_from_user(&calc, (void __user *)arg, sizeof(calc))) return -EFAULT;

  switch(cmd){
    case CALC_ADD:
      calc.num3 = calc.num1 + calc.num2;
      break;
    case CALC_SUB:
      calc.num3 = calc.num1 - calc.num2;
      break;
    case CALC_MUL:
      calc.num3 = calc.num1 * calc.num2;
      break;
    case CALC_DIV:
      if (calc.num2 == 0) return -EINVAL;
      calc.num3 = calc.num1 / calc.num2;
      break;
    default:
      return -ENOTTY;
  }
  if (copy_to_user((void __user *)arg, &calc, sizeof(calc))) return -EFAULT;
  return 0;
}

static int __init init_cdev_calc(void){
  major_number = register_chrdev(0, DEVICE_NAME, &fops);
  if(major_number < 0) {
    printk(KERN_ALERT "failed to register character device");
    return major_number;
  }
  printk(KERN_INFO "Registered character device with major number %d\n", major_number);
  return 0;
}


static void  __exit exit_cdev_calc(void){
  unregister_chrdev(major_number, DEVICE_NAME);
  printk(KERN_INFO "Unregistered character device\n");
}

module_init(init_cdev_calc);
module_exit(exit_cdev_calc);

MODULE_AUTHOR("Ravi Kumar");
MODULE_DESCRIPTION("a basic character device driver which calculates using IOCTL");
MODULE_LICENSE("GPL");
