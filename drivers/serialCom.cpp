#include "serialCom.h"
#include <string.h>

serialCom* serialCom::uarts[4] = {nullptr, nullptr, nullptr, nullptr};

serialCom::serialCom(uint8_t numUart, uint32_t baudR, uint8_t pinTx, uint8_t pinRx)
{
    n_uart = numUart;
    baudrate = baudR;

    // Guardar puntero estático
    if(n_uart < 4)
        uarts[n_uart] = this;
    else
        return; // UART inválida, no hacer nada

    // Habilitar reloj UART
    uint8_t shift = 14 + n_uart;
    SYSCON->SYSAHBCLKCTRL0 |= (1 << shift);

    // Reset
    SYSCON->PRESETCTRL0 &= ~(1<<shift);
    SYSCON->PRESETCTRL0 |= (1<<shift);

    // Habilitar SWM
    SYSCON->SYSAHBCLKCTRL0 |= (1<<7);

    // Configurar Switch Matrix y puntero UART
    switch(n_uart)
    {
        case 0:
            PINASSIGN0 &= ~(0x0000FFFF);
            PINASSIGN0 |= (pinTx << 0) | (pinRx << 8);
            dir_uart = USART0;
            break;
        case 1:
            PINASSIGN1 &= ~(0x00FFFF00);
            PINASSIGN1 |= (pinTx << 8) | (pinRx << 16);
            dir_uart = USART1;
            break;
        case 2:
            PINASSIGN2 &= ~(0xFFFF0000);
            PINASSIGN2 |= (pinTx << 16) | (pinRx << 24);
            dir_uart = USART2;
            break;
        case 3:
            PINASSIGN11 &= ~(0xFF000000);
            PINASSIGN12 &= ~(0x000000FF);
            PINASSIGN11 |= (pinTx << 24);
            PINASSIGN12 |= pinRx;
            dir_uart = USART3;
            break;
    }

    if(dir_uart == nullptr)
        return;

    // Configuración 8N1 sin flow
    dir_uart->CFG = (1<<2); // 8 bits, async, no parity, 1 stop

    // Habilitar interrupción RX
    dir_uart->INTENSET = (1<<0);

    // Configurar baudrate
    if(n_uart == 0) UART0CLKSEL = 0;
    else if(n_uart == 1) UART1CLKSEL = 0;
    else if(n_uart == 2) UART2CLKSEL = 0;
    else if(n_uart == 3) UART3CLKSEL = 0;

    dir_uart->BRG = (30000000 / (baudrate*16));

    // Habilitar NVIC
    shift = 3 + n_uart;
    ISER0 |= (1<<shift);

    // Enable UART
    dir_uart->CFG |= 1;
}

// ----------------- Transmisión -----------------
void serialCom::Transmitir(uint8_t dato)
{
    if(dir_uart == nullptr) return;

    uint8_t next = (idxTxWrite + 1) % TX_BUFFER_SIZE;
    if(next == idxTxRead) return; // buffer lleno

    bufTx[idxTxWrite] = dato;
    idxTxWrite = next;

    // Habilitar interrupción TX
    dir_uart->INTENSET |= (1<<2);
}

void serialCom::Transmitir(uint8_t* buf, uint8_t cant)
{
    if(dir_uart == nullptr) return;
    for(uint8_t i=0; i<cant; i++)
        Transmitir(buf[i]);
}

// ----------------- Recepción -----------------
int16_t serialCom::Recibir()
{
    if(dir_uart == nullptr) return NO_DATA;
    if(idxRxRead == idxRxWrite) return NO_DATA; // vacío

    uint8_t dato = bufRx[idxRxRead];
    idxRxRead = (idxRxRead + 1) % RX_BUFFER_SIZE;
    return dato;
}

