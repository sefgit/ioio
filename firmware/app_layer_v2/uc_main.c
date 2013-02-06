#include <math.h>
#include <stdarg.h>
#include "common.h"
#include "uj.h"
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

extern const UjNativeClass nativeCls_UC;

static BYTE *uj_code=NULL;

UInt8 ujReadClassByte(UInt32 id, UInt32 offset){
        // TODO: range error checking
	return uj_code[offset];
}

#if 0
BYTE *uj_input = NULL;
DWORD uj_input_size = 0;
void ucInput(UInt16 maxsize, BYTE* data)
{
    if (uj_input!=NULL) {
        free(uj_input);
    }
    uj_input = malloc(maxsize);
    if (uj_input!=NULL) {
        uj_input_size = maxsize;
        memcpy(uj_input, data, maxsize);
    }
};

void ucOutput(UInt16 size, BYTE* data)
{
    // FIXME
};
void ucError(UInt16 size, BYTE* data)
{
    // FIXME
};

#endif
static UInt32 threadH = 0;
void ucReset() {
        if (threadH!=0) {
                ujThreadDestroy(threadH);
                threadH = 0;
        }
}
void ucPayload(UInt16 payload_size, UInt16 offset, UInt8 size, BYTE* data)
{
        ucReset();
        if (uj_code == NULL) {
            uj_code = malloc(payload_size);
            if (uj_code == NULL) {
		ujLog("payload: out of memory:%d %d %d", payload_size, offset, size);
                return;
            }
        }
        // TODO: range error checking
        BYTE *p = (BYTE*)&uj_code[offset];
        memcpy(p, data, size);
}


//
// modified to support loading singlemain class only
//

static struct UjClass* mainClass = NULL;
static struct UjClass* objectClass = NULL;

Boolean ucInit(void){
	
	UInt8 i, ret;
	ujLog("uJ init");
        ucReset();
        if (uj_code==NULL) {
            //ujLog("ujInit() failed, no payload");
            return false;
        }
	ret = ujInit(&objectClass);
	if(ret != UJ_ERR_NONE){
		ujLog("ujInit() failed");
		goto fail;	
	}
	ret = ujRegisterNativeClass(&nativeCls_UC, objectClass, NULL);
	if(ret != UJ_ERR_NONE){
		ujLog("ujRegisterNativeClass() failed");
		goto fail;	
	}
        // id = 0
	ret = ujLoadClass(0, &mainClass);
	if(ret != UJ_ERR_NONE){
		ujLog("ujLoadClass() failed");
		goto fail;
        }
        //success
	//ujLog("loaded");

	ret = ujInitAllClasses();
	ujLog("classes inited: %d", ret);
	if(ret != UJ_ERR_NONE){
		ujLog("ujInitAllClasses() failed");
		goto fail;	
	}
	
	//now classes are loaded, time to call the entry point
	
	threadH = ujThreadCreate(0);
	if(threadH == 0){
		ujLog("ujThreadCreate() failed");
		goto fail;	
	}
	
	i = ujThreadGoto(threadH, mainClass, "main", "()V");
	if(i == UJ_ERR_METHOD_NONEXISTENT){
	
		ujLog("Main method not found!");
		goto fail;
	}
	ujLog("uJ main method created!");
    return true;
fail:
        if (threadH!=0) {
                ujThreadDestroy(threadH);
                threadH = 0;
        }
        if (uj_code!=NULL) {
            free(uj_code);
            uj_code = NULL;
        }
    return false;
}



Boolean ucTasks() {
    UInt8 i;
    //UInt8 n=5;
    if(threadH==0){
        return true;
    }
    //while(n-- > 0) {
        if (ujCanRun()) {
            i = ujInstr();
            if(i != UJ_ERR_NONE){
                    ujLog("Ret %d @ instr right before 0x%08lX", i, ujThreadDbgGetPc(threadH));
                    ujThreadDestroy(threadH);
                    threadH = 0;
                    free(uj_code);
                    uj_code = NULL;
                    return false;
            }
        }
    //}
    return true;
}

