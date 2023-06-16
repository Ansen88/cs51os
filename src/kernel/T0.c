#include "equ.h"
#include "config.h"
#include "kernel.h"

void re_i(void);
void wakeup(unsigned char e_id);
void sleep(void);
void f_task(void);
void ext_sleep(unsigned char pro_id);

static union
{
	unsigned char f; 
	struct 
	{
		unsigned char irq		:1;
		unsigned char sl		:1;
		unsigned char wa		:1;
		unsigned char ex		:1;
		unsigned char f_task	:1;
		unsigned char sleep		:1;
		unsigned char wakeup	:1;
		unsigned char ex_sl		:1;
	}b;	
}flage;

static unsigned char xdata irq_buf[EQU_NUM*2];
static unsigned char data irq_buf_pos;
static unsigned char data irq_buf_num;
static unsigned char xdata wakeup_buf;
static unsigned char xdata ex_buf;

extern unsigned char data f_para;

#define T0_RUN()	TH0=0xfe;\
					TL0=0x00;\
					TR0=1;

void system_call(void) interrupt 1 using 1
{
	TR0=0;

	if(flage.b.ex)
	{
		flage.b.ex_sl=1;
		ext_sleep(ex_buf);
		flage.b.ex=0;
		flage.b.ex_sl=0;
	}
	
	if(flage.b.sl)
	{	
		flage.b.sleep=1;
		sleep();
		flage.b.sl=0;
		flage.b.sleep=0;
	}
	
	if(flage.b.wa)
	{
		flage.b.wakeup=1;
		wakeup(wakeup_buf);
		flage.b.wa=0;
		flage.b.wakeup=0;
	}

	if(!flage.b.f_task)
	{	
		flage.b.f_task=1;
		re_i();
		while(irq_buf_num)
		{
			f_para=irq_buf[irq_buf_pos];
			irq_buf_pos++;
			irq_buf_num--;
			f_task();	
		}
		flage.b.irq=0;
		flage.b.f_task=0;
	}	
}

unsigned char sent_msg(unsigned char msg,unsigned char para)
{
	unsigned char result;

	dis_all_int();
	 switch(msg)
	 {
	 	case MSG_R_BUF:
			flage.b.ex=1;
			result=1;
			ex_buf=para;
			T0_RUN();
			break;
	 	case MSG_IRQ_REV:
			result=0xff-irq_buf_pos;
			if(irq_buf_num>result)
			{
				result=0;
				break;
			}
			irq_buf[irq_buf_pos+irq_buf_num]=para;
			irq_buf_num++;
			result=1;
			if(!flage.b.irq)
			{
				flage.b.irq=1;
				T0_RUN();
			}
			break;
		case MSG_SLEEP:
			flage.b.sl=1;
			result=1;
			T0_RUN();
			break;
		case MSG_WAKEUP:
			flage.b.wa=1;
			result=1;
			wakeup_buf=para;
			T0_RUN();
	 }
	 en_all_int();
	 return result;
}
//unsigned char idata _sp_buf[0xd0] _at_ 0xff-0xd0;

void sys_init(void)
{
	flage.f=0;
	irq_buf_pos=0;
	irq_buf_num=0;
//	_sp_buf[0]=1;
}