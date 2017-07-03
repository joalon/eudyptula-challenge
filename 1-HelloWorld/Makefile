COMPILE_KERNEL?=/usr/src/kernels/$(shell uname -r)/

obj-m += simple_module.o

all:
	make -C $(COMPILE_KERNEL) M=$(PWD) modules

clean:
	make -C $(COMPILE_KERNEL) M=$(PWD) clean
