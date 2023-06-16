#include "reg51f.h"
#include "kernel.h"
#include "graphic.h"

extern unsigned char xdata framebuf0[8][64];
extern unsigned char xdata framebuf1[8][64];
extern unsigned char xdata screen_bank; 


sbit _CS0 =P1^4;
sbit _CS1 =P1^3;
sbit _CS2 =P1^2;
sbit RW	  =P1^6;
sbit E	  =P1^7;
sbit CD	  =P1^5;
#define  DATA_BUS P0

void lcm_write(char x,char y,char length,char height)
{
	char xdata y1,yi,xi,i;
	char xdata buf[8];
	y1=y+height;
	yi=y/8+1;
	y=((y%8)&&(!yi))?(yi-1):yi;
	y+=0xb8;
	yi=y1/8;
	y1=(y1%8)?(yi+1):yi;
	y1+=0xb8;

	if(x<64)
	{
		_CS0=0;
		for(yi=y;yi<y1;yi++)
		{

		}		
	}	 	
}

void screen_update(unsigned char x,unsigned char x_num,unsigned char bank)
{
	unsigned char  i,page;
	unsigned char xdata *add;
	
	if(bank)
	{
		add=framebuf1[0];

		if(screen_bank)
			_CS0=0;
		else
			_CS2=0;	
	}
	else
	{
		 add=framebuf0[0];
		 _CS1=0;
	}	

	x |=0x40;
	for(page=0xb8;page<0xc0;page++)
	{
		CD=0;
		RW=0;
		DATA_BUS=page;
		E=1;
		E=0;

		DATA_BUS=x;
		E=1;
		E=0;

		CD=1;
		for(i=x_num;i>0;i--)
		{
			DATA_BUS=*add;
			E=1;
			E=0;
			add++;
		}
	}
	
	_CS0=1;
	_CS1=1;
	_CS2=1;
}

void screen_download(void)
{
	unsigned char n,i;
	unsigned char xdata *add;

/*
	dis_all_int();
	IXRAM;
*/

	if(screen_bank)
	{
		screen_bank=0;
		_CS2=0;
	}
	else
	{
		screen_bank=2;
		_CS0=0;
	}

	add=framebuf0[0];
	for(n=8;n>0;n--)
	{
		CD=0;
		RW=0;
		DATA_BUS=n;
		E=1;
		E=0;

		DATA_BUS=0x40;
		E=1;
		E=0;

		CD=1;
		RW=1;

		E=0;
		E=1;

		for(i=64;i>0;i--)
		{
			E=1;
			E=0;
			*add=DATA_BUS;
			add++;
		}

/*		
		XRAM;
		en_all_int();
		dis_all_int();	
*/
	}

	_CS0=1;
	_CS2=1;

/*
	XRAM;
	en_all_int();
*/
}