// ----------------- Handler IRQ -----------------
void serialCom::handlerIRQ()
{
    if(dir_uart == nullptr) return;

    uint32_t stat = dir_uart->INTSTAT;

    // RX
    if(stat & (1<<0))
    {
        while(dir_uart->STAT & (1<<0))
        {
            uint8_t d = dir_uart->RXDAT;
            uint8_t next = (idxRxWrite + 1) % RX_BUFFER_SIZE;
            if(next != idxRxRead) // evitar overflow
                bufRx[idxRxWrite] = d, idxRxWrite = next;
        }
    }

    // TX
    if(stat & (1<<2))
    {
        if(idxTxRead != idxTxWrite)
        {
            dir_uart->TXDAT = bufTx[idxTxRead];
            idxTxRead = (idxTxRead + 1) % TX_BUFFER_SIZE;
        }
        else
        {
            dir_uart->INTENCLR |= (1<<2);
        }
    }
}

// ----------------- IRQ Handlers -----------------
extern "C" {
void UART0_IRQHandler(void){ if(serialCom::uarts[0]) serialCom::uarts[0]->handlerIRQ(); }
void UART1_IRQHandler(void){ if(serialCom::uarts[1]) serialCom::uarts[1]->handlerIRQ(); }
void UART2_IRQHandler(void){ if(serialCom::uarts[2]) serialCom::uarts[2]->handlerIRQ(); }
void UART3_IRQHandler(void){ if(serialCom::uarts[3]) serialCom::uarts[3]->handlerIRQ(); }
}









/*
 * serialCom.cpp
 *
 *  Created on: 10 sep. 2023
 *      Author: chor
 */

