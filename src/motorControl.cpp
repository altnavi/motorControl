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
#include <i2cCom.h>
#include <LcdI2C.h>

void actualizarGUI();
void actDisplay();

#define gpio_cambioSentido 0,18
#define gpio_alarma 0,13
#define gpio_motor 0,19
#define gpio_sensor1 0,16
#define gpio_sensor2 0,17

#define TAMANO_BUFFER_ENVIO 128

//  GPIOS
gpio led_wifi_OK(1,0,gpio::SALIDA, gpio::LOW);
gpio led_wifi_conectado(1,1,gpio::SALIDA, gpio::LOW);
gpio cambioSentido(gpio_cambioSentido,gpio::SALIDA, gpio::HIGH);
gpio alarma(gpio_alarma,gpio::SALIDA, gpio::HIGH);
gpio motor(gpio_motor,gpio::SALIDA, gpio::HIGH);

// BOTONES
DigitalInput bot(0, 4);

// COMUNICACION SERIE
serialCom esp(1, 115200, 8, 9);
serialCom pc(0, 115200, 25, 24);

// TIMERS
Timer led_wifi_inter(300, wifi_intermitente_handler);
Timer handler_envio_datos(1000, actualizarGUI);
Timer RPMtimer(4000,handler_RPMtimer);
Timer alarmatimer(3000,handler_alarmatimer);
Timer actualizarDatos(800,handler_actualizarDatos);
Timer buzzer_inter(100, buzzer_intermitente);
// VARIABLES
uint32_t rpm;
char buffer_envio[TAMANO_BUFFER_ENVIO];
float temperatura_motor = 0;

uint32_t velocidad;
uint8_t sentido_giro = 0;

bool flag_RPMtimer = false;
bool flag_alarmatimer = false;
bool flag_actualizarDatos = false;
bool flag_boton_inicio = false;
bool flag_boton_parada = false;
bool flag_velocidad = false;
bool flag_sentido = false;

uint8_t sentido_detectado = DetectorGiro::DETENIDO;


// MOTOR
driverMotor motor1(motor,cambioSentido);
DetectorGiro d1(gpio_sensor1, gpio_sensor2);
Temperatura t1(6);


//DISPLAY
I2C_COM i2c(0, 11, 10, 0x27);
LcdI2C lcd(&i2c);


int main(void) {
	Inicializar();
	InicializarMdE();
    Wifi_Init();

    handler_envio_datos.start();

    lcd.init(16, 2);

    lcd.setCursor(0, 0);
    lcd.print("Estado:");

        // Opcional: Escribir algo en la linea 2 mientras esperamos datos
    lcd.setCursor(0, 1);
    lcd.print("Esperando...");


    while(1) {
    	MdEMotor();
        Wifi_Manejar();
        d1.procesar();

    }

    return 0;
}

void actDisplay()
{
	lcd.setCursor(8,0);
	if(flag_alarmatimer)
	{
		lcd.print("     ");
		lcd.setCursor(8,0);
		lcd.print("FALLA");
	}
	else
	{
		lcd.print("     ");
		lcd.setCursor(8,0);
		lcd.print("OK");
	}

    lcd.setCursor(0, 1);
    lcd.print("RPM: ");
    lcd.print(rpm);
    lcd.print("    "); // limpiar restos


    lcd.setCursor(9,1);
    lcd.print("T: ");
    lcd.print(temperatura_motor);
}

void actualizarGUI()
{
	static uint8_t contador = 0;

	if (Wifi_EstaOcupado()) {
	    handler_envio_datos.start();
	    return;
	}

	int bytes_escritos = formato_json_economico(
	    buffer_envio,
	    TAMANO_BUFFER_ENVIO,
	    rpm,
	    temperatura_motor,
	    sentido_giro,
	    flag_alarmatimer
	);

	if (bytes_escritos > 0)
    {
        if(!Wifi_EnviarDato(buffer_envio))
        {
        		//no hago nada por el momento si esta ocupado
        }
    }

	if(contador>=2)
	{
		actDisplay();
		contador=0;
	}

	contador++;

	handler_envio_datos.start();
}
