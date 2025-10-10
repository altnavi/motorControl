/*
╭──╦──╮
│IC║XC│
╠══╬══╣
│NI║KA│
╰──╩──╯
 */
#include <cr_section_macros.h>
#include <Init.h>
#include <LPC845.h>
#include "../drivers/detectorGiro.h"
#include <gpio.h>
#include <driverMotor.h>
#include <Timer.h>

void handlr();

gpio motor(0,8,gpio::SALIDA,gpio::HIGH);
Timer t1(2000,handlr);
driverMotor m1(&motor);

int main(void) {



	m1.setVelocidad(5);
	m1.encenderMotor();
	t1.start();

	Inicializar();

	enum est_posibles {APAGADO, GIRANDO, FALLA, QUILOMBO};
	est_posibles estado = APAGADO;

	while(1)
	{
		switch(estado)
		{
		case APAGADO:
			/*
			if (pulsador.getPIN())
			{
				estado = GIRANDO;
				ledOK.setPIN();
			}
			*/
			break;

		case GIRANDO:
			/*
			if (motor.RPM != LCD.RPM)
				LCD.print(motor.RPM);
			if (motor.RPM != RPMesperadas)
			{
				estado = FALLA;

			}
			*/
			break;

		case FALLA:
			break;

		case QUILOMBO:
			break;

		default:
			estado = QUILOMBO;
			break;
		}
	}

    return 0 ;
}

void handlr()
{
	static uint8_t aux = 1;
	aux ++;
	if(aux>6)
		aux=0;

	m1.setVelocidad(aux);
	t1.start();
}

