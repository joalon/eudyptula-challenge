#!/bin/bash

MY_KERNEL_DIR=~/myKernel

echo "Creating initial directory $MY_KERNEL_DIR"
mkdir $MY_KERNEL_DIR
cd $MY_KERNEL_DIR

echo "Cloning source"
git clone https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git

echo "Entering linux source folder"
cd linux

echo "Copy config file"
#cp /boot/config-`uname -r`* .config
make defconfig
sed -i '/CONFIG_LOCALVERSION_AUTO/ c\CONFIG_LOCALVERSION_AUTO=y' .config

echo "Making and installing stuff"
make -j4 && make -j4 modules && make -j4 modules_install

echo "Finishing touches"
make -j4 install

