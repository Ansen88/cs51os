#ifndef _CDEFINE_H_
#define _CDEFINE_H_

typedef union _cprs
{
	unsigned char f;
	struct
	{
		unsigned char p_id	: 2;
		unsigned char bl	: 1;
		unsigned char 		: 1;
		unsigned char bl_id	: 4;
	}b;
}_cprs;

typedef union
{
	unsigned char f;
	struct
	{
		unsigned char b_t	: 1;
		unsigned char p_id	: 2;
		unsigned char req	: 1;
		unsigned char w_b	: 1;
		unsigned char r_b	: 1;
		unsigned char 		: 2;
	}b;
}ef;

#define uint8	unsigned char xdata


#endif