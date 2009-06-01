#ifndef __SYSTEM_H
#define __SYSTEM_H

#define __save_flags(x) \
	__asm__ __volatile__("pushfl; popl %0" \
			:"=g"(x):)

#define __restore_flags(x) \
	__asm__ __volatile__("pushl %0; popfl" \
			::"g"(x):"memory", "cc")

#define __cli() __asm__ __volatile__("cli":::"memory")
#define __sti() __asm__ __volatile__("sti":::"memory")

#define safe_halt() __asm__ __volatile__("sti; hlt":::"memory")

#define __save_and_cli(x) do{__save_flags(x); __cli();}while(0);
#define __save_and_sti(x) do{__save_flags(x); __sti();}while(0);

#define local_irq_save(x) __save_and_cli(x)
#define local_irq_set(x) __save_and_sti(x)

#define local_irq_restore(x) __restore_flags(x)
#define local_irq_disable(x) __cli()
#define local_irq_enable(x) __sti()

#define do_div(n, base) ({ \
		unsigned long __upper, __low, __high, __mod; \
		asm("":"=a" (__low), "=d" (__high):"A" (n)); \
		__upper = __high; \
		if (__high) { \
			__upper = __high % (base); \
			__high = __high / (base); \
		} \
		asm("divl %2":"=a" (__low), "=d" (__mod):"rm" (base), "0" (__low), "1" (__upper)); \
		asm("":"=A" (n):"a" (__low),"d" (__high)); \
		__mod; \
})

#endif
