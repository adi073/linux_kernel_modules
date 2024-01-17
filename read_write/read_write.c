#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/*Meta Information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ADITYA BHUSHAN GNU/Linux");
MODULE_DESCRIPTION("Registers a device number and implements some callback functions");

/* Buffer for data */
static char buffer[255];
static int buffer_pointer;

/*Variables for device and device class*/
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "dummydriver"
#define DRIVER_CLASS "MyModuleClass"

/* Read data out of the buffer */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs){
	int to_copy, not_copied, delta;

	/* Get amount of data to copy */
	to_copy = min(count, buffer_pointer);

	/* Copy data to user*/
	not_copied = copy_to_user(user_buffer, buffer, to_copy);

	/* Calculate data */
	delta = to_copy - not_copied;

	return  delta;
}


/* Write data to the buffer */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs){
	int to_copy, not_copied, delta;

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(buffer));

	/* Copy data from user*/
	not_copied = copy_from_user(buffer, user_buffer, to_copy);
	buffer_pointer = to_copy;

	/* Calculate data */
	delta = to_copy - not_copied;

	return  delta;
}

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
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};


/* This function is called when the module is loaded in the kernel*/

/*static int __init ModuleInit(void){
    int retval;
	printk("Hello Kernel!\n");
	//register device num .
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
*/
static int __init ModuleInit(void){
    int retval;
	printk("Hello Kernel!\n");
	
	// Allocate device number
	if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0){
		printk("Device nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write = Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);
	/*Create device class*/
	if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL){
		printk("Device class can not be created!\n");
		goto ClassError;
	}
	/* create device file  */
	if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL){
		printk("Can not crate device file!\n");
		goto FileError;
	}

	/* Initialize device File */
	cdev_init(&my_device, &fops);
	if (cdev_add(&my_device, my_device_nr, 1) == -1){
		printk("Registering of device to kernel failed!\n");
		goto AddError;
	}
	return 0;
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev(my_device_nr, DRIVER_NAME);
	return -1;
}

/* This function is called when the module is exiting the kernel*/
static void __exit ModuleExit(void){
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev(my_device_nr, "my dev num");
    printk("Bye Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
