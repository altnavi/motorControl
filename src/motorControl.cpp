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

	while(1)
	{

	}

    return 0 ;
}

