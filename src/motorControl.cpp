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
#include <driverMotor.h>
#include <detectorGiro.h>
#include <MdEWifi.h>
#include <stdio.h>

void hand_envio_dats();

uint32_t rpm;

DigitalInput bot(0, 4);
gpio led_wifi_OK(1,0,gpio::SALIDA, gpio::LOW);
gpio led_wifi_conectado(1,1,gpio::SALIDA, gpio::LOW);
serialCom esp(1, 115200, 8, 9);
serialCom pc(0, 115200, 25, 24);
Timer led_wifi_inter(300, wifi_intermitente_handler);
Timer handler_envio_datos(1000, hand_envio_dats);

char buffer_envio[128]; // Buffer para crear los mensajes JSON
float temperatura_motor = 45.5;


int main(void) {
	Inicializar();
    Wifi_Init();
    handler_envio_datos.start();

    while(1) {
        Wifi_Manejar();
    }

    return 0;
}


void hand_envio_dats() //prueba envio datos
{
	sprintf(buffer_envio, "{\"rpm\": %d, \"temp\": %.1f}\r\n", rpm, temperatura_motor);

	if (!Wifi_EnviarDato(buffer_envio) ) {
		pc.Transmitir((uint8_t*)"WARN: WiFi ocupado, reintentando...\r\n", 37);
	}
	else
	{
		rpm++;
	}

	handler_envio_datos.start();
}

