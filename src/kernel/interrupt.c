#include "reg51f.h"

static unsigned char xdata all;
static unsigned char xdata ex0;
//static unsigned char xdata et0;
static unsigned char xdata ex1;
//static unsigned char xdata et1;
static unsigned char xdata es;

void dis_all_int(void)
{
	EA=0;
	all++;
}

void en_all_int(void)
{
	all--;
	if(!all)
	{
		EA=1;
	}
	if(all==0xff)
	{
		all=0;
	}
}

void dis_int(unsigned char x)
{
	switch(x)
	{
		case 1:
			dis_all_int();
			EX0=0;
			ex0++;
			en_all_int();
			break;
		case 4:
			dis_all_int();
			EX1=0;
			ex1++;
			en_all_int();
			break;
/*
		case 8:
			dis_all_int();
			ET1=0;
			et1++;
			en_all_int();
			break;
*/
		case 0x10:
			dis_all_int();
			ES=0;
			es++;
			en_all_int();
	}	
}

void en_int(unsigned char x)
{
	switch(x)
	{
		case 1:
			dis_all_int();
			ex0--;
			if(!ex0)
			{
				EX0=1;	
			}
			en_all_int();
			break;
		case 4:
			dis_all_int();
			ex1--;
			if(!ex1)
			{
				EX1=1;
			}
			en_all_int();
			break;
/*
		case 8:
			dis_all_int();
			et1--;
			if(!et1)
			{
				ET1=1;
			}
			en_all_int();
			break;
*/
		case 0x10:
			dis_all_int();
			es--;
			if(!es)
			{
				ES=1;
			}
			en_all_int();
	}
}

void interrupt_init(void)
{
	TCON=0x05;
	IE=0x12;
	IP=0x3D;
	all=1;
	ex0=0;
//	et0=0;
	ex1=0;
//	et1=0;
	es=0;	
}