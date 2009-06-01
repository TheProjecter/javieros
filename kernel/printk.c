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

volatile int screen_char_attr = TTY_CHAR_ATTRIBUTE;

static void
screen_putc(int x, int y, int c)
{
	unsigned char * video_entry;
	video_entry = (unsigned char *)(VIDEO_VA + (x + y * TTY_COLUMNS_NR) * 2);
	*video_entry = c & 0xff;
	video_entry = (unsigned char *)(VIDEO_VA + (x + y * TTY_COLUMNS_NR) * 2) + 1;
	*video_entry = screen_char_attr & 0xff;
	return;
}

/* scroll up 1 line of screen */
static void
screen_scrollup(void)
{
	char * video_entry = (char *)VIDEO_VA;
	memcpy((char *) video_entry, 
		(char *)(video_entry + TTY_COLUMNS_NR * 2), 
		(TTY_LINES_NR - 1) * TTY_COLUMNS_NR * 2);
	for(video_entry = (char *)(VIDEO_VA + (TTY_LINES_NR - 1) * TTY_COLUMNS_NR * 2);
		video_entry < (char *)(VIDEO_VA + TTY_COLUMNS_NR * TTY_LINES_NR * 2);
		video_entry += 2)
	{
		*((char *)video_entry) = ' ';
		*((char *)(video_entry + 1)) = screen_char_attr & 0xff;
	}
}

//FIXME: now only display message on console.
static void
kputc(int c)
{
	int pad_nr = 0;
	if(c < 0)
		return;
	if(c == 0)
		c = '@';

	if(console_pos_x >= TTY_COLUMNS_NR)
	{
		console_pos_x = 0;
		console_pos_y += 1;		
	}
	if(console_pos_y >= TTY_LINES_NR)
	{
		screen_scrollup();
		console_pos_y -= 1;
	}
	switch(c)
	{
		case '\n':
			console_pos_x = 0;
			console_pos_y += 1;
			break;
		case '\t' :
			pad_nr = TTY_TABWIDTH - (console_pos_x % TTY_TABWIDTH);
			while(pad_nr --)
			{
				screen_putc(console_pos_x, console_pos_y, ' ');
				console_pos_x += 1;
			}
			break;
		default:
			screen_putc(console_pos_x, console_pos_y, c);
			console_pos_x += 1;
			break;
	}
}

void
set_screen_pos_x(int pos_x)
{
	int flags;
	local_irq_save(flags);
	console_pos_x = pos_x;
	local_irq_restore(flags);
}

void
set_screen_pos_y(int pos_y)
{	int flags;
	local_irq_save(flags);
	console_pos_y = pos_y;
	local_irq_restore(flags);
}

void
set_screen_pos_xy(int pos_x, int pos_y)
{
	int flags;
	local_irq_save(flags);
	set_screen_pos_x(pos_x);
	set_screen_pos_y(pos_y); 
	local_irq_restore(flags);
}

void
clear_screen(void)
{
	int flags;
	int x, y;

	local_irq_save(flags);
	set_screen_pos_x(0);
	set_screen_pos_y(0);
	for(x = 0; x < TTY_COLUMNS_NR; x++)
		for(y = 0; y < TTY_LINES_NR; y++)
		{
			screen_putc(x, y, '\0');
		}
	set_screen_pos_x(0);
	set_screen_pos_y(0);
	local_irq_restore(flags);
}

void
printk(const char *fmt, ...)		/* format to be printed */
{
	int c;						/* next character in fmt*/
	int d;
	unsigned long u;				/* hold number argument */
	int base;					/* base of number arg */
	int negative;					/* print minus sign */
	static char x2c[] = "0123456789abcdef";	/* nr conversion table */
	char ascii[8 * sizeof(long) / 3 + 2]; 	/* string for ascii number */
	char *s;					/* string to be printed */
	char s_buf[16];				/* tempory buffer for char *s */
	char ch;					/* character to be printed */
	va_list argp;					/* optional arguments */
	int flags;
	
	local_irq_save(flags);
	va_start(argp, fmt);
	while((c = *fmt++) != 0)
	{
		if(c == '%')
		{
			negative = 0;
			s = NULL;
			switch(c = *fmt++)
			{
			case 'd':
				d = va_arg(argp, signed int);
				if(d < 0) 
				{
					negative = 1;
					u = -d;
				} 
				else
				{
					u = d;
				}
				base = 10;
				break;
			case 'u':
				u = va_arg(argp, unsigned long);
				base = 10;
				break;
			case 'x':
				u = va_arg(argp, unsigned long);
				base = 0x10;
				break;
			case 's':
				s = va_arg(argp, char *);
				if(s == NULL)
					s = "(null)";
				break;
			case 'c':
				ch = va_arg(argp, char);
				if(ch == 0)
				{
					s = "(NUL)";
				}
				else
				{
					memset(s_buf, 0, sizeof(s_buf));
					s_buf[0] = ch;
					s = (char *)s_buf;
				}
				break;
			case '%':
				s = "%";
				break;
			/* unrecognized key, just echo back */
			default:
				memset(s_buf, 0, sizeof(s_buf));
				s_buf[0] = '%';
				s_buf[1] = (char)c;
				s = (char *)s_buf;
			}

			/* if s is NULL, so convert number to ascii */
			if(s == NULL)
			{
				s = ascii + sizeof(ascii)-1;
				*s = 0;
				do{
					*--s = x2c[(u % base)];
				}while((u = u / base) > 0);
			}
			
			if(negative)
				kputc('-');
			while(*s != 0)
				kputc(*s++);
		}
		else
		{
			kputc(c);
		}
	} /* while */
	kputc(END_OF_KMESS);
	va_end(argp);
	local_irq_restore(flags);
}
