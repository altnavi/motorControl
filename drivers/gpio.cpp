/*
 * GPIO.cpp
 *
 *  Created on: 2 abr. 2023
 *      Author: chor
 */

#include <Init.h>
#include <gpio.h>

const uint8_t gpio::iocon_index[2][32] = {{17,11,6,5,4,3,16,15,14,13,8,7,2,1,18,10,9,0,30,29,28,27,26,25,24,23,22,21,20,50,51,35},{36,37,38,41,42,43,46,49,31,32,55,54,33,34,39,40,44,45,47,48,52,53,0,0,0,0,0,0,0,0,0,0}};

gpio::gpio()
{
	//Valores fuera de rango:
	puerto = 3;
	pin = 0;
	direction = 2;
	actividad = 2;
}

gpio::gpio ( uint8_t _puerto , uint8_t _pin, uint8_t dir , uint8_t act)
{
	puerto = _puerto;
	pin = _pin;
	direction = dir;
	actividad = act;

	//Algunas configuraciones que tengo que hacer
	//independientemente de si el pin se comporta
	//como entrada o salida:

	if ( puerto == 0)
		//Habilito el CLK para el GPIO0:
		SYSCON->SYSAHBCLKCTRL0 |= 1 << SYSCON_SYSAHBCLKCTRL0_GPIO0_SHIFT;
	if ( puerto == 1)
		//Habilito el CLK para el GPIO0:
		SYSCON->SYSAHBCLKCTRL0 |= 1 << SYSCON_SYSAHBCLKCTRL0_GPIO1_SHIFT;

	setDIR ( direction );

	//En principio seteo el modo por defecto:
	if ( dir == ENTRADA )
		setModeIN();
	else
		setModeOUT();

}

bool gpio::getPIN ( void )
{
	return (((GPIO->PIN[puerto]>>pin)&0x01) == actividad);
}

void gpio::setPIN (uint8_t val)
{
	if (direction == SALIDA )
	{
		if ( !val )
			GPIO->PIN[puerto] &= ~(1<<pin);
		else
			GPIO->PIN[puerto] |= (1<<pin);
	}
}

void gpio::togglePIN ()
{
	if (direction == SALIDA)
		GPIO->PIN[puerto] ^= (1<<pin);
}

void gpio::setModeOUT ( uint8_t modo )
{
	//No hace falta borrar previamente porque es un solo bit:
	if (modo)
		IOCON->PIO[iocon_index[puerto][pin]] |= 1<<10;
	else
		IOCON->PIO[iocon_index[puerto][pin]] &= ~(1<<10);
}

void gpio::setModeIN ( uint8_t modo )
{
	//Primero borro los bits 3 y 4 del registro:
	uint8_t indice = iocon_index[puerto][pin];
	IOCON->PIO[ indice ] &= ~(3<<3);
	IOCON->PIO[ indice ] |= modo << 3;
}

void gpio::setDIR ( uint8_t dir )
{
	direction = dir;
	if ( direction == SALIDA )
		//Pongo en 1 el bit del registro para indicar que es una salida:
		GPIO->DIR[puerto] |= 1<<pin;
	else
		//Pongo un 0 en el bit del registro para indicar que es una entrada
		GPIO->DIR[puerto] &= ~1<<pin;
}

void gpio::setPIN()
{
	setPIN(actividad);
}

void gpio::clrPIN()
{
	if (actividad == HIGH)
		setPIN(LOW);
	else
		setPIN(HIGH);
}

gpio::gpio (const gpio & G)
{
	puerto = G.puerto;
	pin = G.pin;
	actividad = G.actividad;
	direction = G.direction;
}

gpio & gpio::operator = (const gpio &G)
{
	puerto = G.puerto;
	pin = G.pin;
	actividad = G.actividad;
	direction = G.direction;

	return *this;
}

bool gpio::operator == (uint8_t val)
{
	uint8_t val_pin = getPIN();

	if ( actividad == LOW )
		val_pin ^= 1;

	return (val_pin == val);
}

gpio & gpio::operator = (uint8_t val)
{
	setPIN(val);
	return *this;
}



