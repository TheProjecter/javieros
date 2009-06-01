#include <javieros/tty.h>
#include <javieros/kernel.h>
#include <javieros/stdarg.h>
#include <asm/system.h>

volatile int console_pos_x = 0;
volatile int console_pos_y = 0;


void printk(const char *fmt, ...)
{
	int c;
}
