#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple device driver");
MODULE_AUTHOR("Joakim Lönnegren");

struct miscdevice simple_device;

int simple_open(struct inode *i, struct file *f) {
	pr_debug("simple_driver: open()\n");
	return 0;
}

ssize_t simple_write(struct file *f, const char *input, size_t size, loff_t *flags) {
	return 0;
}

//ssize_t simple_read(struct file *f, const char *c, size_t size, loff_t *flags) {
//
//}

static struct file_operations simple_fops = {
	.open = simple_open,
	.owner = THIS_MODULE,
	.write = simple_write,
//	.read = simple_read,
};

static int simple_init(void)
{
	int ret;

	simple_device.minor = MISC_DYNAMIC_MINOR;
	simple_device.name = "simple";
	simple_device.fops = &simple_fops;

	ret = misc_register(&simple_device);
	if (ret)
		return ret;

	pr_debug("simple_driver: got minor %d\n", simple_device.minor);
	pr_debug("simple_driver: Hello, world!\n");
	return 0;
}

static void simple_exit(void)
{
	misc_deregister(&simple_device);
	pr_debug("simple_driver: Goodbye, world!\n");
}

module_init(simple_init);
module_exit(simple_exit);