static UInt8 natUc_clkGet(struct UjThread* t, _UNUSED_ struct UjClass* cls){

	return ujThreadPush(t,  GetSystemClock(), false) ? UJ_ERR_NONE : UJ_ERR_STACK_SPACE;
}

static UInt8 natUc_clkSpeed(struct UjThread* t, _UNUSED_ struct UjClass* cls){
	return ujThreadPush(t, (((GetSystemClock() / 2) + 128) >> 8UL), false) ? UJ_ERR_NONE : UJ_ERR_STACK_SPACE;
}

static UInt8 natUc_instrsGet(struct UjThread* t, _UNUSED_ struct UjClass* cls){

	return ujThreadPush(t, ujGetNumInstrs(), false) ? UJ_ERR_NONE : UJ_ERR_STACK_SPACE;
}

static UInt8 natUc_msDelay(struct UjThread* t, _UNUSED_ struct UjClass* cls){
//  void msDelay(UInt32 ms)
    UInt32 _ms = ujThreadPop(t);
    UInt32 _dcnt;
    while (_ms)
    {
        _dcnt=((UInt32)(0.001/(1.0/GetInstructionClock())/6));
        while(_dcnt--);
        _ms--;
    }
    return UJ_ERR_NONE;
}

/*
// TODO: receive multiple bytes / string from app!!!

static UInt8 natUC_input(struct UjThread* t, _UNUSED_ struct UjClass* cls){
// UInt32 input(UInt32 maxsize, BYTE[] data)
    UInt32 res = 0;
    UInt32 _dataptr = ujThreadPop(t);
    UInt32 _maxsize = ujThreadPop(t);
    uj_input = malloc(_maxsize);
    if (uj_input != NULL) {
        BYTE* _data = ((BYTE*)ujArrayRawAccessStart(_dataptr));
        res = _maxsize<uj_input_size?_maxsize:uj_input_size;
        memcpy(_data, uj_input, res);
        ujArrayRawAccessFinish(_dataptr);
        free(uj_input);
        uj_input = NULL;
        uj_input_size = 0;
    }
    return ujThreadPush(t, res, false) ? UJ_ERR_NONE : UJ_ERR_STACK_SPACE;
}
static UInt8 natUC_output(struct UjThread* t, _UNUSED_ struct UjClass* cls){
// UInt32 input(UInt32 maxsize, BYTE[] data)
    UInt32 _dataptr = ujThreadPop(t);
    UInt32 _size = ujThreadPop(t);
// TODO: send multiple bytes / string to app!!!
    return UJ_ERR_NONE;
}
static UInt8 natUC_error(struct UjThread* t, _UNUSED_ struct UjClass* cls){
// UInt32 input(UInt32 maxsize, BYTE[] data)
    UInt32 _dataptr = ujThreadPop(t);
    UInt32 _size = ujThreadPop(t);
// TODO: send multiple bytes / string to app!!!
    return UJ_ERR_NONE;
}
*/

