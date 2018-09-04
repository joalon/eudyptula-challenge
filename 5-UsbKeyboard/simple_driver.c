#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple device driver");
MODULE_AUTHOR("Joakim Lönnegren");

static struct usb_device_id simple_table [] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT, 
			USB_INTERFACE_PROTOCOL_MOUSE) },
	{}
};
MODULE_DEVICE_TABLE(usb, simple_table);

static int simple_init(void)
{
	pr_debug("simple_driver: Hello, world!\n");
	return 0;
}

static void simple_exit(void)
{
	pr_debug("simple_driver: Goodbye, world!\n");
}

module_init(simple_init);
module_exit(simple_exit);

