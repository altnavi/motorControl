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
#include <detectorGiro.h>
#include <Temperatura.h>

void MdEMotor (void);
void InicializarMdE (void);
void handler_RPMtimer();
void handler_alarmatimer();
void handler_actualizarDatos();
void buzzer_intermitente();

extern Timer RPMtimer;
extern Timer alarmatimer;
extern Timer actualizarDatos;        // usado para refrescar datos en pantalla
extern Timer buzzer_inter;

extern driverMotor motor1;
extern DetectorGiro d1;
extern Temperatura t1;

extern gpio cambioSentido;
extern gpio alarma;

extern uint32_t velocidad;

extern uint8_t sentido_giro;       // Orden de control
extern uint8_t sentido_detectado;  // Lo que detecta el sensor

extern uint32_t rpm;
extern float temperatura_motor;

// Flags generados por la MDE WiFi o timers (NO ISR)
extern bool flag_RPMtimer;
extern bool flag_alarmatimer;
extern bool flag_sentido;
extern bool flag_boton_inicio;
extern bool flag_boton_parada;
extern bool flag_velocidad;

// Estados
#define DETENID             0
#define GIRANDO             1
#define FALLA_INTERM 		2
#define FALLA_CONT      	3

#endif /* MDEMOTOR_H_ */

