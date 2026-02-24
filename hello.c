/*
     a simple driver module that which logs hello world on the kernel when loaded and by world when unloaded.
     nothing more.
*/
#include<linux/init.h>
#include<linux/module.h>


static int __init hello_init(void){
  printk("hello module loaded: HELLO WORLD!\n");
  return 0;
}

static void __exit hello_exit(void){
  printk("hello module unloaded: BYE WORLD!\n");
}

module_init(hello_init);
module_exit(hello_exit);


MODULE_AUTHOR("Ravi Kumar");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("hello World driver");
