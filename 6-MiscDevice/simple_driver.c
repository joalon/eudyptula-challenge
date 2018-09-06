#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple device driver");
MODULE_AUTHOR("Joakim Lönnegren");

struct miscdevice *simple_device;

static int simple_init(void)
{

	simple_device->minor = MISC_DYNAMIC_MINOR;
	simple_device->name = "simple";

	int ret = misc_register(simple_device);
	if (ret)
		return ret;

	pr_debug("simple_driver: got minor %d\n", simple_device.minor);
	pr_debug("simple_driver: Hello, world!\n");
	return 0;
}

static void simple_exit(void)
{
	misc_unregister(simple_device);
	pr_debug("simple_driver: Goodbye, world!\n");
}

module_init(simple_init);
module_exit(simple_exit);

