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

class driverMotor {
private:
	Timer t1;
	gpio *motor;
public:
	driverMotor(gpio*);
	void setSentido(bool sentido);
	void setVelocidad(uint8_t);
	void encenderMotor();
	void apagarMotor();
	friend void handler_motor();
	virtual ~driverMotor();

};

#endif /* DRIVERMOTOR_H_ */
