#include "graphic.h"
#include "dix.h"

#define	ABSUB(x,y)	(((x)>(y))?((x)-(y)):((y)-(x)))

extern unsigned char xdata tmp_buf[64];

/*
static unsigned char absub(unsigned char x,unsigned char y)
{
	return ((x>y)?(x-y):(y-x));
}
*/

static void clr_buf(unsigned char num)
{
	unsigned char i;
	for(i=0;i<num;i++)
	{
		tmp_buf[i]=0;	
	}
}

void l_math(unsigned char x0,unsigned char y0,unsigned char x,unsigned char y)
{
	unsigned char data residue,last;
	unsigned char xdata xn,yn,x_l,y_l,z,x1,y1;
	unsigned char xdata carry,l_y,i,dir;

	x_l=x-x0;
	x1=x0;
	y1=y0;
	y_l=(y>y0)?(y-y0):(y0-y);
	dir=(y>y0)?1:0;
	z=x0%8;
	last=0x80>>z;
	
	clr_buf(8);
	if(x_l==y_l)
	{
		l_y=0x80;
		residue=(x0/8);

		if(z)
		{
			residue++;
			z=7-z;

			for(i=0;last;i++)
			{
				tmp_buf[i]=last;
				last=last>>1;
			}
			framebuf_update(x0,y0,x0+z,y0+z,tmp_buf);
		}

		for(i=0;i<8;i++)
		{
			tmp_buf[i]=l_y;
			l_y=l_y>>1;
		}
		
		residue=residue<<3;
		x0=residue;
		y0=x0-x1+y0;
		l_y=(dir)?7:(-7);

		for(;x0<x;x0+=8)
		{
			framebuf_update(x0,y0,x0+7,y0+l_y,tmp_buf);
			y0 +=l_y+1;
		}

		x0=x0-7;
		y0=y0-7;
		framebuf_update(x0,y0,x,y,tmp_buf);
	}
	else if(x_l>y_l)
	{
		residue=y_l%x_l;
		yn=0;
		l_y=yn;
		z=0;

		for(i=1;i<x_l;i++)
		{
			tmp_buf[l_y] |=last;
			z +=residue;
			if(z-x_l>0)
			{
				z=z-x_l;
				carry=1;
			}
			else carry=0;
			yn +=carry;

			last=last>>1;
			if(!last)
			{
				l_y=yn-1;
				l_y=(dir)?l_y:(l_y+1);
				last=0x80;
				framebuf_update(x0,y0,x0,y0+l_y,tmp_buf);
				x0=i;
				y0=yn;
				clr_buf(8);
			}

			if(z>=(x_l>>1))
				l_y=1+yn;
			else l_y=yn;
		}
		tmp_buf[l_y] |=last;
		l_y=(dir)?(yn-1):(~yn+2);
		framebuf_update(x0,y0,x0,y0+l_y,tmp_buf);
	}
	else
	{
		residue=x_l%y_l;
		xn=8-residue;

		for(i=1;i<y_l;i++)
		{
			yn=last;
			tmp_buf[i-1] |=yn;
			
			z +=residue;
			if(z-y_l>0)
			{
				z=z-y_l;
				carry=1; 
			}
			else carry=0;
			last=last>>carry;
			if(z>=(y_l>>1))
			{
				yn=last>>1;
			}
			else yn=last;
			if(!last)
			{
				l_y=(dir)?(i-1):(~i+2);
				last=0x80;
				framebuf_update(x0,y0,x0,y0+l_y,tmp_buf);
				y0=i;
				x0+=xn;
				xn=8;
				clr_buf(8);
			}
		}
		tmp_buf[i-1] |=last;
		l_y=(dir)?(i-1):(~i+2);
		framebuf_update(x0,y0,x0,y0+l_y,tmp_buf);
	}
	screen_update(x1,x-x1+1,0);	
}

void c_math(unsigned char x,unsigned char y,unsigned char r)
{
	unsigned char xdata xn,yn;
	unsigned char data r2,xn2,tmp2;
	unsigned char xdata l_y,l_y2,yn2,last;
	unsigned char xdata quotient,remainder;

	r2=r*r;
	xn2=0;
	yn2=r2;
	xn=0;
	yn=r;
	last=0x80>>(x%8);

	clr_buf(64);
	
	do
	{
		tmp_buf[yn]=last;
		l_y=yn;
		l_y2=yn2;
		yn++;
		yn2=yn*yn;
		xn2=xn2+1+2*xn;
		xn++;
		if(ABSUB(yn2,r2-xn2)>ABSUB(l_y2,r2-xn2))
		{
			yn=l_y;
			yn2=l_y2;
		}

		last>>1;
		if(!last)
			last=0x80;
		quotient=yn/xn;
		remainder=yn%xn;
		
	}while((quotient) && (remainder));

	if(!remainder)
	{
		tmp_buf[yn] |=last;
	}
	else
	{
		xn++;
		yn=l_y;
	}
	
	framebuf_update(x,y+r,x+xn,y+yn,tmp_buf);
	
		
}
