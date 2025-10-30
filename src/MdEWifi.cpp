#include "MdEWifi.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

volatile AT_State estadoComando = AT_IDLE;
Estado_wifi wifi_State = WIFI_NONE;
const char* comandoPendiente = NULL;
uint32_t timeoutPendiente = 0;
at_callback_t comandoCallback = NULL;
AT_Result resultado_AT = AT_RESULT_NONE;
bool comunicacionOK = false;
char bufferRespuesta[RESPUESTA_MAX_LEN];
uint8_t bufferIndex = 0;
bool timeout_flag = false;
Timer timeout_timer(1, timeout_handler);
Estado_Envio_Wifi estadoEnvio = ENVIO_IDLE;
char bufferPayload[RESPUESTA_MAX_LEN];
uint16_t lenPayload = 0;
bool cliente_conectado = false;

void procesarMensajeCliente(const char* data);
void detectarMensajeIPD(char* linea);
bool iniciarEnvioComando(const char* comando, uint32_t timeout_ms, at_callback_t callback);
void manejarComunicacionAT();

void Wifi_Init(void) {
	led_wifi_inter.stop();
	led_wifi_OK.clrPIN();
	led_wifi_conectado.clrPIN();
    iniciarEnvioComando("AT+RST\r\n", 1000, miCallbackDeResultado);
    pc.Transmitir((uint8_t*)"Sistema listo\r\n", 15);
}

void Wifi_Manejar(void) {
    manejarComunicacionAT();

    if (estadoEnvio == ENVIO_INICIANDO_CIPSEND && estadoComando == AT_IDLE) {
        // La máquina AT está libre, podemos enviar el comando CIPSEND
        char comandoCipsend[32];
        sprintf(comandoCipsend, "AT+CIPSEND=0,%d\r\n", lenPayload);
        iniciarEnvioComando(comandoCipsend, 2000, callback_CIPSEND);
    }

    if (bot.getKey()) {
        wifi_State = WIFI_DESCONECTADO;
        resultado_AT = AT_RESULT_NONE;
        comunicacionOK = true; // Reinicia el flag para reintentar
    }

    switch (wifi_State) {
        case WIFI_DESCONECTADO:
            if (estadoComando == AT_IDLE) {
                led_wifi_inter.start();
                if (resultado_AT == AT_RESULT_OK) {
                    wifi_State = WIFI_MODO_CONFIG;
                    resultado_AT = AT_RESULT_NONE;
                } else if (resultado_AT == AT_RESULT_TIMEOUT || resultado_AT == AT_RESULT_ERROR) {
                    wifi_State = WIFI_FALLA;
                } else {
                    iniciarEnvioComando("AT\r\n", 1000, miCallbackDeResultado);
                }
            }
            break;
        case WIFI_MODO_CONFIG:
            if (estadoComando == AT_IDLE) {
                if (resultado_AT == AT_RESULT_OK) {
                    wifi_State = WIFI_CONECTANDO;
                    resultado_AT = AT_RESULT_NONE;
                } else if (resultado_AT == AT_RESULT_TIMEOUT || resultado_AT == AT_RESULT_ERROR) {
                    wifi_State = WIFI_FALLA;
                } else {
                    iniciarEnvioComando("AT+CWMODE=1\r\n", 1000, miCallbackDeResultado);
                }
            }
            break;
        case WIFI_CONECTANDO:
            if (estadoComando == AT_IDLE) {
                if (resultado_AT == AT_RESULT_OK) {
                    wifi_State = WIFI_OBTENIENDO_IP;
                    resultado_AT = AT_RESULT_NONE;
                } else if (resultado_AT == AT_RESULT_TIMEOUT || resultado_AT == AT_RESULT_ERROR) {
                    wifi_State = WIFI_FALLA;
                } else {
                    iniciarEnvioComando("AT+CWJAP=\"motorola\",\"luciano123\"\r\n", 10000, miCallbackDeResultado);
                }
            }
            break;
        case WIFI_OBTENIENDO_IP:
            if (estadoComando == AT_IDLE) {
                if (resultado_AT == AT_RESULT_OK) {
                    wifi_State = WIFI_CONECTADO;
                    resultado_AT = AT_RESULT_NONE;
                } else if (resultado_AT == AT_RESULT_TIMEOUT || resultado_AT == AT_RESULT_ERROR) {
                    wifi_State = WIFI_FALLA;
                } else {
                    iniciarEnvioComando("AT+CIFSR\r\n", 2000, miCallbackDeResultado);
                }
            }
            break;
        case WIFI_CONECTADO:
            led_wifi_OK.setPIN();
            led_wifi_conectado.clrPIN();
            led_wifi_inter.stop();
            wifi_State = WIFI_INIT_SERVER;
            break;
        case WIFI_INIT_SERVER: {
            static uint8_t count = 0;
            if (estadoComando == AT_IDLE) {
                if (resultado_AT == AT_RESULT_OK) {
                    count++;
                    resultado_AT = AT_RESULT_NONE; // Limpiar para el siguiente comando
                } else if (resultado_AT == AT_RESULT_TIMEOUT || resultado_AT == AT_RESULT_ERROR) {
                    wifi_State = WIFI_FALLA;
                    count = 0;
                    break;
                }

                if (count == 0) {
                     iniciarEnvioComando("AT+CIPMUX=1\r\n", 2000, miCallbackDeResultado);
                } else if (count == 1) {
                     iniciarEnvioComando("AT+CIPSERVER=1,5000\r\n", 2000, miCallbackDeResultado);
                } else if (count >= 2) {
                    wifi_State = WIFI_ENVIO_DATOS;
                    count = 0;
                }
            }
            break;
        }
        case WIFI_ENVIO_DATOS:

            break;
        case WIFI_FALLA:
            led_wifi_inter.stop();
            led_wifi_conectado.setPIN();
            break;
        default:

            break;
    }
}

