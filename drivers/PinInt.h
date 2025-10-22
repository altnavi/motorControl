/*
 * PinInt.h
 *
 *  Created on: 5 jun. 2023
 *      Author: chor
 */

#ifndef PININT_H_
#define PININT_H_

#include "LPC845.h"

//Tengo que agregar esto siempre que use funciones del vector de interrupciones:
#if defined (__cplusplus)
extern "C" {

void PININT0_IRQHandler (void);
void PININT1_IRQHandler (void);
void PININT2_IRQHandler (void);
void PININT3_IRQHandler (void);
void PININT4_IRQHandler (void);
void PININT5_IRQHandler (void);
void PININT6_IRQHandler (void);
void PININT7_IRQHandler (void);
}
#endif


class PinInt {

	public:
		enum tipos_trigger {none , flanco_asc, flanco_desc, ambos_flancos, nivel_alto, nivel_bajo};

	private:

		uint8_t fuente;
		tipos_trigger trigger;
		void (*handler) (void);

		static uint8_t cant_pinInt;

	public:

		PinInt( uint8_t port , uint8_t pin , tipos_trigger trig , void (*h) (void) );
		~PinInt();

		void configIntExt ();
		void deshabilitarINT();
		void habilitarINT();

		friend void PININT0_IRQHandler (void);
		friend void PININT1_IRQHandler (void);
		friend void PININT2_IRQHandler (void);
		friend void PININT3_IRQHandler (void);
		friend void PININT4_IRQHandler (void);
		friend void PININT5_IRQHandler (void);
		friend void PININT6_IRQHandler (void);
		friend void PININT7_IRQHandler (void);

};

#endif /* PININT_H_ */
