#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int hello_init(void)
{
	pr_debug("Hello, world!\n");
	return 0;
}

static void hello_exit(void)
{
	pr_debug("Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Exercising kernel building");
MODULE_AUTHOR("Joakim Lönnegren");
