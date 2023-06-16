#ifndef	  _KERNEL_H_
#define	  _KERNEL_H_

#include "reg51f.h"

/*************define the MSG*********************/
#define	MSG_IRQ_REV		0
#define	MSG_SLEEP		1
#define	MSG_WAKEUP		2
#define MSG_R_BUF		3

unsigned char sent_msg(unsigned char x,unsigned char para);

/**********interrupt operation****************/
#define	EX0		1
//#define	ET0		1<<1	SYSTEM USING,CANNOT BE FORBIDEN
#define	EX1		1<<2
#define	ET1		1<<3
#define	ES		1<<4

void dis_int(unsigned char);
void en_int(unsigned char);
void dis_all_int(void);
void en_all_int(void);

/**********low_level operation for hardwares*****/


#endif