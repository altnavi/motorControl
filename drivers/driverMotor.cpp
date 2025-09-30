/*
 * driverMotor.cpp
 *
 *  Created on: 19 sep. 2025
 *      Author: Iván
 */

#include <driverMotor.h>

driverMotor::setSentido(bool sentido)
{
	setRPM(0);
	if (sensor.getRPM() == 0)
		reset->clrPIN();
}

driverMotor::driverMotor(uint8_t _port1, uint8_t _pin1, uint8_t _port2, uint8_t _pin2, gpio)
:sense(_port1, _pin1, _port2, _pin2)
{


}

driverMotor::~driverMotor() {

}
