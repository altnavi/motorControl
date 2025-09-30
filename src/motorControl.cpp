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

int main(void) {

	gpio motor(1,1,1,1);
	gpio sensor1(1,1,1,1);
	gpio sensor2(1,1,1,1);

	driverMotor(1,1,1,1, motor);

	Inicializar();

	enum est_posibles {APAGADO, GIRANDO, FALLA, QUILOMBO};
	est_posibles estado = APAGADO;

	while(1)
	{
		switch(estado)
		{
		case APAGADO:
			if (pulsador.getPIN())
			{
				estado = GIRANDO;
				ledOK.setPIN();
			}
			break;

		case GIRANDO:
			if (motor.RPM != LCD.RPM)
				LCD.print(motor.RPM);
			if (motor.RPM != RPMesperadas)
			{
				estado = FALLA;

			}
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
