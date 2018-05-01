#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define DEVICE_NAME "schar"
#define CLASS_NAME "s"

static int 	majorNumber;
static char 	message[256] = {0};
static short	size_of_message;
static int	number_of_opens = 0;
static struct 	class* scharClass = NULL;
static struct 	device* scharDevice = NULL;

static int	dev_open(struct inode *, struct file *);
static int	dev_release(struct inode *, struct file *);
static ssize_t	dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t	dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};

static int __init schar_init(void)
{
	pr_debug("SChar: Initializing\n");

	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber < 0) {
		pr_alert("SChar: Failed to register a major number\n");
		return majorNumber;
	}
	pr_debug("SChar: Registered major number %d\n", majorNumber);

	scharClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(scharClass)) {
		unregister_chrdev(majorNumber, DEVICE_NAME);
		pr_alert("SChar: Failed to register device class\n");
		return PTR_ERR(scharClass);
	}
	pr_debug("SChar: Registered device class\n");

	scharDevice = device_create(scharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
	if (IS_ERR(scharDevice)) {
		class_destroy(scharClass);
		unregister_chrdev(majorNumber, DEVICE_NAME);
		pr_alert("SChar: Failed to create device");
		return PTR_ERR(scharDevice);
	}
	pr_debug("SChar: device class created");
	return 0;
}

static void __exit schar_exit(void)
{
	device_destroy(scharClass, MKDEV(majorNumber, 0));
	class_unregister(scharClass);
	class_destroy(scharClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);
	pr_debug("SChar: Exiting\n");
}

static int dev_open(struct inode *inodep, struct file *filep)
{
	pr_debug("SChar: dev_open called\n");
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) 
{
	pr_debug("SChar: dev_release called\n");
	return 0;

}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	pr_debug("SChar: dev_read called\n");
	return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	pr_debug("SChar: dev_write called\n");
	pr_debug("SChar: write got %s of length %d\n", buffer, len);
	return len;
}

module_init(schar_init);
module_exit(schar_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Exercising kernel building");
MODULE_AUTHOR("Joakim Lönnegren");
