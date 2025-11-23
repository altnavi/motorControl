/*
 * driverMotor.cpp
 *
 *  Created on: 19 sep. 2025
 *      Author: Iván
 */

#include <driverMotor.h>

driverMotor * p_motor = nullptr;

driverMotor::driverMotor(gpio _motor,gpio _sentido) :
t1(1,handler_motor),
t_sentido(200,handler_invertir),
gpio_motor(_motor)
{
	sentidoGiro = _sentido;
	p_motor=this;
	funcionando = false;

}

driverMotor::~driverMotor() {

}

void driverMotor::setVelocidad(uint8_t c) // de 0 a 100
{
apagarMotor();
gpio_motor.clrPIN();
	if(c>0)
	t1.setTime(c);
encenderMotor();
}

void driverMotor::encenderMotor()
{
	if(!funcionando)
	{
	funcionando = true;
	t1.start();
	}
}

void driverMotor::apagarMotor()
{
	if(funcionando)
	{
	t1.stop();
	gpio_motor.clrPIN();
	funcionando = false;
	}
}

void driverMotor::setSentido(bool sentido)
{
	if(funcionando)
	{
		apagarMotor();
		sentidoGiro.togglePIN();
		t_sentido.start();
	}
}

void handler_motor()
{
	p_motor->t1.start();
	p_motor->gpio_motor.togglePIN();
}

void handler_invertir()
{
	p_motor->encenderMotor();
}
