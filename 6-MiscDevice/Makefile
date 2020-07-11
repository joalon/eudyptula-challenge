COMPILE_KERNEL?=/usr/lib/modules/$(shell uname -r)/build
CFLAGS_simple_driver.o := -DDEBUG
obj-m += simple_driver.o

all:
	make -C $(COMPILE_KERNEL) M=$(PWD) modules -I string

clean:
	make -C $(COMPILE_KERNEL) M=$(PWD) clean

test: all
	sudo rmmod simple_driver || true
	sudo insmod simple_driver.ko

