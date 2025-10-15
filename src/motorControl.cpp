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

// =================================================================
// == SECCIÓN DE COMUNICACIÓN NO BLOQUEANTE CON ESP8266
// =================================================================

#define RESPUESTA_MAX_LEN 128

// --- Definiciones para la Máquina de Estados y Callbacks ---

// Enum para los posibles resultados de un comando
typedef enum {
    AT_RESULT_OK,
    AT_RESULT_ERROR,
    AT_RESULT_TIMEOUT,
	AT_RESULT_NONE
} AT_Result;

// Enum para definir los estados de nuestra máquina de comunicación
typedef enum {
    AT_IDLE,             // No estamos haciendo nada
    AT_SEND_COMMAND,     // Necesitamos enviar un comando
    AT_WAITING_RESPONSE  // Comando enviado, esperando respuesta
} AT_State;

typedef enum {
    WIFI_DESCONECTADO,
    WIFI_MODO_CONFIG,
    WIFI_CONECTANDO,
    WIFI_OBTENIENDO_IP, // Opcional, para AT+CIFSR
    WIFI_CONECTADO,
    WIFI_FALLA
} Estado_wifi;
// Definición del tipo de una función "callback".
// Será una función que no devuelve nada (void) y recibe un AT_Result.
typedef void (*at_callback_t)(AT_Result resultado);


// --- Handler y bandera para el Timer de Timeout ---
bool timeout_flag = false;

void timeout_handler() {
    timeout_flag = true;
}

// Timer global dedicado para gestionar los timeouts de los comandos AT
Timer timeout_timer(1, timeout_handler);


// --- Variables de la Máquina de Estados ---
volatile AT_State estadoComando = AT_IDLE; // Estado actual de la máquina
Estado_wifi wifi_State = WIFI_FALLA;
const char* comandoPendiente = NULL;       // Puntero al string del comando a enviar
uint32_t timeoutPendiente = 0;             // Timeout para el comando actual
at_callback_t comandoCallback = NULL;      // Puntero a la función callback a llamar al finalizar
AT_Result resultado_AT = AT_RESULT_NONE; 	//si fue exitoso el comando enviado al esp activo este flag

// Buffer para ensamblar las respuestas línea por línea
char bufferRespuesta[RESPUESTA_MAX_LEN];
uint8_t bufferIndex = 0;

// Prototipos
void manejarComunicacionAT();
bool iniciarEnvioComando(const char* comando, uint32_t timeout_ms, at_callback_t callback);
void miCallbackDeResultado(AT_Result resultado);
void handlr();

DigitalInput bot(0, 4);
gpio led_wifi(1,0,gpio::SALIDA, gpio::LOW);
serialCom esp(1, 115200, 8, 9);     // UART1 al ESP8266
serialCom pc(0, 115200, 25, 24);    // UART0 a la PC

// =================================================================
// == IMPLEMENTACIÓN DE LAS FUNCIONES
// =================================================================

/**
 * @brief Prepara un comando AT para ser enviado, especificando un callback para la respuesta.
 * @param comando El comando AT a enviar (debe incluir \r\n).
 * @param timeout_ms Tiempo máximo de espera para este comando.
 * @param callback La función que se ejecutará cuando se reciba la respuesta final.
 * @return true si el comando fue aceptado, false si la máquina ya está ocupada.
 */
bool iniciarEnvioComando(const char* comando, uint32_t timeout_ms, at_callback_t callback) {
    if (estadoComando == AT_IDLE) {
        comandoPendiente = comando;
        timeoutPendiente = timeout_ms;
        comandoCallback = callback; // Guardamos la función a la que se debe llamar
        estadoComando = AT_SEND_COMMAND;
        return true;
    }
    return false; // La máquina está ocupada
}

/**
 * @brief Motor de la máquina de estados. Gestiona la comunicación AT de forma no bloqueante.
 * DEBE SER LLAMADA CONSTANTEMENTE EN EL BUCLE PRINCIPAL.
 */
