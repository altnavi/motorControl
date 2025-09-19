/*
 * DR_Init.c
 *
 *  Created on: 8 ago. 2021
 *      Author: chor
 */

#include "LPC845.h"
#include "Init.h"
#include "Systick.h"

//Genero una funcion a partir de un puntero a función que llame a esa posición de memoria:

void (*fro_set_frec) (int ) = FRO_SET_FRECUENCY_FUNC;

void Inicializar(void)
{
	//Seteo la frecuencia del fro a 30MHz, llamando a la funcion de ROM:
	fro_set_frec(30000);

	//Selecciono la salida del FRO como el clk principal del sistema:

	SYSCON->FROOSCCTRL |= 1<<17;

	//Refresco el registro para habilitar el clk de 30MHz:
	SYSCON->FRODIRECTCLKUEN &= ~1;
	SYSCON->FRODIRECTCLKUEN |= 1;

	//Seteo FRO como el clkSrc del PLL:
	SYSCON->MAINCLKPLLSEL = 0;

	SYSCON->MAINCLKPLLUEN &= ~1;
	SYSCON->MAINCLKPLLUEN |= 1;

	//Seteo FRO como el clkSrc del PLL:
	SYSCON->SYSPLLCLKSEL = 0;

	SYSCON->SYSPLLCLKUEN &= ~1;
	SYSCON->SYSPLLCLKUEN |= 1;

	SYSCON->SYSAHBCLKDIV = 1;

	InitSystick();
}
