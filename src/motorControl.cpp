/*
╭──╦──╮
│IC║XC│
╠══╬══╣
│NI║KA│
╰──╩──╯
 */
#include <cr_section_macros.h>
#include <Init.h>
#include <LPC845.h>
#include <DigitalInput.h>
#include <serialCom.h>
#include "../drivers/detectorGiro.h"

uint8_t respuestaOK[] = {"LED N ENCENDIDO"};
uint8_t bytesOK = 15;
uint8_t respuestaERROR[] = {"ERROR"};
uint8_t bytesERROR = 5;

int main(void) {

	detectorGiro jose(0,1);

	Inicializar();

	serialCom comPC ( 1 , 115200 ,  25 , 24 );

	gpio ledG(1,0,gpio::SALIDA,gpio::LOW);
	gpio ledB(1,1,gpio::SALIDA,gpio::LOW);
	gpio ledR(1,2,gpio::SALIDA,gpio::LOW);

	ledG.clrPIN();
	ledB.clrPIN();
	ledR.clrPIN();

	int16_t dato;

	while(1) {

		dato = comPC.Recibir();

		switch ( dato )
		{
		case 'r':

			ledG.clrPIN();
			ledB.clrPIN();
			ledR.setPIN();
			respuestaOK[4] = 'R';
			comPC.Transmitir(respuestaOK,bytesOK);
			comPC.Transmitir(0x0D);
			comPC.Transmitir(0x0A);
			break;

		case 'g':

			ledG.setPIN();
			ledB.clrPIN();
			ledR.clrPIN();
			respuestaOK[4] = 'G';
			comPC.Transmitir(respuestaOK,bytesOK);
			comPC.Transmitir(0x0D);
			comPC.Transmitir(0x0A);
			break;

		case 'b':

			ledG.clrPIN();
			ledB.setPIN();
			ledR.clrPIN();
			respuestaOK[4] = 'B';
			comPC.Transmitir(respuestaOK,bytesOK);
			comPC.Transmitir(0x0D);
			comPC.Transmitir(0x0A);
			break;

		case -1:
			//En este caso no hago nada
			break;

		default:
			//Llego un dato distinto de los válidos:
			comPC.Transmitir(respuestaERROR,bytesERROR);
			comPC.Transmitir(0x0D);
			comPC.Transmitir(0x0A);
			break;
		}

	}
    return 0 ;
}

