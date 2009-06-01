#ifndef __KERNEL_H
#define __KERNEL_H

#define DEBUG	do{printk("(%s:%s:%d) ", __FILE__, __FUNCTION__, __LINE__);}while(0);
#define DIE	do{DEBUG __asm__ __volatile__("hlt\n");}while(1);

#define DELAY	do{ \
		    int i,j,k; \
		    for(i = 0; i< 1000; i++) \
			for(j = 0; j< 1000; j++) \
				k = j; \
		}while(0);


#define INT_MAX	((int)(~0U>>1))
#define INT_MIN	(-INT_MAX - 1)
#define UINT_MAX	(~0U)
#define LONG_MAX	((long)(~0UL>>1))
#define LONG_MIN	(-LONG_MAX - 1)
#define ULONG_MAX	(~0UL)

/* timer interval is 0.1ms */
#define HZ	100


void printk(const char * fmt, ...);
void set_screen_pos_x(int pos_x);
void set_screen_pos_y(int pos_y);
void set_screen_pos_xy(int pos_x, int pos_y);
void clear_screen(void);

#endif
