#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple device driver");
MODULE_AUTHOR("Joakim Lönnegren");

static const char *eudyptula_id = "a005a48651a3\n\0";
static char k_input[256] = {0};

ssize_t simple_write(struct file *f, const char *input, size_t count,
		loff_t *position)
{
	ssize_t ret = simple_write_to_buffer(k_input, sizeof(k_input),
			position, input, count);
	if (ret < 0)
		return ret;

	if (strncmp(eudyptula_id, k_input, strlen(eudyptula_id)))
		return -EINVAL;

	return ret;
}

ssize_t simple_read(struct file *f, char *user_buffer, size_t count,
		loff_t *position)
{
	return simple_read_from_buffer(user_buffer,
			count, position, eudyptula_id, strlen(eudyptula_id));
}

static const struct file_operations simple_fops = {
	.owner = THIS_MODULE,
	.write = simple_write,
	.read = simple_read,
};

static struct miscdevice simple_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &simple_fops,
};

static int simple_init(void)
{
	return misc_register(&simple_device);
}

static void simple_exit(void)
{
	misc_deregister(&simple_device);
}

module_init(simple_init);
module_exit(simple_exit);
