/*
 * driverMotor.cpp
 *
 *  Created on: 19 sep. 2025
 *      Author: Iván
 */

#include <driverMotor.h>

driverMotor * p_motor = nullptr;

driverMotor::driverMotor(gpio*gp) :
t1(1,handler_motor),
motor(gp)
{
	p_motor=this;
}

driverMotor::~driverMotor() {
	// TODO Auto-generated destructor stub
}

void driverMotor::setVelocidad(uint8_t c) // de 0 a 100
{
apagarMotor();
motor->clrPIN();
	if(c>0)
	t1.setTime(c);
encenderMotor();
}

void driverMotor::encenderMotor()
{
	t1.start();
}

void driverMotor::apagarMotor()
{
	t1.stop();
}

void driverMotor::setSentido(bool sentido)
{

}

void handler_motor()
{
	p_motor->t1.start();
	p_motor->motor->togglePIN();
}
