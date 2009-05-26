ARCH = i386

CC = gcc
LDS = os.lds

C_SOURCES = kernel.c
ASM_SOURCES = boot.S

INCLUDES = -I./include/ -I./
GENERAL_FLAGS = -nostartfiles -nostdlib -nodefaultlibs -nostdinc -fno-builtin
CFLAGS = $(INCLUDES) $(GENERAL_FLAGS) -O2 -Wall -D__KERNEL__
ASMFLAGS = $(INCLUDES) $(GENERAL_FLAGS) -O2 -Wall -D__KERNEL__ -D__ASM__

all: clean
	$(CC) $(CFLAGS) -c $(C_SOURCES)
	$(CC) $(ASMFLAGS) -c $(ASM_SOURCES)
	ld *.o -T $(LDS) -o os.bin
	@rm -f *.o
	@nm os.bin > System.map
	@readelf -a os.bin > os_elf.info

clean:
	rm -f *.o os.bin

install: all
	mount ./floppy.img /mnt/jos -o loop
	mv /mnt/jos/boot/os.bin /mnt/jos/boot/os.bin.bk
	install ./os.bin /mnt/jos/boot
	umount /mnt/jos
	sync

fsck:
	/sbin/fsck.ext2 ./floppy.img
