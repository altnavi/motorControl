/*
 * serialCom.h
 *
 *  Created on: 10 sep. 2023
 *      Author: chor
 */

#ifndef SERIALCOM_H_
#define SERIALCOM_H_

#include <LPC845.h>

#if defined (__cplusplus)
extern "C" {

void UART0_IRQHandler (void);
void UART1_IRQHandler (void);
void UART2_IRQHandler (void);
void UART3_IRQHandler (void);

}
#endif


class serialCom {

	uint8_t * bufTx;
	uint8_t byteRx;
	bool datoNuevo;
	bool enviando;

	uint8_t indiceTx,cantBytes;

	uint32_t baudrate;
	uint8_t n_uart;

	//Uso este puntero para apuntar a la uart que voy a configurar:
	USARTs * dir_uart;

public:
	static const int16_t NO_DATA = -1;

	//Uso esta variable para ir creando una nueva uart por cada instancia, podría pasar el nro de uart al constructor
	static uint8_t cant_uarts;

	//Por defecto el pin para transmision es el P025 y el de recepcion P0.24:
	serialCom( uint8_t numUart = 0 , uint32_t baudR = 9600 , uint8_t pinTx = 25 , uint8_t pinRx = 24 );
	~serialCom();

	//Leer y configurar la velocidad de comunicacion:
	uint32_t getBaudR();
	void setBaudR ( uint32_t );

	//Transmitir o recibir un byte por el puerto
	void Transmitir ( uint8_t );
	void Transmitir ( uint8_t *, uint8_t );
	int16_t Recibir ();

	//Funcion que se invoca en el handler de las IRQ:
	void handlerIRQ ( void );
};

#endif /* SERIALCOM_H_ */
