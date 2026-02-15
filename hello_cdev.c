#include<linux/init.h>
#include<linux/module.h>

static int __init init_cdev(void){
  printk("cdev:: hello from cdev\n");
  return 0;
}


static void  __exit exit_cdev(void){
  printk("cdev:: bye from cdev\n");
}

module_init(init_cdev);
module_exit(exit_cdev);

MODULE_AUTHOR("Ravi Kumar");
MODULE_DESCRIPTION("a basic character device driver");
MODULE_LICENSE("GPL");
