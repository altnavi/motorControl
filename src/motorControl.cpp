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
#include <MdEMotor.h>
#include <Timer.h>
#include <Temperatura.h>
#include "StringFormatter.h"


void actualizarGUI();

#define gpio_botonArranque 0,0
#define gpio_botonParada 0,0
#define gpio_botonVelocidad 0,0
#define gpio_botonCambioSentido 0,0
#define gpio_cambioSentido 0,18
#define gpio_led_falla 0,0
#define gpio_alarma 0,0
#define gpio_motor 0,19
#define gpio_sensor1 0,16
#define gpio_sensor2 0,17

#define TAMANO_BUFFER_ENVIO 128

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
char buffer_envio[TAMANO_BUFFER_ENVIO]; // Buffer para crear los mensajes JSON
float temperatura_motor = 0;

uint32_t velocidad;
uint8_t sentido_giro = 2;
bool flag_RPMtimer;
bool flag_alarmatimer = false;
bool flag_actualizarDatos;

//MOTOR
driverMotor motor1(motor,cambioSentido);
DetectorGiro d1(gpio_sensor1, gpio_sensor2);

Temperatura t1(6);

int main(void) {
	Inicializar();
	InicializarMdE();
    Wifi_Init();
    handler_envio_datos.start();

    while(1) {
        Wifi_Manejar();
        d1.procesar();
        MdEMotor();
    }

    return 0;
}


void actualizarGUI() //actualizamos datos de la interfaz grafica
{
	rpm = d1.getRPM();
	sentido_giro = d1.getSentidoGiro();
	temperatura_motor = t1.getTemperatura();

	if (Wifi_EstaOcupado()) {
	        handler_envio_datos.start(); // Reintentar más tarde
	        return;
	    }

	// --- INICIO DEL CAMBIO ---

	// Llamamos a la nueva función económica.
	// Le pasamos el búfer Y su tamaño total para seguridad.
	int bytes_escritos = formato_json_economico(
	    buffer_envio,
	    TAMANO_BUFFER_ENVIO, // <-- Seguridad contra desbordamiento
	    rpm,
	    temperatura_motor,
	    sentido_giro,
	    flag_alarmatimer
	);
    // --- FIN DEL CAMBIO ---

    // --- MEJORA DE SEGURIDAD ---
    // Si bytes_escritos es -1, el búfer fue muy pequeño.
    // No deberíamos enviar datos corruptos.
	if (bytes_escritos > 0)
    {
        if(!Wifi_EnviarDato(buffer_envio)) //realizamos el envio de datos solo si estamos conectados
        {
            pc.Transmitir((uint8_t*)"WARN: WiFi ocupado, reintentando...\r\n", 37);
        }
    }
    else
    {
        // El JSON no cupo en el búfer. ¡Esto es un error grave!
        // (En el pasado, 'sprintf' habría colgado tu LPC aquí)
        pc.Transmitir((uint8_t*)"ERROR: Buffer de envío demasiado pequeño!\r\n", 40);
    }

	handler_envio_datos.start();
}
