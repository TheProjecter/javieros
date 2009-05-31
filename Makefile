ARCH = i386

CC = gcc -g
LDS = javieros.lds

C_SOURCES = $(wildcard init/*.c)
C_SOURCES += $(wildcard kernel/*.c)
C_SOURCES += $(wildcard mm/*.c)
C_SOURCES += $(wildcard lib/*.c)
C_SOURCES += $(wildcard drivers/*.c)

ASM_SOURCES = $(wildcard boot/*.S)
ASM_SOURCES += $(wildcard kernel/*.S)
ASM_SOURCES += $(wildcard lib/*.S)

INCLUDES = -I./include/ -I./
GENERAL_FLAGS = -nostartfiles -nostdlib -nodefaultlibs -nostdinc -fno-builtin
CFLAGS = $(INCLUDES) $(GENERAL_FLAGS) -O2 -Wall -D__KERNEL__
ASMFLAGS = $(INCLUDES) $(GENERAL_FLAGS) -O2 -Wall -D__KERNEL__ -D__ASM__

all: clean
	$(CC) $(ASMFLAGS) -c $(ASM_SOURCES)
	$(CC) $(CFLAGS) -c $(C_SOURCES)
	ld *.o -T $(LDS) -o os.bin
	@rm -f *.o
#	@nm os.bin > System.map
#	@readelf -a os.bin > os_elf.info

clean:
	rm -f *.o os.bin

install:
	mount ./floppy.img /mnt/jos -o loop
	rm -f /mnt/jos/boot/os.bin
	install ./os.bin /mnt/jos/boot
	umount /mnt/jos
	sync

fsck:
	/sbin/fsck.ext2 ./floppy.img