static UInt8 natUc_toggleLED(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 state = ujThreadPop(t);
    if (state == 0) {
        led_off();
    } else if (state > 0) {
        led_on();
    } else {
        led_toggle();
    }
    return UJ_ERR_NONE;
}
static UInt8 natUc_SetDigitalOutLevel(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    // void SetDigitalOutLevel(int pin, int value);
    // NOTE: reverse order of parameters
    UInt32 value = ujThreadPop(t);
    UInt32 pin = ujThreadPop(t);
    SetDigitalOutLevel(pin, value);
    return UJ_ERR_NONE;
}
static UInt8 natUc_HardReset(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    HardReset();
    return UJ_ERR_NONE;
}
static UInt8 natUc_SoftReset(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    SoftReset();
    return UJ_ERR_NONE;
}
// ADC
static UInt8 natUc_ADCSetScan(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 enable  = ujThreadPop(t);
    UInt32 pin = ujThreadPop(t);
    ADCSetScan(pin, enable);
    return UJ_ERR_NONE;
}
static UInt8 natUc_ADCClrScan(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 pin = ujThreadPop(t);
    //ADCClrScan(pin);
    ADCSetScan(pin, 0);
    return UJ_ERR_NONE;
}
// I2C
static UInt8 natUc_I2CConfigMaster(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 level = ujThreadPop(t);
    UInt32 rate = ujThreadPop(t);
    UInt32 num = ujThreadPop(t);
    I2CConfigMaster(num, rate, level);
    return UJ_ERR_NONE;
}
static UInt8 natUc_I2CWriteRead(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 rdbytes = ujThreadPop(t);
    UInt32 wrbytes = ujThreadPop(t);
    UInt32 dataptr = ujThreadPop(t);
    UInt32 adr = ujThreadPop(t);
    UInt32 num = ujThreadPop(t);
    BYTE* data = ((BYTE*)ujArrayRawAccessStart(dataptr));
    I2CWriteRead(num,adr,data,wrbytes,rdbytes);
    ujArrayRawAccessFinish(dataptr);
    return UJ_ERR_NONE;
}
// PWM
static UInt8 natUc_SetPwmDutyCycle(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 fraction = ujThreadPop(t);
    UInt32 dc = ujThreadPop(t);
    UInt32 num = ujThreadPop(t);
    SetPwmDutyCycle(num, dc, fraction);
    return UJ_ERR_NONE;
}
static UInt8 natUc_SetPwmPeriod(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 scale = ujThreadPop(t);
    UInt32 period = ujThreadPop(t);
    UInt32 num = ujThreadPop(t);
    SetPwmPeriod(num, period, scale);
    return UJ_ERR_NONE;
}
#if 0
// InCap
static UInt8 natUc_InCapConfig(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 clock = ujThreadPop(t);
    UInt32 mode = ujThreadPop(t);
    UInt32 prec = ujThreadPop(t);
    UInt32 num = ujThreadPop(t);
    InCapConfig(num, prec, mode, clock);
    return UJ_ERR_NONE;
}
// SPI
static UInt8 natUc_SPIConfigMaster(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 pol  = ujThreadPop(t);
    UInt32 edge  = ujThreadPop(t);
    UInt32 end  = ujThreadPop(t);
    UInt32 div  = ujThreadPop(t);
    UInt32 scale = ujThreadPop(t);
    UInt32 num = ujThreadPop(t);
    SPIConfigMaster(num, scale, div, end, edge, pol);
    return UJ_ERR_NONE;
}
static UInt8 natUc_SPITransmit(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 trim_rx = ujThreadPop(t);
    UInt32 total_size = ujThreadPop(t);
    UInt32 data_size = ujThreadPop(t);
    UInt32 dataptr = ujThreadPop(t);
    UInt32 dest = ujThreadPop(t);
    UInt32 num = ujThreadPop(t);
    BYTE* data = ((BYTE*)ujArrayRawAccessStart(dataptr));
    SPITransmit(num, dest, data, data_size, total_size, trim_rx);
    ujArrayRawAccessFinish(dataptr);
    return UJ_ERR_NONE;
}
// UART
static UInt8 natUc_UARTConfig(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 parity  = ujThreadPop(t);
    UInt32 two_stop_bits  = ujThreadPop(t);
    UInt32 speed4x  = ujThreadPop(t);
    UInt32 rate  = ujThreadPop(t);
    UInt32 num = ujThreadPop(t);
    UARTConfig(num, rate, speed4x, two_stop_bits, parity);
    return UJ_ERR_NONE;
}
static UInt8 natUc_UARTTransmit(struct UjThread* t, _UNUSED_ struct UjClass* cls){
    UInt32 size = ujThreadPop(t);
    UInt32 dataptr = ujThreadPop(t);
    UInt32 num = ujThreadPop(t);
    BYTE* data = ((BYTE*)ujArrayRawAccessStart(dataptr));
    UARTTransmit(num, data, size);
    ujArrayRawAccessFinish(dataptr);
    return UJ_ERR_NONE;
}

#endif

