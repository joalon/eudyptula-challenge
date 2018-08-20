#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple device driver");
MODULE_AUTHOR("Joakim Lönnegren");

<<<<<<< Updated upstream
static struct usb_device_id simple_table [] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT, 
			USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{}
};
MODULE_DEVICE_TABLE(usb, simple_table);
=======
//static struct usb_device_id simple_table [] = {
//	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
//	{}
//};
//MODULE_DEVICE_TABLE(usb, simple_table);
>>>>>>> Stashed changes

static int simple_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	pr_debug("simple_driver: USB probe called\n");
	return 0;
}

static void simple_disconnect(struct usb_interface *interface)
{
	pr_debug("simple_driver: USB disconnect called\n");
}

static struct usb_driver simple_driver = {
	.name		= "Simple driver",
//	.id_table	= simple_table,
	.probe 		= simple_probe,
	.disconnect	= simple_disconnect
};

static int simple_init(void)
{
	int result;

	result = usb_register(&simple_driver);
	if (result < 0) {
		pr_debug("simple_driver: usb_register failed, error number %d\n", result);
		return -1;
	}

	pr_debug("simple_driver: Hello, world!\n");
	return 0;
}

static void simple_exit(void)
{
	usb_deregister(&simple_driver);

	pr_debug("simple_driver: Goodbye, world!\n");
}

module_init(simple_init);
module_exit(simple_exit);

