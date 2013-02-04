/* 
 * File:   uc.h
 * Author: sef
 *
 * Created on January 27, 2013, 1:59 PM
 */

#ifndef UC_H
#define	UC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "common.h"

    // execute single JVM instruction
    // return false on error
    Boolean ucTasks();
    void ucPayload(UInt16 payload_size, UInt16 offset, UInt8 size, BYTE* data);
    void ucReset();
    Boolean ucInit(void);
    // stdin, stdout, stderr => input, output, error
    void ucInput(UInt16 maxsize, BYTE* data);
    void ucOutput(UInt16 size, BYTE* data);
    void ucError(UInt16 size, BYTE* data);

#ifdef	__cplusplus
}
#endif

#endif	/* UC_H */

