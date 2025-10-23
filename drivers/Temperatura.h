#ifndef TEMPERATURA_H_
#define TEMPERATURA_H_

#include <LPC845.h>
#include <PerifericoTemporizado.h>

#define CANT_DIVISIONES 100

class Temperatura : public PerifericoTemporizado {

protected:
	float valor_temperatura;
	uint32_t Rfija_kOhm = 10;   // Resistencia conocida para el divisor de voltaje
	const float Vref = 3.3;
	float cuentas;			//Resultado del main
	float Rterm_kOhm;

	struct tabla_base {
	    float min;
	    float max;
	    uint32_t valor;
	};



public:
	Temperatura ();
	uint32_t getTemperatura();	//°C
	void handler();

	tabla_base tabla[CANT_DIVISIONES];

};



#endif /* TEMPERATURA_H_ */
