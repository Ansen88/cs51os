#include "kernel.h"
#include "dma.h"
#include "gmath.h"
#include "lcm.h"
#include "graphic.h"

#define	ABSUB(x,y)	(((x)>(y))?((x)-(y)):((y)-(x)))
 
void draw_line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
	if(x2>x1)
		l_math(x1,y1,x2,y2);
	else
		l_math(x2,y2,x1,y1);	
}

unsigned char d_circle(unsigned char x,unsigned char y,unsigned char r)
{
	if(x>y)
		return r+1;
	else
		return r-1;
}

/*************** API ****************/

