/*
 * MdEWifi.h
 */
#ifndef MDEWIFI_H_
#define MDEWIFI_H_

#include <LPC845.h>
#include <serialCom.h>
#include <gpio.h>
#include <DigitalInput.h>
#include <Timer.h>
#include <detectorGiro.h>
#include <driverMotor.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "StringFormatter.h"


// =================================================================
// == TIPOS Y ENUMS GLOBALES
// =================================================================
#define RESPUESTA_MAX_LEN 128
#define TAMANO_BUFFER_WIFI 128 // Debe ser igual (o mayor) a tu buffer_envio

typedef enum { AT_RESULT_OK, AT_RESULT_ERROR, AT_RESULT_TIMEOUT, AT_RESULT_NONE } AT_Result;
typedef enum { AT_IDLE, AT_SEND_COMMAND, AT_WAITING_RESPONSE } AT_State;
typedef enum { WIFI_NONE, WIFI_DESCONECTADO, WIFI_MODO_CONFIG, WIFI_CONECTANDO, WIFI_OBTENIENDO_IP, WIFI_CONECTADO, WIFI_INIT_SERVER, WIFI_ENVIO_DATOS, WIFI_FALLA } Estado_wifi;
typedef void (*at_callback_t)(AT_Result resultado);
typedef enum { ENVIO_IDLE, ENVIO_INICIANDO_CIPSEND,ESPERANDO_SEND_OK} Estado_Envio_Wifi;


void Wifi_Init(void);
void Wifi_Manejar(void);
bool Wifi_EnviarDato(const char* mensaje);
void timeout_handler(void);
void miCallbackDeResultado(AT_Result resultado);
void wifi_intermitente_handler();
void callback_CIPSEND(AT_Result resultado);
bool Wifi_EstaOcupado();
void procesarMensajeCliente(const char* data);
void detectarMensajeIPD(char* linea);
bool iniciarEnvioComando(const char* comando, uint32_t timeout_ms, at_callback_t callback);
void manejarComunicacionAT();


// --- Máquina de Estados ---
extern volatile AT_State estadoComando;
extern Estado_wifi wifi_State;
extern const char* comandoPendiente;
extern uint32_t timeoutPendiente;
extern at_callback_t comandoCallback;
extern AT_Result resultado_AT;
extern bool comunicacionOK;
extern Estado_Envio_Wifi estadoEnvio;
extern bool cliente_conectado;
extern driverMotor motor1;
extern DetectorGiro d1;

// --- Buffer ---
extern char bufferRespuesta[RESPUESTA_MAX_LEN];
extern uint8_t bufferIndex;
extern char bufferPayload[RESPUESTA_MAX_LEN];
extern uint16_t lenPayload;

// --- Timer ---
extern bool timeout_flag;
extern Timer timeout_timer;

// --- Hardware ---
extern DigitalInput bot;
extern gpio led_wifi_OK;
extern gpio led_wifi_conectado;
extern serialCom esp;
extern serialCom pc;
extern Timer led_wifi_inter;

// --- Datos de Aplicación ---
extern uint32_t rpm;
extern uint32_t velocidad;
extern uint8_t sentido_giro;

extern bool flag_boton_inicio;
extern bool flag_velocidad;
extern bool flag_sentido;
extern bool flag_boton_parada;

#endif /* MDEWIFI_H_ */
