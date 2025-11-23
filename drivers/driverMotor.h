/*
 * driverMotor.h
 *
 *  Created on: 19 sep. 2025
 *      Author: Iván
 */

#ifndef DRIVERMOTOR_H_
#define DRIVERMOTOR_H_
#include <LPC845.h>
#include <detectorGiro.h>
#include <gpio.h>

void handler_motor();
void handler_invertir();

class driverMotor {
private:

	Timer t1; //timer para generar los pulsos al driver del motor
	Timer t_sentido; //para frenar el motor e invertir el giro
	gpio gpio_motor;
	gpio sentidoGiro;
	bool funcionando;

public:

	driverMotor(gpio _motor,gpio _sentido);
	void setSentido(bool sentido);
	void setVelocidad(uint8_t);
	void encenderMotor();
	void apagarMotor();
	friend void handler_motor();
	friend void handler_invertir();
	virtual ~driverMotor();

};

#endif /* DRIVERMOTOR_H_ */
