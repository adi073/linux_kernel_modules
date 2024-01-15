#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

/*Meta Information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ADITYA BHUSHAN GNU/Linux");
MODULE_DESCRIPTION("Registers a device number and implements some callback functions");

/* This function is called when the device file is opened */
static int driver_open(struct inode *device_file, struct file *instance){
	printk("device_num - open was called\n");
	return 0;
}

/* This function is called when device file is closed */
static int driver_close(struct inode *device_file, struct file *instance){
	printk("device_num - close was called\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close
};

#define MYMAJOR 90

/* This function is called when the module is loaded in the kernel*/

static int __init ModuleInit(void){
    int retval;
	printk("Hello Kernel!\n");
	/*register device num . */
	retval = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
	if (retval == 0){
		printk("dev_num - register device number Major : %d, Minor : %d\n", MYMAJOR, 0);
	}
	else if(retval > 0){
		printk("dev num - registered device number Major : %d, Minor : %d\n", retval>>20, retval&0xfffff);
	}
	else{
		printk("Could not register device num");
		return -1;
	}
	return 0;
}

/* This function is called when the module is exiting the kernel*/
static void __exit ModuleExit(void){
	unregister_chrdev(MYMAJOR, "my dev num");
    printk("Bye Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
