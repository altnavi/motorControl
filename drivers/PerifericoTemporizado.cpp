/*
 * PerifericoTemporizado.cpp
 *
 *  Created on: 29 jul. 2024
 *      Author: chor
 */

#include "PerifericoTemporizado.h"

uint8_t PerifericoTemporizado::cant_pt = 0;
PerifericoTemporizado **perifericosTemp = nullptr;

PerifericoTemporizado::PerifericoTemporizado( void (*h) (void) ) {

	funcUsuario = h;

	//Actualizo la lista de perifericos temporizados agregando el nuevo:
	PerifericoTemporizado **aux = new PerifericoTemporizado*[cant_pt+1];
	for ( int i = 0 ; i < cant_pt ; i++ )
		aux[i] = perifericosTemp[i];

	aux[cant_pt] = this;
	cant_pt++;

	//Borro la lista vieja y hago que la lista apunte al nuevo vector dinámico:
	delete[] perifericosTemp;
	perifericosTemp = aux;

}

PerifericoTemporizado::~PerifericoTemporizado() {

	//Borro de la lista de perifericos temporizados el que se acaba de destruir:
	int posicion;

	for ( posicion = 0 ; posicion < cant_pt ; posicion++ )
		if ( perifericosTemp[posicion] == this )
			break;

	//Saco a "this" de la lista de perifericos:
	PerifericoTemporizado **aux = new PerifericoTemporizado*[cant_pt-1];
	for ( int i = 0 ; i < posicion; i++ )
		aux[i] = perifericosTemp[i];
	for ( int i = posicion + 1 ; i < cant_pt ; i++ )
		aux[i-1] = perifericosTemp[i];

	delete[] perifericosTemp;
	cant_pt--;

	perifericosTemp = aux;

}
/*
void PerifericoTemporizado::handler()
{
	if ( funcUsuario )
		funcUsuario();
}
*/
