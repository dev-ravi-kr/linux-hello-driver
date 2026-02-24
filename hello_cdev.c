/*
    a simple character device driver to test read and write operations on the driver.
    it registers itself with a major number provided by the kernel.
    if a device file exists in the /dev/ folder with the same major number,
    the read and write features can be invoked with cat and echo "input" > /dev/<device_name> respectively.

*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/uaccess.h>


#define DEVICE_NAME "HELLO_CDEV"
#define BUFFER_SIZE 1024


static int major_number;
static char device_buffer[BUFFER_SIZE];
static int buffer_offset = 0;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {

  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release,
};

static int device_open(struct inode *dnode, struct file *dfile){
  printk(KERN_INFO "DEVICE OPENED\n");
  return 0;
}

static int device_release(struct inode *dnode, struct file *dfile){
  printk(KERN_INFO "DEVICE CLOSED\n");
  return 0;
}
static ssize_t device_read(struct file *dfilep, char *buffer, size_t length, loff_t *offset){
  if (*offset >= buffer_offset) return 0;
  if (*offset + length > buffer_offset) length = buffer_offset - *offset;
  if (copy_to_user(buffer, device_buffer + *offset, length)) return -EFAULT;
  *offset += length;
  return length;
}
static ssize_t device_write(struct file *dfilep, const char *buffer, size_t length, loff_t *offset){
  if(buffer_offset + length >= BUFFER_SIZE) return -ENOMEM;
  if(copy_from_user(device_buffer + buffer_offset, buffer, length )) return -EFAULT;
  buffer_offset += length;
  return length;
}

static int __init init_cdev(void){
  major_number = register_chrdev(0, DEVICE_NAME, &fops);
  if(major_number < 0) {
    printk(KERN_ALERT "failed to register character device");
    return major_number;
  }
  printk(KERN_INFO "Registered character device with major number %d\n", major_number);
  return 0;
}


static void  __exit exit_cdev(void){
  unregister_chrdev(major_number, DEVICE_NAME);
  printk(KERN_INFO "Unregistered character device\n");
}

module_init(init_cdev);
module_exit(exit_cdev);

MODULE_AUTHOR("Ravi Kumar");
MODULE_DESCRIPTION("a basic character device driver");
MODULE_LICENSE("GPL");
