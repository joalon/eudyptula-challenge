#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple device driver");
MODULE_AUTHOR("Joakim Lönnegren");

struct miscdevice simple_device;
static char eudyptula_id[] = "a005a48651a3\n\0";
static char k_input[256] = {0};
static int ret = 0;

ssize_t simple_write(struct file *f, const char *input, size_t count, loff_t *position)
{
	ret = copy_from_user(k_input, input, count);
	if (ret < 0) {
		pr_debug("simple_driver: got error %d from copy_from_user\n", ret);
		return -EFAULT;
	}

	if (strncmp(eudyptula_id, k_input, count) != 0) {
		pr_debug("simple_driver: write called, did not match my eudyptula id\n", k_input);
		return -EINVAL;
	}

	pr_debug("simple_driver: got my eudyptula id!\n", k_input);
	return count;
}

ssize_t simple_read(struct file *f, char *user_buffer, size_t count, loff_t *position)
{
	if (*position != 0) {
		pr_debug("simple_driver: read called a second time, returning 0\n");
		return 0;
	}

	ret = copy_to_user(user_buffer, eudyptula_id, sizeof(eudyptula_id));
	if (ret < 0) {
		pr_debug("simple_driver: got error %d from copy_to_user\n", ret);
		return -EFAULT;
	}

	pr_debug("simple_driver: read called first time, writing id to user\n");
	*position += sizeof(eudyptula_id);
	return sizeof(eudyptula_id);
}

static struct file_operations simple_fops = {
	.owner = THIS_MODULE,
	.write = simple_write,
	.read = simple_read,
};

static int simple_init(void)
{
	simple_device.minor = MISC_DYNAMIC_MINOR;
	simple_device.name = "eudyptula";
	simple_device.fops = &simple_fops;

	ret = misc_register(&simple_device);
	if (ret) {
		pr_debug("simple_driver: error %d when registering misc device\n", ret);
		return ret;
	}

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
