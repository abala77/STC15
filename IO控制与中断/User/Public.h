#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#define true 1
#define false 0

#define bit0 0x01
#define bit1 0x02
#define bit2 0x04
#define bit3 0x08
#define bit4 0x10
#define bit5 0x20
#define bit6 0x40
#define bit7 0x80


typedef struct
{
	void (*Delayms)(int);
}Public_t;

extern Public_t Public;

#endif