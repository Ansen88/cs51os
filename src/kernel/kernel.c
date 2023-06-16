#include "cdefine.h"
#include "config.h"
#include "kernel.h"
#include "graphic.h"

#define IDLE()		PCON |= 0x01

void sys_init(void);

static unsigned char data n_buf_num;
static unsigned char data n_buf_pos;
static unsigned char xdata n_buf[255];

static unsigned char data task_max;
static unsigned char data last_task;

unsigned char data f_para;
_cprs data cprs;

static struct
{
	unsigned char buf;
	unsigned char sp;	
}xdata keep_task[THREAD_NUM];

unsigned char judge(unsigned char x)
{
	if(x=='a')
		return 1;
	else return 0;
}

void n_task(void)
{
	do{
		if(n_buf_num)
		{			
			dis_all_int();
			n_buf_pos++;
			draw_line(0,0,5,5);//*********************//
			n_buf_num--;
			en_all_int();
		}
		else
		{
			IDLE();
		}	
	}while(1);
}

void f_task(void) using 2
{
	unsigned char data tmp;

	if(!judge(f_para))
	{
		F0=1;
		F1=0;
		tmp=n_buf_pos+n_buf_num;			 //***********if n_buf_num==255?
		n_buf[tmp]=f_para;
		n_buf_num++;
	}
	else
	{
		last_task=cprs.b.p_id;
		keep_task[last_task].buf=cprs.f;
		keep_task[last_task].sp=SP;
		cprs.f=0;
		cprs.b.p_id=++task_max;
		SP=SP_START+SP_SPACE*task_max;

		F1=1;
		F0=0;

		task_max--;
		cprs.f=keep_task[last_task].buf;
		SP=keep_task[last_task].sp;
	}
}

void sleep(void)
{
	if(!cprs.b.p_id)
		IDLE();
	else
	{
		keep_task[cprs.b.p_id].buf=cprs.f;
		keep_task[cprs.b.p_id].sp=SP;
		cprs.b.p_id--;
		cprs.f=keep_task[cprs.b.p_id].buf;
		SP=keep_task[cprs.b.p_id].sp;
	}	
}

void wakeup(unsigned char e_id)
{
	unsigned char data i;
	_cprs c;

	for(i=task_max;i>0;i--)
	{
		c.f=keep_task[i].buf;
		if(c.b.bl)
		{
    		if(e_id==c.b.bl_id)
	    	{
		    	if(c.b.p_id>cprs.b.p_id)
			    {
     				keep_task[cprs.b.p_id].buf=cprs.f;
	    			keep_task[cprs.b.p_id].sp=SP;
		    		cprs.f=keep_task[i].buf;
			    	SP=keep_task[i].sp;
				    break;
		    	}
			    else 
			    {
					c.b.bl=0xff;
					keep_task[i].buf=c.f;
				}
			}
		}		
	}

}

void ext_sleep(unsigned char pro_id)
{
	if(pro_id>cprs.b.p_id)
	{
		last_task=cprs.b.p_id;
		keep_task[last_task].sp=SP;
		keep_task[last_task].buf=cprs.f;
		SP=keep_task[pro_id].sp;
		cprs.f=keep_task[pro_id].buf;
		cprs.b.bl=0;
	}
/*
	else if(pro_id>last_task)
	{
		last_task=pro_id;
	}
*/
}


void task_init(void)
{
	sys_init();
	n_buf_num=0;
	n_buf_pos=0;
	task_max=0;
}
