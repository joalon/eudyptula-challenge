#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple device driver");
MODULE_AUTHOR("Joakim Lönnegren");

struct miscdevice simple_device;
static char var[] = "ede3f266a0f6\n\0";

int simple_open(struct inode *i, struct file *f) {
	pr_debug("simple_driver: open()\n");
	return 0;
}

ssize_t simple_write(struct file *f, const char *input, size_t size, loff_t *flags) {
	pr_debug("simple_driver: write()\n");
	return 0;
}

ssize_t simple_read(struct file *f, char *user_buffer, size_t count, loff_t *position) {
	pr_debug("simple_driver: read()\n");

//	if (*position >= sizeof(var)){
//		pr_debug("simple_driver: tried to read further than there is data!\n");
//		return 0;
//	}

	if (*position + count > sizeof(var))
		count = sizeof(var) - *position;
	*position += count;

	if (copy_to_user(user_buffer, var, count) != 0)
		return -EFAULT;

	return count;
}

int simple_close(struct inode *i, struct file *f) {
	pr_debug("simple_driver: close()");
	return 0;
}

static struct file_operations simple_fops = {
	.owner = THIS_MODULE,
	.open = simple_open,
	.write = simple_write,
	.read = simple_read,
	.release = simple_close,
};

static int simple_init(void)
{
	int ret;
	pr_debug("simple_driver: starting init\n");

	simple_device.minor = MISC_DYNAMIC_MINOR;
	simple_device.name = "eudyptula";
	simple_device.fops = &simple_fops;

	ret = misc_register(&simple_device);
	if (ret)
		return ret;

	pr_debug("simple_driver: got minor %d\n", simple_device.minor);
	pr_debug("simple_driver: init done\n");
	return 0;
}

static void simple_exit(void)
{
	misc_deregister(&simple_device);
	pr_debug("simple_driver: Goodbye, world!\n");
}

module_init(simple_init);
module_exit(simple_exit);

