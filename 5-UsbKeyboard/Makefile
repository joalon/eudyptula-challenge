COMPILE_KERNEL?=/usr/src/linux-headers-$(shell uname -r)/

CFLAGS_simple_module.o := -DDEBUG
obj-m += simple_module.o

all:
	make -C $(COMPILE_KERNEL) M=$(PWD) modules

clean:
	make -C $(COMPILE_KERNEL) M=$(PWD) clean
