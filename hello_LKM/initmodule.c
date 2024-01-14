#include <linux/module.h>
#include <linux/init.h>

/*Meta Information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ADITYA BHUSHAN GNU/Linux");
MODULE_DESCRIPTION("Hello World LKM");

/* This function is called when the module is loaded in the kernel*/

static int __init ModuleInit(void){
    printk("Hello Kernel !\n");
    return 0;
}

/* This function is called when the module is exiting the kernel*/
static void __exit ModuleExit(void){
    printk("Bye Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
