#include "kernel.h"

void task_init(void);
void interrupt_init(void);
void hard_init(void);
void n_task(void);
void timer_init(void);
void serial_init(void);

void main(void)
{
	interrupt_init();
	hard_init();
	task_init();

	en_all_int();
	n_task();
}

void hard_init(void)
{
	timer_init();
	serial_init();
}