/*
#include "serialCom.h"

serialCom ** uarts = new serialCom*[4];


serialCom::serialCom( uint8_t numUart , uint32_t baudR , uint8_t pinTx , uint8_t pinRx )
{
	//Genero un buffer para la transmision:
	bufTx = new uint8_t[20];
	enviando = false;
	datoNuevo = false;
	indiceTx = 0;
	cantBytes = 0;

	n_uart = numUart;
	uarts[n_uart] = this;

	baudrate = baudR;

	// HABILITO LA UART:
	uint8_t shift = 14 + n_uart;
	SYSCON->SYSAHBCLKCTRL0 |= (1 << shift);

	//Reseteo la UART:
	SYSCON->PRESETCTRL0 &= ~(1<<shift);
	SYSCON->PRESETCTRL0 |= (1<<shift);

	//Habilito el CLK a la SWM:
	SYSCON->SYSAHBCLKCTRL0 |= (1<<7);

	switch ( n_uart)
	{
		case 0:
			// CONFIGURO LA SWITCH MATRIX
			PINASSIGN0 &= ~(0x0000FFFF);
			PINASSIGN0 |= (pinTx << 0) | (pinRx << 8);
			//Seteo la direccion de la uart a configurar
			dir_uart = USART0;
			break;
		case 1:
			// CONFIGURO LA SWITCH MATRIX
			PINASSIGN1 &= ~(0x00FFFF00);
			PINASSIGN1 |= (pinTx << 8) | (pinRx << 16);
			//Seteo la direccion de la uart a configurar
			dir_uart = USART1;
			break;
		case 2:
			// CONFIGURO LA SWITCH MATRIX
			PINASSIGN2 &= ~(0xFFFF0000);
			PINASSIGN2 |= (pinTx << 16) | (pinRx << 24);
			//Seteo la direccion de la uart a configurar
			dir_uart = USART2;
			break;

		case 3:
			// CONFIGURO LA SWITCH MATRIX
			PINASSIGN11 &= ~(0xFF000000);
			PINASSIGN12 &= ~(0x000000FF);
			PINASSIGN11 |= (pinTx << 24);
			PINASSIGN12 |= pinRx;
			//Seteo la direccion de la uart a configurar
			dir_uart = USART3;
			break;
	}

	// CONFIGURACION GENERAL
	dir_uart->CFG = 	(0 << 0)		// 0=DISABLE 1=ENABLE
						| (1 << 2)		// 0=7BITS 1=8BITS 2=9BITS
						| (0 << 4)		// 0=NOPARITY 2=PAR 3=IMPAR
						| (0 << 6)		// 0=1BITSTOP 1=2BITSTOP
						| (0 << 9)		// 0=NOFLOWCONTROL 1=FLOWCONTROL
						| (0 << 11);	// 0=ASINCRONICA 1=SINCRONICA

	// CONFIGURACION INTERRUPCIONES
	dir_uart->INTENSET = (1 << 0);	//RX

	// CONFIGURACION DEL BAUDRATE
	//CLK = FRO = 30MHz
	if ( n_uart == 0 )
		UART0CLKSEL = 0;
	else if ( n_uart == 1 )
		UART1CLKSEL = 0;
	else if ( n_uart == 2 )
		UART2CLKSEL = 0;
	else if ( n_uart == 3 )
		UART3CLKSEL = 0;

	dir_uart->BRG = (30000000 / (baudrate* 16));

	//Habilito interrupcion USART0 en el NVIC
	shift = 3 + n_uart;
	ISER0 |= (1 << shift);

	// ENABLE LA UART
	dir_uart->CFG |= 1;
}

serialCom::~serialCom()
{
	delete[] bufTx;
}

uint32_t serialCom::getBaudR()
{
	return baudrate;

}

void serialCom::setBaudR ( uint32_t br)
{
	baudrate = br;
	//Reconfiguro Baudrate
	dir_uart->BRG = (30000000 / (baudrate* 16));
}

void serialCom::Transmitir ( uint8_t dato )
{

	//Si ya estaba enviando un dato lo agrego:
	if ( enviando )
	{
			bufTx[cantBytes] = dato;
			cantBytes++;
	}
	else
	{
		//pongo el nuevo dato para enviar:
		bufTx[0] = dato;
		cantBytes = 1;
		enviando = true;
	}

	//Transmito el dato por interrupción
	dir_uart->INTENSET |= 1<<2;
}

void serialCom::Transmitir ( uint8_t *buf , uint8_t cant )
{

	int i = 0;

	for ( ; i < cant ; i++ )
		bufTx[i] = buf[i];

	cantBytes = i;
	indiceTx = 0;

	enviando = true;

	//Transmito el dato por interrupción
	dir_uart->INTENSET |= 1<<2;
}


int16_t serialCom::Recibir ()
{
	int16_t retorno = NO_DATA;

	if ( datoNuevo )
	{
		retorno = byteRx;
		datoNuevo = false;
	}

	return retorno;
}

void serialCom::handlerIRQ ( void )
{
	uint32_t stat = dir_uart->INTSTAT;
	int16_t dato;

    // CASO RECEPCION
	if(stat & (1 << 0))
	{
		 while (dir_uart->STAT & (1 << 0))  // RXRDY
		    {
		        byteRx = dir_uart->RXDAT;
		        datoNuevo = true;
		        // ⚠️ Si usás buffer circular, este es el lugar donde hacés PushByte(byteRx)
		    }
	}

	//CASO TRANSMISION
	else if(stat & (1 << 2))
	{
		dato = bufTx[indiceTx];
		dir_uart->TXDAT = dato;
		indiceTx++;

		if ( indiceTx == cantBytes )
		{
			indiceTx = 0;
			dir_uart->INTENCLR |= 1<<2;
			enviando = false;
		}
	}

	//CASO ERRORES
	else
	{

	}

}

//Funciones de interrupcion:
void UART0_IRQHandler (void)
{
	uarts[0]->handlerIRQ();
}

void UART1_IRQHandler (void)
{
	uarts[1]->handlerIRQ();
}

void UART2_IRQHandler (void)
{
	uarts[2]->handlerIRQ();
}

void UART3_IRQHandler (void)
{
	uarts[3]->handlerIRQ();
}
*/
