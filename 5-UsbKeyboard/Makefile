COMPILE_KERNEL?=/usr/lib/modules/$(shell uname -r)/build
CFLAGS_simple_driver.o := -DDEBUG
obj-m += simple_driver.o

all:
	make -C $(COMPILE_KERNEL) M=$(PWD) modules

clean:
	make -C $(COMPILE_KERNEL) M=$(PWD) clean

install: simple_driver.ko
	sudo mv /lib/modules/$(shell uname -r)/kernel/drivers/hid/usbhid/usbhid.ko.xz .
	sudo rmmod usbhid
	sudo cp simple_driver.ko /lib/modules/$(shell uname -r)/kernel/drivers/hid/usbhid/simple_driver.ko
	sudo sed -i 's/usbhid.ko.xz/simple_driver.ko/g' /lib/modules/$(shell uname -r)/modules.dep
	sudo depmod -a

uninstall: usbhid.ko.xz
	sudo rmmod simple_driver
	sudo cp usbhid.ko.xz /lib/modules/$(shell uname -r)/kernel/drivers/hid/usbhid/usbhid.ko.xz
	sudo sed -i 's/simple_driver.ko/usbhid.ko.xz/g' /lib/modules/$(shell uname -r)/modules.dep
	sudo depmod -a
	sudo rm usbhid.ko.xz