// En MdEWifi.cpp

bool Wifi_EnviarDato(const char* mensaje) {
    if (wifi_State != WIFI_ENVIO_DATOS || !cliente_conectado || estadoEnvio != ENVIO_IDLE) {
        return false;
    }
    lenPayload = strlen(mensaje);
    if (lenPayload >= RESPUESTA_MAX_LEN) lenPayload = RESPUESTA_MAX_LEN - 1;
    strncpy(bufferPayload, mensaje, lenPayload);
    bufferPayload[lenPayload] = '\0';

    estadoEnvio = ENVIO_INICIANDO_CIPSEND;
    return true;
}

void timeout_handler() {
    timeout_flag = true;
}

void miCallbackDeResultado(AT_Result resultado) {
    if (resultado == AT_RESULT_OK) {
        pc.Transmitir((uint8_t*)"[CALLBACK]: El comando tuvo exito\r\n", 33);
        comunicacionOK = true;
    } else {
        pc.Transmitir((uint8_t*)"[CALLBACK]: El comando fallo\r\n", 30);
        comunicacionOK = false;
    }
}

 void procesarMensajeCliente(const char* data) {
    if (strcmp(data, "motor_ON") == 0) {
        motor1.encenderMotor();
        pc.Transmitir((uint8_t*)"motor encendido\r\n", 15);
    } else if (strcmp(data, "motor_OFF") == 0) {
    	motor1.apagarMotor();
        pc.Transmitir((uint8_t*)"motor apagado\r\n", 13);
    } else if (strncmp(data, "RPM=", 4) == 0) {
        //rpm = atoi(data + 4);
        //char buffer_respuesta[40];
        //sprintf(buffer_respuesta, "RPM objetivo actualizado a: %d\r\n", rpm);
        //pc.Transmitir((uint8_t*)buffer_respuesta, strlen(buffer_respuesta));
    }
    else if(strcmp(data, "aumentar") == 0) //aumenta la velocidad del motor
	{
    	if(velocidad>0)
    	velocidad--;

    	motor1.setVelocidad(velocidad);
    	pc.Transmitir((uint8_t*)"velocidad aumentada\r\n", 22);

	}
    else if(strcmp(data, "disminuir") == 0) //disminuye la velocidad del motor
	{
    	if(velocidad<5)
    	velocidad++;

    	motor1.setVelocidad(velocidad);
    	pc.Transmitir((uint8_t*)"velocidad disminuida\r\n", 22);
	}
    else if(strcmp(data, "invertir") == 0) //disminuye la velocidad del motor
	{
    	if(sentido_giro==DetectorGiro::ANTIHORARIO)
    		motor1.setSentido(DetectorGiro::HORARIO);
    	else
    		motor1.setSentido(DetectorGiro::ANTIHORARIO);

    	pc.Transmitir((uint8_t*)"velocidad disminuida\r\n", 22);
	}
	else
	{
        pc.Transmitir((uint8_t*)"Comando desconocido\r\n", 22);
    }
}

 void detectarMensajeIPD(char* linea) {
    if (strncmp(linea, "+IPD,", 5) != 0) return;

    uint8_t pos = 5;
    int len = 0;

    // Saltar el ID de conexión y la coma
    while (linea[pos] != '\0' && linea[pos] != ',') pos++;
    if (linea[pos] == ',') pos++;

    // Leer la longitud del mensaje
    while (linea[pos] >= '0' && linea[pos] <= '9') {
        len = len * 10 + (linea[pos++] - '0');
    }

    if (linea[pos] == ':') pos++;

    // Extraer los datos
    char data[64];
    uint8_t i;
    for (i = 0; i < len && pos < RESPUESTA_MAX_LEN && i < sizeof(data) - 1; i++, pos++) {
        data[i] = linea[pos];
    }
    data[i] = '\0';

    procesarMensajeCliente(data);
}

 bool iniciarEnvioComando(const char* comando, uint32_t timeout_ms, at_callback_t callback) {
    if (estadoComando == AT_IDLE) {
        comandoPendiente = comando;
        timeoutPendiente = timeout_ms;
        comandoCallback = callback;
        estadoComando = AT_SEND_COMMAND;
        return true;
    }
    return false;
}

 void wifi_intermitente_handler() {
 	led_wifi_conectado.togglePIN();
 	led_wifi_inter.start();
 }

 void manejarComunicacionAT() {
    int16_t b = esp.Recibir();
    if (b != serialCom::NO_DATA) {
        char c = (char)b;
        if (bufferIndex < RESPUESTA_MAX_LEN - 1) {
            bufferRespuesta[bufferIndex++] = c;
        }

        if (c == '\n') {
            bufferRespuesta[bufferIndex] = '\0';
            pc.Transmitir((uint8_t*)bufferRespuesta, bufferIndex);
                if (strstr(bufferRespuesta, "CONNECT") != NULL) {
                    cliente_conectado = true;
                    pc.Transmitir((uint8_t*)"[INFO] Cliente Conectado.\r\n", 28);
                } else if (strstr(bufferRespuesta, "CLOSED") != NULL) {
                    cliente_conectado = false;
                    pc.Transmitir((uint8_t*)"[INFO] Cliente Desconectado.\r\n", 30);
                }

            if (strncmp(bufferRespuesta, "+IPD,", 5) == 0) {
                detectarMensajeIPD(bufferRespuesta);
            } else if (estadoComando == AT_WAITING_RESPONSE) {
                if (strstr(bufferRespuesta, "OK") != NULL) {
                    timeout_timer.stop();
                    if (comandoCallback != NULL) comandoCallback(AT_RESULT_OK);
                    resultado_AT = AT_RESULT_OK;
                    estadoComando = AT_IDLE;
                } else if (strstr(bufferRespuesta, "ERROR") != NULL) {
                    timeout_timer.stop();
                    if (comandoCallback != NULL) comandoCallback(AT_RESULT_ERROR);
                    resultado_AT = AT_RESULT_ERROR;
                    estadoComando = AT_IDLE;
                }
            }
            bufferIndex = 0;
            memset(bufferRespuesta, 0, RESPUESTA_MAX_LEN);
        }
    }

    switch (estadoComando) {
        case AT_IDLE:
            break;
        case AT_SEND_COMMAND:
            while (esp.Recibir() != serialCom::NO_DATA); // Limpiar buffer antes de enviar
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
                timeout_timer.stop();
                if (comandoCallback != NULL) comandoCallback(AT_RESULT_TIMEOUT);
                resultado_AT = AT_RESULT_TIMEOUT;
                estadoComando = AT_IDLE;
            }
            break;
    }
}

 void callback_CIPSEND(AT_Result resultado) {
     if (resultado == AT_RESULT_OK) {
         // El comando fue aceptado, ahora enviamos el payload (el mensaje real)
         esp.Transmitir((uint8_t*)bufferPayload, lenPayload);
     } else {
         // El comando falló, informamos el error
         pc.Transmitir((uint8_t*)"[ERROR] Falla en CIPSEND.\r\n", 28);
     }
     // El proceso de envío terminó (con éxito o fracaso), liberamos la máquina
     estadoEnvio = ENVIO_IDLE;
 }
