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
#include <gpio.h>
#include <driverMotor.h>
#include <Timer.h>
#include <detectorGiro.h>
#include <digitalInput.h>

void handlr();

gpio motor(0,8,gpio::SALIDA,gpio::HIGH);
gpio led(1,2,gpio::SALIDA,gpio::LOW);
gpio invertir_giro(0,6,gpio::SALIDA, gpio::HIGH);
Timer t1(2000,handlr);
driverMotor m1(&motor);
DetectorGiro dec(1,0,1,1);
uint32_t rpm = 0;
DigitalInput bot(0,4);

int main(void) {



	m1.setVelocidad(1);
	m1.encenderMotor();
	t1.start();

	Inicializar();

	while(1)
	{
		if(bot.getKey())
		{
			invertir_giro.togglePIN();
		}
    }

	return 0 ;
}

void handlr()
{
	rpm = dec.getRPM();
	uint8_t g = dec.getSentidoGiro();
	if (g == DetectorGiro::ANTIHORARIO)
		led.setPIN();
	else
		led.clrPIN();
	t1.start();
}

