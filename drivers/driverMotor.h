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

class driverMotor {
private:
	DetectorGiro sense;
	uint32_t rpm;
	uint8_t port1;
public:
	driverMotor(uint8_t _port1, uint8_t _pin1, uint8_t _port2, uint8_t _pin2, gpio);
	void setRPM(uint8_t);
	void setSentido(bool);
	void apagar();
	void encender();

	virtual ~driverMotor();

};

#endif /* DRIVERMOTOR_H_ */
