/*
 * entradaFiltrada.h
 *
 *  Created on: 28 jun. 2023
 *      Author: chor
 */

#ifndef DIGITALINPUT_H_
#define DIGITALINPUT_H_

#include <gpio.h>
#include <PerifericoTemporizado.h>
#include <LPC845.h>

class DigitalInput  : public PerifericoTemporizado, public gpio {

	uint8_t buffer_key;
	const uint8_t cant_rebotes = 5;
	uint8_t contador,valor_anterior;

public:

	enum estados {DESACTIVADA = 0 , ACTIVADA };

	DigitalInput(uint8_t port , uint8_t _pin , uint8_t actividad_on = 0 );

	uint8_t getKey();
	void handler ();
};



#endif /* DIGITALINPUT_H_ */