void manejarComunicacionAT() {
    switch (estadoComando) {
        case AT_IDLE:
            break;

        case AT_SEND_COMMAND:
            while(esp.Recibir() != serialCom::NO_DATA); // Limpiar buffer de entrada

            pc.Transmitir((uint8_t*)"---> Enviando: ", 15);
            pc.Transmitir((uint8_t*)comandoPendiente, strlen(comandoPendiente));
            esp.Transmitir((uint8_t*)comandoPendiente, strlen(comandoPendiente));

            timeout_flag = false;
            timeout_timer.setTime(timeoutPendiente);
            timeout_timer.start();

            bufferIndex = 0;
            memset(bufferRespuesta, 0, RESPUESTA_MAX_LEN);

            estadoComando = AT_WAITING_RESPONSE;
            break;

        case AT_WAITING_RESPONSE:
            if (timeout_flag) {
                pc.Transmitir((uint8_t*)"\r\n<--- TIMEOUT\r\n", 17);
                if (comandoCallback != NULL) {
                    comandoCallback(AT_RESULT_TIMEOUT); // Llamar al callback con resultado TIMEOUT
                }
                estadoComando = AT_IDLE;
                break;
            }

            int16_t b = esp.Recibir();
            if (b != serialCom::NO_DATA) {
                char c = (char)b;
                if (bufferIndex < RESPUESTA_MAX_LEN - 1) {
                    bufferRespuesta[bufferIndex++] = c;
                }

                if (c == '\n') {
                    pc.Transmitir((uint8_t*)bufferRespuesta, bufferIndex);

                    if (strstr(bufferRespuesta, "OK") != NULL) {
                        if (comandoCallback != NULL) {
                            comandoCallback(AT_RESULT_OK); // Llamar al callback con resultado OK
                            resultado_AT = AT_RESULT_OK;
                        }
                        estadoComando = AT_IDLE;
                    } else if (strstr(bufferRespuesta, "ERROR") != NULL) {
                        if (comandoCallback != NULL) {
                            comandoCallback(AT_RESULT_ERROR); // Llamar al callback con resultado ERROR
                            resultado_AT = AT_RESULT_ERROR;
                        }
                        estadoComando = AT_IDLE;
                    }
                    bufferIndex = 0;
                }
            }
            break;
    }
}


/**
 * @brief Esta es TU función. Se ejecutará cuando un comando AT termine.
 * Aquí pones la lógica para reaccionar al resultado.
 * @param resultado El resultado del comando (OK, ERROR, o TIMEOUT).
 */
void miCallbackDeResultado(AT_Result resultado) {
    if (resultado == AT_RESULT_OK) {
        pc.Transmitir((uint8_t*)"[CALLBACK]: El comando tuvo exito! ✅\r\n", 37);
        // Por ejemplo, encender el LED para indicar que el comando funcionó
        //led.setPIN();
    } else {
        pc.Transmitir((uint8_t*)"[CALLBACK]: El comando fallo! ❌\r\n", 34);
        // Apagar el LED para indicar que algo salió mal
        //led.clrPIN();
    }
}


// =================================================================
// == FUNCIÓN PRINCIPAL Y HANDLERS
// =================================================================

int main(void) {
    Inicializar();
    led_wifi.clrPIN();
    iniciarEnvioComando("AT+RST\r\n", 10000, miCallbackDeResultado); //reseteo el modulo wifi
    pc.Transmitir((uint8_t*)"Sistema listo",15);

    while(1) {
        manejarComunicacionAT();

        if(bot.getKey())
        {
        	wifi_State = WIFI_DESCONECTADO;
        }

        switch(wifi_State)
        {
        case WIFI_DESCONECTADO:
            if (estadoComando == AT_IDLE)
            {
				if(resultado_AT == AT_RESULT_OK)
				{
					wifi_State = WIFI_MODO_CONFIG;
					resultado_AT = AT_RESULT_NONE;
					break;
				}
				iniciarEnvioComando("AT\r\n",1000, miCallbackDeResultado);
            }
        	break;
        case WIFI_MODO_CONFIG:
            if (estadoComando == AT_IDLE)
            {
				if(resultado_AT == AT_RESULT_OK)
				{
					wifi_State = WIFI_CONECTANDO;
					resultado_AT = AT_RESULT_NONE;
					break;
				}
				iniciarEnvioComando("AT+CWMODE=1\r\n",1000, miCallbackDeResultado);
            }
        	break;
        case WIFI_CONECTANDO:
            if (estadoComando == AT_IDLE)
            {
				if(resultado_AT == AT_RESULT_OK)
				{
				   wifi_State = WIFI_OBTENIENDO_IP;
				   resultado_AT = AT_RESULT_NONE;
				   break;
				}
				iniciarEnvioComando("AT+CWJAP=\"ssid\",\"password\"\r\n",10000, miCallbackDeResultado);
            }
        	break;
        case WIFI_OBTENIENDO_IP:
            if (estadoComando == AT_IDLE)
            {
				if(resultado_AT == AT_RESULT_OK)
				{
				   wifi_State = WIFI_CONECTADO;
				   resultado_AT = AT_RESULT_NONE;
				   break;
				}
				iniciarEnvioComando("AT+CIFSR\r\n",2000, miCallbackDeResultado);
            }
        	break;
        case WIFI_CONECTADO:
        	led_wifi.setPIN(); //conectado correctamente
        	break;
        case WIFI_FALLA:
        	break;
        default:
        	break;
        }
    }
}

