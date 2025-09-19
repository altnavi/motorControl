/*
 * GPIO.h
 *
 *  Created on: 2 abr. 2023
 *      Author: chor
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <LPC845.h>

class gpio {

	//En primer lugar declaro todos los atributos de la clase, como protegidos
	//en caso que quiera heredar posteriormente de la clase:
	protected:

		uint8_t puerto;
		uint8_t pin;
		uint8_t direction;
		uint8_t actividad;

	public:

	//Esta matriz la declaro para poder acceder más fácilmente a los registro PIO:
	//Los mismos están ordenados en memoria de una manera "poco feliz" lo que hace
	//o sea que no vienen en el orden que uno se esperaría (primero puerto 0 pin 0,
	//luego 1, etc. sino que vienen en cualquier orden. En esta matriz pongo
	//los indices, pudiendo entrar a la matriz con el [puerto][pin], y
	//obteniendo como resultado el índice del registro PIO al que tengo que acceder
	//para configurar dicho puerto, pin. Se usa en los métodos setMODE
	static const uint8_t iocon_index[2][32];

	//Diferentes enum para los diferentes métodos. Hacen más legible el código
	enum modo_in {MODE_NONE = 0, MODE_PULLUP, MODE_PULLDOWN, MODE_REPEAT };
	enum modo_out {MODE_OD_ON = 0, MODE_OD_OFF };
	enum dir { ENTRADA = 0, SALIDA };
	enum activity {LOW = 0 , HIGH };

	//Constructor parametrizado y de copia
	gpio(uint8_t _puerto , uint8_t _pin , uint8_t dir , uint8_t act = HIGH );
	gpio (const gpio &);
	gpio();

	//Métodos para configurar las entradas y salidas, y para definir
	//si el gpio se comportará de uno u otro modo:
	void setModeIN ( uint8_t mode_in = MODE_NONE);
	void setModeOUT ( uint8_t mode_in = MODE_OD_OFF);

	void setDIR ( uint8_t dir);

	//Métodos para leer y escribir los pines:
	//getPIN() devuelve el estado de actividad
	bool getPIN();
	//setPIN(uint8_t) pone un valor (0 o 1) en el pin
	void setPIN(uint8_t val);
	//setPIN() y clrPIN() modifican el valor del pin usando el estado de
	//actividad que fue configurado en el objeto (si es activo en bajo setPIN
	//pone un 0 y clrPIN un 1, y si es activo en alto viceversa)
	void setPIN();
	void clrPIN();
	//togglePIN cambia el valor de la salida (0 a 1 o 1 a 0)
	void togglePIN();

	//Sobrecargo el operador = para igualar con otro objeto:
	gpio & operator = (const gpio &);

	//Sobrecargo los operadores == e = para leer el valor de un pin o
	//para setear un valor en el pin
	gpio & operator = (uint8_t );
	bool operator == (uint8_t );

};

#endif /* GPIO_H_ */
