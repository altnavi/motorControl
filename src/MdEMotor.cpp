/*
 * MdEMotor.cpp
 *
 * Created on: 13 oct. 2025
 * Author: Usuario
 */

#include <MdEMotor.h>

void InicializarMdE (void)
{
	motor1.apagarMotor();
	led.clrPIN();
	alarma.clrPIN();

	sentido_giro = DetectorGiro::DETENIDO;
	motor1.setSentido(sentido_giro);


	RPMtimer.stop();
	alarmatimer.stop();


	actualizarDatos.start();
}

static uint8_t estadoski = DETENID;

void MdEMotor (void)
{
	static bool timer_activo = false;

	switch (estadoski)
	{
	case DETENID:
		if(flag_velocidad)
		{
			motor1.setVelocidad(velocidad);
			flag_velocidad = false;
		}

		if(flag_boton_inicio)
		{
			motor1.setSentido(sentido_giro);
			motor1.encenderMotor();

			RPMtimer.start();
			timer_activo = true;

			flag_boton_inicio = false;
			estadoski = GIRANDO;
		}
		break;

	case GIRANDO:
		if(flag_boton_parada == true)
		{
			motor1.apagarMotor();
			RPMtimer.stop();
			timer_activo = false;
			flag_boton_parada = false;
			estadoski = DETENID;
		}
		else if(flag_velocidad)
		{
			motor1.setVelocidad(velocidad);
			flag_velocidad = false;
		}
		else if(flag_sentido)
		{
			if(sentido_giro == DetectorGiro::HORARIO)
				motor1.setSentido(DetectorGiro::ANTIHORARIO);
			else
				motor1.setSentido(DetectorGiro::HORARIO);

			motor1.setSentido(sentido_giro);
			flag_sentido = false;
			estadoski = GIRANDO;
		}

		else if(rpm > 0)
		{
			if(timer_activo) {
				RPMtimer.stop();
				timer_activo = false;
			}
			flag_RPMtimer = false;
		}
		else // rpm == 0
		{
			if(!timer_activo) {
				RPMtimer.start();
				timer_activo = true;
			}
		}

		if(flag_RPMtimer)
		{
			alarmatimer.start();
			buzzer_inter.start();
			flag_RPMtimer = false;
			estadoski = FALLA_INTERMITENTE;
		}
		break;

	case FALLA_INTERMITENTE:

		if(flag_boton_parada)
		{
			flag_boton_parada = false;
			motor1.apagarMotor();
			alarmatimer.stop();
			buzzer_inter.stop();
			led.clrPIN();
			alarma.clrPIN();
			estadoski = DETENID;
		}
		else if(rpm > 0)
		{
			alarmatimer.stop();
			buzzer_inter.stop();
			led.clrPIN();
			alarma.clrPIN();

			timer_activo = false;
			flag_RPMtimer = false;

			estadoski = GIRANDO;
		}

		else if(flag_alarmatimer)
		{
			led.setPIN();
			alarma.setPIN();
			alarmatimer.stop();
			buzzer_inter.stop();
			motor1.apagarMotor();
			estadoski = FALLA_CONTINUA;
		}
		break;

	case FALLA_CONTINUA:
		if(flag_boton_parada)
		{
			led.clrPIN();
			alarma.clrPIN();
			flag_boton_parada = false;
			flag_alarmatimer = false;
			estadoski = DETENID;
		}
		break;

	default:

		break;
	}
}

void handler_RPMtimer()
{
    flag_RPMtimer = true;
}

void handler_alarmatimer()
{
	rpm = d1.getRPM();

	if(rpm==0)
    flag_alarmatimer = true;
}

void handler_actualizarDatos()
{
    rpm = d1.getRPM();
    temperatura_motor = t1.getTemperatura();
    sentido_giro = d1.getSentidoGiro();
    actualizarDatos.start();
}

void buzzer_intermitente()
{
	alarma.togglePIN();
	buzzer_inter.start();
}
