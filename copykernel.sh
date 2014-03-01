sudo mount -o loop dev_kernel_grub.img /mnt/os611
sudo cp kernel.bin /mnt/os611
make clean
sudo umount /mnt/os611
