/*
 * Systick.h
 *
 *  Created on: 3 abr. 2025
 *      Author: chor
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#if defined (__cplusplus)
extern "C" {

void SysTick_Handler (void);
}
#endif

#include "LPC845.h"
#include <PerifericoTemporizado.h>

void InitSystick ( void );


#endif /* SYSTICK_H_ */
