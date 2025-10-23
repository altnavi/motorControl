///*
//===============================================================================
// Name        : main.c
// Author      : $(author)
// Version     :
// Copyright   : $(copyright)
// Description : main definition
//===============================================================================
//*/
//#include <cr_section_macros.h>
//#include <LPC845.h>
//#include <AnalogInput.h>	//Para usar el ADC
//#include <serialCom.h>		//Para usar el puerto serie
//#include <Timer.h>			//Para usar el Timer tengo que agregar Timer.h, perifericoTemporizado y Systick al proyecto
//
//// Canal 0 : pin P0.7	Canal 1 :  pin P0.6		Canal 2 :  pin P0.14
//// Canal 3 : pin P0.23 	Canal 4 :  pin P0.22 	Canal 5 :  pin P0.21
//// Canal 6 : pin P0.20 	Canal 7 :  pin P0.19 	Canal 8 :  pin P0.18
//// Canal 9 : pin P0.17 	Canal 10 : pin P0.13 	Canal 11 : pin P0.4
//
////Puse canal 0 para probarlo con el pote del stick que está en el pin P0.7
//#define 	CANAL_ADC		1		//USAMOS CANAL 1
//#define		FREC_MUESTREO	10000
//#define		VELOCIDAD_COM	115200
//#define		TIEMPO_MS		1000
//
//
//
//void levantarFlag ( void );
//
//AnalogInput *sensor;
//serialCom	*comPC;
//
//bool flag_envio = false;
//
//int main(void) {
//
//	uint32_t resultado;
//	uint8_t resultadoASCII[6] = {'0','0','0','0','\n','\r'};
//
//	//Instancio los objetos que voy a necesitar en la aplicación
//	Timer tiempo_envio ( TIEMPO_MS , levantarFlag , true );
//	sensor = new AnalogInput ( CANAL_ADC , FREC_MUESTREO );
//	comPC = new serialCom (0, VELOCIDAD_COM, 25, 24);
//
//	//Arranco el timer (esta definido como repetitivo, asi que no tengo que volver a arrancarlo)
//	tiempo_envio.start();
//
//	while(1) {
//
//
//		if (flag_envio )
//		{
//			//Obtengo el resultado de las últimas muestras del ADC:
//			resultado = sensor->getValor();
//
//			//Convierto en ASCII para verlo en una terminal:
//			for ( int i = 0 ; i < 4 ; i++ )
//			{
//				resultadoASCII[ 3 - i ] = (resultado%10) + 48;
//				resultado/=10;
//			}
//
//			//Lo mando por puerto serie:
//			comPC->Transmitir( resultadoASCII , 6 );
//
//			flag_envio = false;
//		}
//
//    }
//    return 0 ;
//}
//
//void levantarFlag ( void )
//{
//	flag_envio = true;
//}
