#ifndef TEMPERATURA_H_
#define TEMPERATURA_H_

#include <LPC845.h>
#include <math.h>
#include <PerifericoTemporizado.h>
#include <AnalogInput.h>

#define CANT_DIVISIONES 100

class Temperatura : public PerifericoTemporizado {

protected:
	float valor_temperatura;
	    uint32_t Rfija;    	// Resistencia conocida para el divisor de voltaje
		uint32_t R0; //resistencia del ntc a 19.6°c (temp de muestra)
		float T0; //temp de referencia en kelvin
		float cuentas;		//resultado del main
		float beta; //constante beta del ntc
		float vref;	//tension de referencia del adc
		uint32_t adc_res; //resolucion del adc 10 bits
		uint8_t muestras; //numero de muestras

		AnalogInput A1;

public:
	Temperatura (uint8_t canal);
	float getTemperatura();	//°C
	void handler();

};



#endif /* TEMPERATURA_H_ */
