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
#include <gpio.h>
#include <driverMotor.h>
#include <Timer.h>
#include <detectorGiro.h>
#include <digitalInput.h>
#include <serialCom.h>
#include <string.h>

void handlr();

gpio motor(0,10,gpio::SALIDA,gpio::HIGH);
gpio led(1,2,gpio::SALIDA,gpio::LOW);
gpio invertir_giro(0,6,gpio::SALIDA, gpio::HIGH);
Timer t1(2000,handlr);
driverMotor m1(&motor);
DetectorGiro dec(1,0,1,1);
uint32_t rpm = 0;
DigitalInput bot(0,4);

#define RESPUESTA_MAX_LEN 128  // suficiente para cualquier línea del ESP

char lineBuffer[RESPUESTA_MAX_LEN];
uint8_t lineIndex = 0;

// Inicializamos UARTs correctamente
serialCom esp(1, 115200, 8, 9);   // UART1 al ESP
serialCom pc(0, 115200, 25, 24);  // UART0 a PC

uint8_t COMANDO_AT[] = "AT+RST\r\n";

// Función para leer una línea completa del ESP
bool LeerLineaESP(char* buffer, uint8_t* index)
{
    int16_t b;
    bool lineaCompleta = false;

    while((b = esp.Recibir()) != serialCom::NO_DATA)
    {
        char c = (char)b;
        if(*index < RESPUESTA_MAX_LEN - 1)
            buffer[(*index)++] = c;

        if(c == '\n')
        {
            buffer[*index] = '\0';
            *index = 0;
            lineaCompleta = true;
            break;
        }
    }
    return lineaCompleta;
}

int main(void)
{
    Inicializar();

    while(1)
    {
        // Reenviar líneas completas
        if(LeerLineaESP(lineBuffer, &lineIndex))
        {
            uint8_t len = strlen(lineBuffer);
            if(len) pc.Transmitir((uint8_t*)lineBuffer, len);
        }

        // Enviar AT si se presiona botón
        if(bot.getKey())
        {
            esp.Transmitir(COMANDO_AT, sizeof(COMANDO_AT)-1);
        }
    }
}

void handlr()
{
    rpm = dec.getRPM();
    uint8_t g = dec.getSentidoGiro();
    if (g == DetectorGiro::ANTIHORARIO)
        led.setPIN();
    else
        led.clrPIN();
    t1.start();
}

