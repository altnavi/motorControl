/*
 * PerifericoTemporizado.h
 *
 *  Created on: 29 jul. 2024
 *      Author: chor
 */

#ifndef PERIFERICOTEMPORIZADO_H_
#define PERIFERICOTEMPORIZADO_H_

#include <LPC845.h>

class PerifericoTemporizado {

	void (*funcUsuario) (void);

public:
	PerifericoTemporizado( void (*h) (void) = nullptr );
	~PerifericoTemporizado();

	virtual void handler ( void ) = 0;
	static uint8_t cant_pt;
};

extern PerifericoTemporizado **perifericosTemp;


#endif /* PERIFERICOTEMPORIZADO_H_ */
