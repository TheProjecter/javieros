#include <javieros/tty.h>
#include <javieros/kernel.h>
#include <javieros/types.h>
#include <javieros/stdarg.h>
#include <asm/system.h>
#include <asm/string.h>
#include <asm/page.h>

#define END_OF_KMESS -1

volatile int console_pos_x = 0;
volatile int console_pos_y = 0;

