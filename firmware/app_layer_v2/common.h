#ifndef _COMMON_H_
#define _COMMON_H_

// === IOIO: AppLayer
#include "platform.h"
#include "Compiler.h"
#include "HardwareProfile.h"
#include "byte_queue.h"
#include "features.h"
#include "pwm.h"
#include "adc.h"
#include "digital.h"
#include "logging.h"
#include "platform.h"
#include "uart.h"
#include "spi.h"
#include "i2c.h"
#include "sync.h"
#include "icsp.h"
#include "incap.h"
// === IOIO ===

typedef signed char Int8;
typedef signed short Int16;
typedef signed long Int32;
typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned long UInt32;
typedef unsigned char Boolean;
typedef float UjFloat;

typedef unsigned long UInt24;

#define true	1
#define false	0
#ifndef NULL
	#define NULL	0
#endif
#ifndef NAN
	#define NAN	(0.0f/0.0f)
#endif

void err(const char* str);

#ifndef offsetof
    #define offsetof(struc, membr)	((int)(&((struc*)0)->membr))
#endif

#include <stdio.h>
#define TL(...)		//fprintf(stderr, "**UL** " __VA_ARGS__)

#define DEBUG		1

#define _UNUSED_	__attribute__((unused))
#define _INLINE_	__attribute__((always_inline)) inline 


#define GET_FAR_ADDRESS(var)                          \
({                                                    \
    uint_farptr_t tmp;                                \
                                                      \
    __asm__ __volatile__(                             \
                                                      \
            "ldi    %A0, lo8(%1)"           "\n\t"    \
            "ldi    %B0, hi8(%1)"           "\n\t"    \
            "ldi    %C0, hh8(%1)"           "\n\t"    \
            "clr    %D0"                    "\n\t"    \
        :   "=d" (tmp)                                \
        :   "p"  (&(var))                             \
    );                                                \
    tmp;                                              \
}) 


	#define GET_ADDRESS(x)		((UInt32)(&(x)))
	UInt8 pgm_read(UInt32 addr);
	#define pgm_read_str(addr)	(*((UInt8*)(addr)))
	#define _PROGMEM_
	#define HEAP_ALIGN		2
	#define xxxSDLOADER
        #define UJ_OPT_CLASS_SEARCH
        //#define UJ_FTR_SUPPORT_UJC_FORMAT
        #define UJ_FTR_SUPPORT_CLASS_FORMAT
	#define GPIO_PORTS		2
	#define GPIO_PINS_PER_PORT	16
	#define EEPROM_SIZE		1536
	#define _HEAP_ATTRS_		__attribute__((far))
	#define EEPROM_EMUL_ADDR	0x800000UL	//we use 4 pages
	
		//#define F_CPU			32000000UL
		#define F_CPU			GetSystemClock()
		#define HEAP_SZ			6336
	#define HAVE_I2C
	#define FLASHROM_PAGE		1024	//actually later they become 512, but who cares :)
	

        #define ujLog log_printf

//common board api
	void board_init(void);
	UInt32 getClock(void);
	UInt32 getClkTicksPerSec(void);
	void fail_msg(UInt8 var);

//i2c api
	void i2cStart(void);
	Boolean i2cSend(UInt8 v);
	UInt8 i2cRecv(Boolean ack);
	void i2cStop(void);

//SPI api
	void spiClockSpeed(Boolean fast);
	UInt8 spiByte(UInt8 v);
	void spiSingleClock(void);

//gpio api
	void gpioSetOut(UInt8 port, UInt8 pin, Boolean isOutput);
	void gpioSetVal(UInt8 port, UInt8 pin, Boolean isHi);
	Boolean gpioGetVal(UInt8 port, UInt8 pin);

//EEPROM api
	UInt8 eepromRead(UInt16 addr);
	void eepromWrite(UInt16 addr, UInt8 val);

//character LCD api
	void clcdClear(void);
	void clcdGotoXY(UInt8 x, UInt8 y);
	void clcdChar(char c);
	void clcdRawWrite(UInt8 val);


//GLCD api
	void glcdClear(void);
	void glcdSetPixels(UInt8 rowOver8, UInt8 c, UInt8 vals);				//8 pixels, vertically. LSB = top
	void glcdSetManyPixels(UInt8 rowOver8, UInt8 c, const UInt8* vals, UInt16 nCols);	//8 pixels, vertically. LSB = top

//PWM api
	#define PWM_WHITE_LED	0
	#define PWM_LCD_BL	1
	Boolean pwmSet(UInt8 which, UInt8 bri);

//flashrom API

	#define FLASH_KEY	0x4648
	UInt32 getFlashFreeArea(UInt32* sz);	//return addr
	void flashromWrite(UInt32 addr, const UInt8* data, UInt16 key);

#endif
