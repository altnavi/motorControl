/*
 * PinInt.cpp
 *
 *  Created on: 5 jun. 2023
 *      Author: chor
 */

#include <PinInt.h>

PinInt * intext [8] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
uint8_t PinInt::cant_pinInt = 0;

PinInt::PinInt( uint8_t port , uint8_t pin , tipos_trigger trig , void (*h) (void) )
{
	//La variable estática cant_PinInt nos indica cuantos objetos fueron instanciados:
	fuente = cant_pinInt;
	trigger = trig;
	handler = h;
	intext[cant_pinInt] = this;

	cant_pinInt++;

	//Habilito el clk del perisferico PIN INTERRUPT
	SYSCON->SYSAHBCLKCTRL0 |= 1 << 28;

	//Fuente de interrupcion indicada por int_src
	//Tengo que poner el nro de pin que generará la interrupción:
	//P0.0 a P0.31 se los indica con los números 0 a 31
	//P1.0 a P1.31 se los indica con los números 32 a 63
	SYSCON->PINTSEL[ fuente ] = port*32 + pin;

	//Configuro interrupciones segun me indico el usuario:
	configIntExt();

}


PinInt::~PinInt()
{
	//Deshabilito la interrupción en el NVIC:
	ICER0 |= 1 << (24+fuente);
	trigger = none;

	//Deshabilito flanco y nivel de esta fuente de interrupcion:
	configIntExt();
}


void PinInt::configIntExt ()
{
	//De acuerdo a lo configurado en el campo trigger, modifico
	//los registros ISEL, IENF e IENR:

	//Si es por flanco ISEL = 0, si es por nivel ISEL = 1
	if ( trigger == flanco_asc || trigger == flanco_desc || trigger == ambos_flancos)
		INT_EXT->ISEL &= ~(1<<fuente);
	else if ( trigger == nivel_alto || trigger == nivel_bajo )
		INT_EXT->ISEL |= 1<<fuente;

	//Si es por flanco ascendente o por nivel IENR = 1
	if ( trigger == flanco_asc || trigger == ambos_flancos || trigger == nivel_alto || trigger == nivel_bajo)
		INT_EXT->IENR |= 1<<fuente;
	else if ( trigger == flanco_desc || trigger == none )
		INT_EXT->IENR &= ~(1<<fuente);

	//Si es por flanco descendente o por nivel alto IENF = 1
	if ( trigger == flanco_desc || trigger == ambos_flancos || trigger == nivel_alto )
		INT_EXT->IENF |= 1<<fuente;
	else if ( trigger == flanco_asc || trigger == nivel_bajo )
		INT_EXT->IENF &= ~(1<<fuente);

	//Por último, el registro ISER0 es el que habilita las interrupciones en el NVIC:
	if ( trigger != none )
		ISER0 |= 1 << (24+fuente);

}

void PININT0_IRQHandler (void)
{
	INT_EXT->IST |= 1;
	if ( intext[0] != nullptr )
		intext[0]->handler();
}

void PININT1_IRQHandler ( void )
{
	INT_EXT->IST |= 2;
	if ( intext[1] != nullptr )
		intext[1]->handler();

}

void PININT2_IRQHandler ( void )
{
	INT_EXT->IST |= 4;
	if ( intext[2] != nullptr )
		intext[2]->handler();

}

void PININT3_IRQHandler ( void )
{
	INT_EXT->IST |= 8;
	if ( intext[3] != nullptr )
		intext[3]->handler();

}

void PININT4_IRQHandler ( void )
{
	INT_EXT->IST |= 0x10;
	if ( intext[4] != nullptr )
		intext[4]->handler();

}

void PININT5_IRQHandler ( void )
{
	INT_EXT->IST |= 0x20;
	if ( intext[5] != nullptr )
		intext[5]->handler();

}

void PININT6_IRQHandler ( void )
{
	INT_EXT->IST |= 0x40;
	if ( intext[6] != nullptr )
		intext[6]->handler();

}

void PININT7_IRQHandler ( void )
{
	INT_EXT->IST |= 0x80;
	if ( intext[7] != nullptr )
		intext[7]->handler();

}

