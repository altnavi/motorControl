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
#include <MdEMotor.h>
#include <Timer.h>

void actualizarGUI();

#define gpio_botonArranque 0,0
#define gpio_botonParada 0,0
#define gpio_botonVelocidad 0,0
#define gpio_botonCambioSentido 0,0
#define gpio_cambioSentido 0,0
#define gpio_led_falla 0,0
#define gpio_alarma 0,0
#define gpio_motor 0,0

//  GPIOS
gpio led_wifi_OK(1,0,gpio::SALIDA, gpio::LOW);
gpio led_wifi_conectado(1,1,gpio::SALIDA, gpio::LOW);
gpio botonArranque(gpio_botonArranque,gpio::ENTRADA, gpio::LOW);
gpio botonParada(gpio_botonParada,gpio::ENTRADA, gpio::LOW);
gpio botonVelocidad(gpio_botonVelocidad,gpio::ENTRADA, gpio::LOW);
gpio botonCambioSentido(gpio_botonCambioSentido,gpio::ENTRADA, gpio::LOW);
gpio cambioSentido(gpio_cambioSentido,gpio::SALIDA, gpio::HIGH);
gpio led(gpio_led_falla,gpio::SALIDA, gpio::HIGH);
gpio alarma(gpio_alarma,gpio::SALIDA, gpio::HIGH);
gpio motor(gpio_motor,gpio::SALIDA, gpio::HIGH);
//BOTONES
DigitalInput bot(0, 4);
//COMUNICACION SERIE
serialCom esp(1, 115200, 8, 9);
serialCom pc(0, 115200, 25, 24);
//TIMERS
Timer led_wifi_inter(300, wifi_intermitente_handler);
Timer handler_envio_datos(1000, actualizarGUI);

Timer RPMtimer(1000,handler_RPMtimer);
Timer alarmatimer(1000,handler_alarmatimer);
Timer actualizarDatos(1000,handler_actualizarDatos);
//VARIABLES
uint32_t rpm;
char buffer_envio[128]; // Buffer para crear los mensajes JSON
float temperatura_motor = 45.5;

uint32_t velocidad;
bool sentido_giro = false;
bool flag_RPMtimer;
bool flag_alarmatimer = true;
bool flag_actualizarDatos;

driverMotor motor1(&motor);

int main(void) {
	Inicializar();
	InicializarMdE();
    Wifi_Init();
    handler_envio_datos.start();

    while(1) {
        Wifi_Manejar();
        MdEMotor();
    }

    return 0;
}


void actualizarGUI() //actualizamos datos de la interfaz grafica
{
	sprintf(buffer_envio,
	        "{"
	        "\"rpm\": %d, "
	        "\"temp\": %.1f, "
	        "\"sentido\": %s, "
	        "\"alarma\": %s"
	        "}\r\n",
	        rpm,
	        temperatura_motor,
	        sentido_giro ? "true" : "false",
	        flag_alarmatimer ? "true" : "false");


	if(!Wifi_EnviarDato(buffer_envio)) //realizamos el envio de datos solo si estamos conectados
	{
		pc.Transmitir((uint8_t*)"WARN: WiFi ocupado, reintentando...\r\n", 37);
	}

		handler_envio_datos.start();
}

