/*
 * AnalogInput.h
 *
 *  Created on: 31 oct. 2023
 *      Author: chor
 */

#ifndef ANALOGINPUT_H_
#define ANALOGINPUT_H_

#if defined (__cplusplus)
extern "C" {

void ADC_SEQA_IRQHandler ( void );

}
#endif

#include <LPC845.h>


class AnalogInput {

	uint32_t *vectorCuentas;
	uint8_t tam_vector;

	uint8_t canal;

	void nuevaLectura( uint32_t );

public:


	static uint8_t cant_entradas;

	AnalogInput( uint8_t _canal = 0 , uint32_t frec = 10000 , uint32_t cant_muestras = 10);

	uint32_t getValor();
	uint8_t getCanal();

	friend void ADC_SEQA_IRQHandler ( void );

};

extern AnalogInput **entradasAnalogicas;


#endif /* ANALOGINPUT_H_ */
