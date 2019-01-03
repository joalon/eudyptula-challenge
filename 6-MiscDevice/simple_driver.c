#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple device driver");
MODULE_AUTHOR("Joakim Lönnegren");

struct miscdevice simple_device;
static char var[] = "ede3f266a0f6\n\0";

ssize_t simple_write(struct file *f, const char *input, size_t count, loff_t *position) {
	if (strcmp(var, input) == 0) {
		pr_debug("simple_driver: got my eudyptula id!\n");
	} else {
		pr_debug("simple_driver: didn't get eudyptula id!\n");
		return -EINVAL;
	}

	*position += count;
	return count;
}

ssize_t simple_read(struct file *f, char *user_buffer, size_t count, loff_t *position) {
	if (*position >= sizeof(var)){
		pr_debug("simple_driver: tried to read further than there is data!\n");
		return 0;
	}

	if (*position + count > sizeof(var))
		count = sizeof(var) - *position;
	*position += count;

	if (copy_to_user(user_buffer, var, count) != 0)
		return -EFAULT;

	return count;
}

static struct file_operations simple_fops = {
	.owner = THIS_MODULE,
	.write = simple_write,
	.read = simple_read,
};

static int simple_init(void)
{
	int ret;

	simple_device.minor = MISC_DYNAMIC_MINOR;
	simple_device.name = "eudyptula";
	simple_device.fops = &simple_fops;

	ret = misc_register(&simple_device);
	if (ret)
		return ret;

	pr_debug("simple_driver: init done. Got minor %d\n", simple_device.minor);
	return 0;
}

static void simple_exit(void)
{
	misc_deregister(&simple_device);
	pr_debug("simple_driver: exiting\n");
}

module_init(simple_init);
module_exit(simple_exit);

