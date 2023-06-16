#include "config.h"
#include "dix.h"
#include "reg51f.h"
#include "lcm.h"

#define IXRAM	AUXR &=0xfd
#define XRAM	AUXR |=0x02

unsigned char xdata tmp_buf[64] _at_ XRAM_END-63;
unsigned char xdata framebuf[64][24] _at_ 0;
static unsigned char xdata f_x,f_y,f_l,f_w;


void flush(void)
{
	
	lcm_write(f_x,f_y,f_l,f_w);
	f_x=0xff;
	f_y=0xff;
	f_l=0;
	f_w=0;	
}
static setupflush(unsigned char x,unsigned char y,unsigned char l,unsigned char w)
{
	f_x=(x<f_x)?x:f_x;
	f_y=(y<f_y)?y:f_y;
	f_l=(l>f_l)?l:f_l;
	f_w=(w>f_w)?w:f_w;
}

unsigned char screen(unsigned char x,unsigned char y,unsigned char length,unsigned char width, unsigned char xdata *add)
{
	unsigned char xdata x_s,l,w;
	unsigned char xdata *p;
	union
	{
		struct
		{
			unsigned char a;
			unsigned char b;
		}half;
		unsigned short c;
	}tmp;
	 

	x_s=x%8;
	p=x/8;

	tmp.c=0xffff;
	if(x_s)
	{
		for(w=0;w<width;w++)
		{
			p +=framebuf[y+w];
			for(l=0;l<length;l++)
			{
				tmp.half.a |=(0xff & (~tmp.half.a));
				tmp.half.b=0xff;
				if(*add)
					tmp.c &=((*add)<<(16-x_s));
				else
					tmp.half.a &=~(0xff>>x_s)
				add++;
				*p |=tmp.half.a;
				p++;
				tmp.half.a=tmp.half.b;
			}
			*p |=tmp.half.a;
		}
	}
	else
	{
		for(w=0;w<width;w++)
		{
			p +=framebuf[y+w];
			for(l=0;l<length;l++)
			{
				if(*add)
					*p |=*add;
				add++;
				p++;
			}
		}
	   l--;
	}
	y=!(y%8)?(y/8):(y/8+1);
	setupflush(x,y,l,(w-1));
}

unsigned char screen_fill(unsigned char x,unsigned char y,unsigned char length,unsigned char width, unsigned char dat)
{
	unsigned char xdata x_s,l,w;
	unsigned char xdata *p;
	union
	{
		struct
		{
			unsigned char a;
			unsigned char b;
		}half;
		unsigned short c;
	}tmp;
	 

	x_s=x%8;
	p=x/8;

	if(x_s)
	{
		if(dat)
		{
			tmp.c=0xffff;
			for(w=0;w<width;w++)
			{
				p +=framebuf[y+w];
				for(l=0;l<length;l++)
				{
					tmp.half.a |=(0xff & (~tmp.half.a));
					tmp.half.b=0xff;
					tmp.c &=((dat)<<(16-x_s));
					*p |=tmp.half.a;
					p++;
					tmp.half.a=tmp.half.b;
				}
				*p |=tmp.half.a;
			}
		}
		else
			for(w=0;w<width;w++)
			{
				p +=framebuf[y+w];
				tmp.half.a =0xff>>x_s;
				tmp.half.b =~tmp.half.a;
				*p |=tmp.half.b;
				for(l=1;l<(length-1);l++)
				{
					p++;
					*p=0;
				}
				p++;
				*p |=tmp.half.a;
			}
	}
	else
	{
		tmp.c=0xffff;
		for(w=0;w<width;w++)
		{
			p +=framebuf[y+w];
			for(l=0;l<length;l++)
			{
				if(dat)
					*p |=dat;
				p++;
			}
		}
		l--;
	}
	y=!(y%8)?(y/8):(y/8+1);
	setupflush(x,y,l,(w-1));
	return 1;
}