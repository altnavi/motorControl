/*
 * MdEMotor.h
 *
 *  Created on: 13 oct. 2025
 *      Author: Usuario
 */

#ifndef MDEMOTOR_H_
#define MDEMOTOR_H_

#include <LPC845.h>
#include <gpio.h>
#include <Timer.h>
#include <driverMotor.h>

void MdEMotor (void);
void InicializarMdE (void);


extern Timer RPMtimer;
extern Timer alarmatimer;
extern Timer actualizarDatos;		//usado para refrescar datos en pantalla

extern driverMotor motor1;

extern gpio botonArranque;
extern gpio botonParada;
extern gpio botonVelocidad;
//extern gpio botonVelocidad_mas;
//extern gpio botonVelocidad_menos;
extern gpio botonCambioSentido;
extern gpio cambioSentido;
//extern gpio pulso;
extern gpio led;
extern gpio alarma;

extern uint32_t velocidad;
extern bool sentido_giro;
extern bool flag_RPMtimer;
extern bool flag_alarmatimer;
extern bool flag_actualizarDatos;

#define DETENIDO			0
#define	GIRANDO				1
#define	FALLA_INTERMITENTE	2
#define	FALLA_CONTINUA		3



#endif /* MDEMOTOR_H_ */
