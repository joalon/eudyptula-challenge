#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_init(void)
{
	printk(KERN_DEBUG "Hello, world!\n");
	return 0;


static void __exit hello_cleanup(void) 
{
	printk(KERN_DEBUG "Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_cleanup);

MODULE_LICENSE("WTFPL");
MODULE_DESCRIPTION("Excersing kernel building");
MODULE_AUTHOR("Joakim Lönnegren");
