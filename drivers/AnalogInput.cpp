/*
 * AnalogInput.cpp
 *
 *  Created on: 31 oct. 2023
 *      Author: chor
 */

#include <AnalogInput.h>

AnalogInput **entradasAnalogicas = nullptr;
uint8_t AnalogInput::cant_entradas = 0;


AnalogInput::AnalogInput( uint8_t _canal , uint32_t frec , uint32_t cant_muestras )
{
	//Agrego una a la cantidad de entradas analogicas configuradas:
	cant_entradas ++;

	//Establezco cual es el canal que se corresponde a dicha entrada:
	canal = _canal;

	//Genero un vector de cant_muestras para almacenar los ultimos valores del ADC:
	vectorCuentas = new uint32_t [cant_muestras];
	tam_vector = cant_muestras;

	//Agrego la entrada analogica que se está instanciando a un vector dinámico de entradas:
	//1. Genero un nuevo vector dinámico:
	AnalogInput **auxiliar = new AnalogInput*[cant_entradas];
	//2. Si había un vector previo copio miembro a miembro todos los valores:
	for ( int i = 0 ; i < cant_entradas - 1 ; i++ )
		auxiliar[i] = entradasAnalogicas[i];
	//3. Y agrego al final este objeto:
	auxiliar[cant_entradas - 1] = this;
	//4. Elimino el vector viejo (si existía)
	delete[] entradasAnalogicas;
	//5. Y hago que el puntero entradasAnalógicas apunte a mi nuevo vector de entradas:
	entradasAnalogicas = auxiliar;

	//Energizo el ADC
	SYSCON->PDRUNCFG &= ~(1<<4);

	//Habilito el CLK del ADC:
	SYSCON->SYSAHBCLKCTRL0 |= 1<<24;

	//Selecciono el clk del ADC a 30MHz:
	SYSCON->ADCCLKSEL = 0;

	//Sin divisor:
	SYSCON->ADCCLKDIV = 0;

	//Para modificar la matriz de Switch (seleccionar el pin que voy a usar para el ADC)
	//primero habilito el clk:
	SYSCON->SYSAHBCLKCTRL0 |= 1<<7;

	//En función del canal que haya elegido, habilito diferentes pines como entrada analógica:
	// Canal 0 : pin P0.7 	-> Se habilita con PINENABLE0, bit 14
	// Canal 1 : pin P0.6 	-> Se habilita con PINENABLE0, bit 15
	// Canal 2 : pin P0.14 	-> Se habilita con PINENABLE0, bit 16
	// Canal 3 : pin P0.23 	-> Se habilita con PINENABLE0, bit 17
	// Canal 4 : pin P0.22 	-> Se habilita con PINENABLE0, bit 18
	// Canal 5 : pin P0.21 	-> Se habilita con PINENABLE0, bit 19
	// Canal 6 : pin P0.20 	-> Se habilita con PINENABLE0, bit 20
	// Canal 7 : pin P0.19 	-> Se habilita con PINENABLE0, bit 21
	// Canal 8 : pin P0.18 	-> Se habilita con PINENABLE0, bit 22
	// Canal 9 : pin P0.17 	-> Se habilita con PINENABLE0, bit 23
	// Canal 10 : pin P0.13 -> Se habilita con PINENABLE0, bit 24
	// Canal 11 : pin P0.4 	-> Se habilita con PINENABLE0, bit 25

	uint8_t shift =  14 + canal;

	//Habilito la entrada ADCn, que se encuentra como función especial:
	PINENABLE0 &= ~(1<<shift);

	//Calibración del ADC:
	//Activo el bit de calibracion (bit30) y pongo el divisor a 500kHz (60):
	ADC->CTRL = (60 | (1<<30));

	//Espero a que se termine la calibracion (bloqueante por aprox 290useg):
	while ( ADC->CTRL & (1<<30) );

	//Establezco la frecuencia de muestreo:
	//Cada muestra toma 25 ciclos de clock, por lo que la frecuencia máxima es 30MHz/25.
	//Luego tengo que tener en cuenta que la frecuencia de muestreo estará dividida por la cantidad
	//de entradas analógicas que haya (tengo que repartir el adc entre cant_entradas)
	//Tengo que configurar el divisor para lograr esta frecuencia de muestreo de manera que:
	//frec_muestreo = FRO/(25*cant_entradas*divisor)

	//Por lo tanto: divisor = FRO/(25*cant_entradas*frec_muestreo)
	uint32_t aux = 25*cant_entradas*frec;
	uint32_t divisor = (30000000/aux)-1;

	//Muestreo a 10kHz: como cada conversion requiere 25 ciclos de clk,
	//necesito que el clk del ADC quede a 250kHz. Como el clk es de 30MHz,
	//el divisor debe ser de 120

	ADC->CTRL = 	divisor		|	//DIVISOR - 1
					(0 << 8 )	|	//Modo sincrónico
					(0 << 10)	|	//Low power mode deshabilitado
					(0 << 30);		//Modo calibración deshabilitado

	//Deshabilito la secuencia B:
	ADC->SEQB_CTRL = 0;

	//Selecciono VDD por encima de 2,7V (la alimentacion del ADC):
	ADC->TRM &= ~(1<<5);

	//Deshabilito SEQA_CTRL para configurarla:
	ADC->SEQA_CTRL &= ~(1<<31);

	//Configuro la secuencia A:
	ADC->SEQA_CTRL = 	(1 << canal)|		//Selecciono que se utilice el canal configurado
						(0 << 19) 	|		//Conversiones sincrónicas
						(0 << 24) 	|		//Conversion a la frec_adc
						(1 << 27) 	|		//BURST mode
						(1 << 31);			//Habilitar seq A

	//Habilito la interrupcion por el fin de la conversion de la secuencia A:
	ADC->INTEN = 1;

	//Habilito la interrupcion en el NVIC:
	ISER0 |= (1 << 16);

}

uint32_t AnalogInput::getValor()
{
	//Devuelvo el valor como un promedio de las ultimas n_muestras:
	uint32_t resultado = 0;

	for ( int i = 0 ; i < tam_vector ; i++ )
		resultado+= vectorCuentas[i];

	return ( resultado / tam_vector );
}

void AnalogInput::nuevaLectura ( uint32_t lectura )
{
	static uint8_t i = 0;

	//Pongo las ultimas n muestras en un vector, y también actualizo el ultimo valor:
	vectorCuentas[i] = lectura;
	i++;
	i%= tam_vector;

}

uint8_t AnalogInput::getCanal()
{
	return canal;
}


void ADC_SEQA_IRQHandler ( void )
{
	//Leo la nueva muestra obtenida por la interrupcion:
	uint32_t resultado = ADC->SEQA_GDAT;
	uint8_t canal = (resultado >> 26) & 0x0F;
	uint32_t muestras = (resultado >> 4) & 0xFFF;

	//Me fijo a cual de las entradas habilitadas se corresponde:
	for ( int i = 0 ; i < AnalogInput::cant_entradas ; i++ )
		if ( entradasAnalogicas[i]->getCanal() == canal )
			entradasAnalogicas[i]->nuevaLectura(muestras);

}

