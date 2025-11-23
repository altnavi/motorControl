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
	alarma.clrPIN();

	sentido_giro = DetectorGiro::DETENIDO;
	motor1.setSentido(sentido_giro);

	buzzer_inter.stop();
	RPMtimer.stop();
	alarmatimer.stop();

	actualizarDatos.start();
}


void MdEMotor (void)
{
	static uint8_t estado = DETENID;
	static bool timer_activo = false;

	switch (estado)
	{
	case DETENID:
		if(flag_boton_inicio)
		{
			motor1.setSentido(sentido_giro);
			motor1.encenderMotor();

			RPMtimer.start();
			timer_activo = true;

			flag_boton_inicio = false;
			estado = GIRANDO;
		}
		break;

	case GIRANDO:
		if(flag_boton_parada == true)
		{
			motor1.apagarMotor();
			RPMtimer.stop();
			timer_activo = false;
			flag_boton_parada = false;
			estado = DETENID;
		}
		else if(flag_velocidad)
		{
			motor1.setVelocidad(velocidad);
			flag_velocidad = false;
			estado = GIRANDO;
		}
		else if(flag_sentido)
		{
			if(sentido_giro == DetectorGiro::HORARIO)
				motor1.setSentido(DetectorGiro::ANTIHORARIO);
			else
				motor1.setSentido(DetectorGiro::HORARIO);

			motor1.setSentido(sentido_giro);
			flag_sentido = false;
			estado = GIRANDO;
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
			estado = FALLA_INTERM;
		}
		break;

	case FALLA_INTERM:
		if(flag_boton_parada)
		{
			flag_boton_parada = false;
			motor1.apagarMotor();
			alarmatimer.stop();
			buzzer_inter.stop();
			alarma.clrPIN();
			estado = DETENID;
		}
		else if(rpm > 0)
		{
			alarmatimer.stop();
			buzzer_inter.stop();
			alarma.clrPIN();

			timer_activo = false;
			flag_RPMtimer = false;

			estado = GIRANDO;
		}

		else if(flag_alarmatimer)
		{
			alarma.setPIN();
			alarmatimer.stop();
			buzzer_inter.stop();
			motor1.apagarMotor();
			estado = FALLA_CONT;
		}
		break;

	case FALLA_CONT:
		if(flag_boton_parada)
		{
			alarma.clrPIN();
			flag_boton_parada = false;
			flag_alarmatimer = false;
			estado = DETENID;
		}
		break;

	default:
		motor1.apagarMotor();
		alarma.clrPIN();

		sentido_giro = DetectorGiro::DETENIDO;
		motor1.setSentido(sentido_giro);

		buzzer_inter.stop();
		RPMtimer.stop();
		alarmatimer.stop();

		actualizarDatos.start();
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