const UjNativeClass nativeCls_UC =
	{
		"UC",
		0,
		0,
		NULL,
		NULL,

		6
                // input, output, error
                //+ 3
                // digital
                + 2
                // ADC
                + 2
                // I2C
                + 2
                // PWM
                + 2
                // InCap
                //+ 1
                // SPI
                //+ 2
                // UART
                //+ 2
		,{
                        // common
			{
				"clkGet",
				"()I",
				natUc_clkGet,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
			},
			{
				"clkSpeed",
				"()I",
				natUc_clkSpeed,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
			},
			{
				"instrsGet",
				"()I",
				natUc_instrsGet,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
			},
			{
				"msDelay",
				"(I)V",
				natUc_msDelay,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
			},
                        {
                            // void HardReset();
				"HardReset()",
				"()V",
				natUc_HardReset,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        },
                        {
                            // void SoftReset();
				"SoftReset()",
				"()V",
				natUc_SoftReset,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        },
#if 0
			{
				"input",
				"(I[)I",
				natUC_input,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
			},
			{
				"output",
				"(I[)V",
				natUC_output,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
			},
			{
				"error",
				"(I[)V",
				natUC_error,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
			},
#endif
                                // digital
			{
				"toggleLED",
				"(I)V",
				natUc_toggleLED,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
			},
                        {
                            // void SetDigitalOutLevel(int pin, int value);
				"SetDigitalOutLevel",
				"(II)V",
				natUc_SetDigitalOutLevel,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC                  
                        }
                        // ADC
                        ,{
                            //void ADCSetScan(int pin, int enable);
				"ADCSetScan()",
				"(II)V",
				natUc_ADCSetScan,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
                        ,{
                            //void ADCClrScan(int pin);
				"ADCClrScan()",
				"()V",
				natUc_ADCClrScan,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
                        // I2C
                        ,{
                            // rate is 0:off 1:100KHz, 2:400KHz, 3:1MHz
                            // void I2CConfigMaster(int i2c_num, int rate, int smbus_levels);
				"I2CConfigMaster()",
				"(III)V",
				natUc_I2CConfigMaster,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
                        ,{
                            // void I2CWriteRead(int i2c_num, unsigned int addr, const void* data,
                            //                   int write_bytes, int read_bytes);
				"I2CWriteRead()",
				"(II[II)V",
				natUc_I2CWriteRead,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
                        // PWM
                        ,{
                                // void SetPwmDutyCycle(int pwm_num, int dc, int fraction);
				"SetPwmDutyCycle()",
				"(III)V",
				natUc_SetPwmDutyCycle,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
                        ,{
                                // void SetPwmPeriod(int pwm_num, int period, int scale);
				"SetPwmPeriod()",
				"(III)V",
				natUc_SetPwmPeriod,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
#if 0
                        // InCap
                        ,{
                                // mode:
                                //   0: off
                                //   1: high-pulse
                                //   2: low-pulse
                                //   3: rise-to-rise
                                //   4: rise-to-rise x 4
                                //   5: rise-to-rise x 16
                                //
                                // clock:
                                //   0: 16MHz
                                //   1: 2MHz
                                //   2: 250KHz
                                //   3: 62.5KHz
                                // void InCapConfig(int incap_num, int double_prec, int mode, int clock);
				"InCapConfig()",
				"(IIII)V",
				natUc_InCapConfig,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
                        // SPI
                        ,{
                                // void SPIConfigMaster(int spi_num, int scale, int div,
                                //                      int smp_end, int clk_edge, int clk_pol);
				"SPIConfigMaster()",
				"(IIIIII)V",
				natUc_SPIConfigMaster,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
                        ,{
                                // void SPITransmit(int spi_num, int dest, const void* data, int data_size,
                                //                  int total_size, int trim_rx);
				"SPITransmit()",
				"(II[III)V",
				natUc_SPITransmit,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
                        // UART
                        ,{
                                // void UARTConfig(int uart_num, int rate, int speed4x,
                                //                  int two_stop_bits, int parity);
				"UARTConfig()",
				"(IIIII)V",
				natUc_UARTConfig,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
                        ,{
                                // void UARTTransmit(int uart_num, const void* data, int size);
				"UARTTransmit()",
				"(I[I)V",
				natUc_UARTTransmit,
				JAVA_ACC_PUBLIC | JAVA_ACC_NATIVE | JAVA_ACC_STATIC
                        }
#endif
                }
        };

