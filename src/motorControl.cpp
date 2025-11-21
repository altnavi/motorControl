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
#include <stdio.h>


void actualizarGUI();
void lcd_sendCmd(I2C_COM &i2c, uint8_t cmd);


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

I2C_COM i2c;


// --- FUNCIONES AUXILIARES DE RETARDO (BLOQUEANTES) ---
void delay_ms(uint32_t ms) {
    for(volatile uint32_t i = 0; i < (ms); i++) { __asm("NOP"); }
}

// --- FUNCIONES DE BAJO NIVEL LCD ---
void lcd_sendNibble(uint8_t nibble, uint8_t is_data) {
    uint8_t byte = nibble & 0xF0;

    byte |= 0x08; // Backlight ON (Bit 3)
    if (is_data) byte |= 0x01; // RS = 1 para datos, 0 para comandos

    // Pulse EN (Bit 2)
    // 1. Enable High
    i2c.write(byte | 0x04);
    // 2. Enable Low (Latch)
    i2c.write(byte & ~0x04);
}

void lcd_cmd(uint8_t cmd) {
    lcd_sendNibble(cmd, 0);       // Parte alta
    lcd_sendNibble(cmd << 4, 0);  // Parte baja
}

void lcd_data(uint8_t data) {
    lcd_sendNibble(data, 1);      // Parte alta
    lcd_sendNibble(data << 4, 1); // Parte baja
}

void lcd_init() {
    //delay_ms(50); // Esperar a que el voltaje se estabilice

    // Secuencia mágica de inicialización (Modo 4 bits)
    lcd_sendNibble(0x30, 0); delay_ms(5);
    lcd_sendNibble(0x30, 0); delay_ms(1);
    lcd_sendNibble(0x30, 0); delay_ms(1);
    lcd_sendNibble(0x20, 0); delay_ms(1); // Set 4-bit mode

    // Configuración
    lcd_cmd(0x28); // Function set: 4-bit, 2 lines, 5x8 dots
    delay_ms(1);
    lcd_cmd(0x0C); // Display ON, Cursor OFF, Blink OFF
    delay_ms(1);
    lcd_cmd(0x01); // Clear Display
    delay_ms(2);
    lcd_cmd(0x06); // Entry mode: Increment cursor
}

void lcd_print(const char* str) {
    while (*str) {
        lcd_data(*str++);
    }
}

void lcd_printInt(int num) {
    char buffer[16];
    // snprintf convierte el int a una cadena de texto
    snprintf(buffer, sizeof(buffer), "%d", num);
    lcd_print(buffer);
}

void lcd_setCursor(uint8_t col, uint8_t row) {
    uint8_t offset[] = {0x00, 0x40, 0x14, 0x54};
    lcd_cmd(0x80 | (col + offset[row]));
}

int main(void) {
	Inicializar();

	InicializarMdE();
    Wifi_Init();
    handler_envio_datos.start();

    lcd_init();

    while(1) {
        Wifi_Manejar();
        d1.procesar();
        MdEMotor();
    }

    return 0;
}

void actualizarGUI() //actualizamos datos de la interfaz grafica
{
    lcd_setCursor(0, 0);
    lcd_print("Hola Mundo!");
    lcd_setCursor(0, 1);
    lcd_print("RPM:");
    lcd_print("123");
	lcd_setCursor(5, 1);
    lcd_printInt(123);

	rpm = d1.getRPM();
	sentido_giro = d1.getSentidoGiro();
	temperatura_motor = t1.getTemperatura();

	if (Wifi_EstaOcupado()) {
	        handler_envio_datos.start(); // Reintentar más tarde
	        return;
	    }

	int bytes_escritos = formato_json_economico(
	    buffer_envio,
	    TAMANO_BUFFER_ENVIO, // Seguridad contra desbordamiento
	    rpm,
	    temperatura_motor,
	    sentido_giro,
	    flag_alarmatimer
	);

	if (bytes_escritos > 0)
    {
        if(!Wifi_EnviarDato(buffer_envio)) //realizamos el envio de datos solo si estamos conectados
        {
            pc.Transmitir((uint8_t*)"WARN: WiFi ocupado, reintentando...\r\n", 37);
        }
    }
    else
    {
        pc.Transmitir((uint8_t*)"ERROR: Buffer de envío demasiado pequeño!\r\n", 40);
    }

	handler_envio_datos.start();
}



