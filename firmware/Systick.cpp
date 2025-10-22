/*
 * Systick.cpp
 *
 *  Created on: 3 abr. 2025
 *      Author: chor
 */

#include "Systick.h"

void InitSystick ( void )
{
	//Inicializo el Systick para que interrumpa cada 1mseg:
	SYSTICK->CSR=6;				//Habilito la interrupción del systick y selecciono como fuente del systick el fro
	SYSTICK->RVR=30000-1;		//Configuro el systick cada 1ms, le pongo -1 porque el micro tarda un tick mas en hacer el reload
	SYSTICK->CVR=SYSTICK->RVR;	//Hago que la primer interrupción dure lo mismo que el resto
	SYSTICK->CSR |= 1;			//Habilito el systick
}

void SysTick_Handler (void)
{
	for (int i = 0 ; i < PerifericoTemporizado::cant_pt ; i++)
		perifericosTemp[i]->handler();
}


