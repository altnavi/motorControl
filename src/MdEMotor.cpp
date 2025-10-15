/*
 * MdEMotor.cpp
 *
 *  Created on: 13 oct. 2025
 *      Author: Usuario
 */

#include <MdEMotor.h>


void InicializarMdE (void)
{
	motor1.apagarMotor();
	led.clrPIN();
	alarma.clrPIN();

	RPMtimer.stop();
	alarmatimer.stop();
	actualizarDatos.start();

}


void MdEMotor (void)
{
	static uint8_t estado = DETENIDO;

	switch (estado)
	{
	case DETENIDO:
		if(botonVelocidad.getPIN())
		{
			motor1.setVelocidad(velocidad);
			estado = DETENIDO;
		}
		else if(botonCambioSentido.getPIN())
		{
			motor1.setSentido(sentido_giro);
			estado = DETENIDO;
		}
		else if(botonArranque.getPIN())
		{
			motor1.encenderMotor();
			RPMtimer.start();
			estado = GIRANDO;
		}
		break;

	case GIRANDO:
		if(botonParada.getPIN())
		{
			motor1.apagarMotor();
			estado = DETENIDO;
		}
		else if(botonVelocidad.getPIN())
		{
			motor1.setVelocidad(velocidad);
			estado = GIRANDO;
		}

		else if(flag_RPMtimer && velocidad==0)
		{
//			led.parpadeo();
//			alarma.parpadeo();
			alarmatimer.start();
			RPMtimer.stop();
			flag_RPMtimer = false;
			estado = FALLA_INTERMITENTE;
		}
		break;

	case FALLA_INTERMITENTE:
		if(botonParada.getPIN())
		{
			motor1.apagarMotor();
			estado = DETENIDO;
		}
		else if(flag_alarmatimer && velocidad==0)
		{
			led.setPIN();
			alarma.setPIN();
			alarmatimer.stop();
			flag_alarmatimer = false;
			estado = FALLA_CONTINUA;
		}
		break;

	case FALLA_CONTINUA:
		if(botonParada.getPIN())
		{
			motor1.apagarMotor();
			led.clrPIN();
			alarma.clrPIN();
			estado = DETENIDO;
		}
		break;

	default:
		estado = DETENIDO;
		motor1.apagarMotor();
		led.clrPIN();
		alarma.clrPIN();
		RPMtimer.stop();
		alarmatimer.stop();
		actualizarDatos.start();

	}
}


