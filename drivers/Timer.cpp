/*
 * Timer.cpp
 *
 *  Created on: 11 jun. 2023
 *      Author: chor
 */

#include "Timer.h"


Timer::Timer ( uint32_t _tiempo , void (*_Handler) (void), bool _recargar)
{
	//Configuro el nuevo timer:
	recarga = _tiempo;
	Handler = _Handler;
	recargar = _recargar;

	//NO lo agrego a la lista de timers (porque lo hace PerifericoTemporizado):
	/*Timer **aux = new Timer*[cant_timers+1];
	for ( int i = 0 ; i < cant_timers ; i++ )
		aux[i] = timers[i];

	aux[cant_timers] = this;
	cant_timers++;

	//Borro la lista vieja y hago que la lista apunte al nuevo vector dinámico:
	delete[] timers;
	timers = aux;
	 */
}

Timer::~Timer()
{
	//No hace nada (porque lo hace PerifericoTemporizado)
	/*
	//Busco la posicion del timer en el listado de timers:
	int posicion;

	for ( posicion = 0 ; posicion < cant_timers ; posicion++ )
		if ( timers[posicion] == this )
			break;

	//Saco el timer de la lista de timers activos:
	Timer **aux = new Timer*[cant_timers-1];
	for ( int i = 0 ; i < posicion; i++ )
		aux[i] = timers[i];
	for ( int i = posicion + 1 ; i < cant_timers ; i++ )
		aux[i-1] = timers[i];

	delete[] timers;
	cant_timers--;
	*/
}

void Timer::start ()
{
	if ( recarga != 0 && Handler != nullptr )
		tiempo = recarga;
}

void Timer::stop ()
{
	tiempo = 0;
}

void Timer::handler ( void )
{
	if ( tiempo != 0)
	{
		tiempo--;
		if ( !tiempo )
		{
			Handler();
			if (recargar)
				tiempo = recarga;
		}
	}
}
