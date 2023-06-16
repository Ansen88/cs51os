#include "cdefine.h"
#include "kernel.h"
#include "equ.h"
#include "dma.h"

static ef data f;
static uint8 s_buf_w[128];
static uint8 wp_s;
static uint8 wp_e;
static uint8 s_buf_r[128];
static uint8 rp_s;
static uint8 rp_e;


void serial_init(void)
{
	wp_s=0;
	wp_e=0;
	rp_s=0;
	rp_e=0;
	f.f=0;

	SCON=0X50;
	TMOD |= 0x20;
	TH1   = 221;
	TR1=1;
}

unsigned char s_open(unsigned char pro_id)
{
	unsigned char result;

	dis_all_int();
	
	if(!f.b.b_t)
	{
		f.b.b_t=1;
		f.b.p_id=pro_id;
		result=1;
	}
	else result=0;

	en_all_int();
	return result;
}

unsigned char s_close(unsigned char pro_id)
{
	unsigned char result;

	dis_all_int();

	if(pro_id==f.b.p_id)
	{
		f.b.b_t=0;
		result=1;
		if(f.b.req)
		{
			sent_msg(MSG_WAKEUP,ID_SERIAL);
		}
		else if(f.b.r_b)
		{
			sent_msg(MSG_IRQ_REV,s_buf_r[rp_e]);
			rp_e++;	
		}
	}
	else result=0;

	en_all_int();
	return result;
}

unsigned char s_write(unsigned char pro_id,unsigned char *src,unsigned char num)
{
	uint8	n;

	if((f.b.p_id != pro_id) ||(f.b.w_b))
	  return 0;

	n=wp_s-wp_e;
	if(wp_e)
	{
		if(dma(s_buf_w,(s_buf_w+wp_e),n))
		{
			wp_e=0;
			wp_s=n;	
		}
		else
		{
			n=wp_s;
		}
	}
	
	n=128-n;	
	num=(num>n)?n:num;
	if(num==n)
		f.b.w_b=1;
	dma((s_buf_w+wp_s),src,num);
/*
	while(!dma((s_buf_w+wp_s),src,num))
	{
		sent_msg(MSG_SLEEP,0);
	}
*/
	wp_s+=num;
	TI=1;
	return num;	
} 

unsigned char  s_read(unsigned char pro_id,unsigned char *dst,unsigned char num)
{
	uint8	n;
	if((f.b.p_id != pro_id)	||(!f.b.r_b)) 
	  return 0;
	n=rp_s-rp_e;
/*	
	if(!n)
	{
		return 0;
	}
	if(!num)
	{
		*dst=s_buf_r[rp_e];
		return 1;
	}
*/	
	num=(num>n)?n:num;
	dma(dst,(s_buf_r+rp_e),num);
/*
	while(!dma(dst,(s_buf_r+rp_e),num))
	{
		cprs.b.bl=1;
		cprs.b.bl_id=ID_DMA;
		sent_msg(MSG_SLEEP,0);
	}
*/
	rp_e+=num;
	dma(s_buf_r,(s_buf_r+rp_e),n);

/*
	while(!dma(s_buf_r,(s_buf_r+rp_e),n))
	{
		cprs.b.bl=1;
		cprs.b.bl_id=ID_DMA;
		sent_msg(MSG_SLEEP,0);
	}
*/

	rp_s=n;
	rp_e=0;
	f.b.r_b=0;

	return num;	
}	

void s_port(void) interrupt 4 using 3
{
	if(RI)
	{
		if(rp_s!=128)
		{
			s_buf_r[rp_s]=SBUF;
			rp_s++;
			f.b.r_b=1;
			if(f.b.b_t)
			{
				sent_msg(MSG_R_BUF,f.b.p_id);
			}
			else
			{ 
				sent_msg(MSG_IRQ_REV,s_buf_r[rp_e]);
				rp_e++;
			}
		}
		RI=0;
	}
	if(TI)
	{
		if(wp_e!=wp_s)
		{
			SBUF=s_buf_w[wp_e];
			wp_e++;
			f.b.r_b=0;
		}
		TI=0;
	}
}

