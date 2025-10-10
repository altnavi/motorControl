/*
 * Timer.h
 *
 *  Created on: 11 jun. 2023
 *      Author: chor
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <LPC845.h>
#include <PerifericoTemporizado.h>

class Timer : public PerifericoTemporizado {

	private:
		uint32_t tiempo, recarga;
		bool recargar;
		void (*Handler) (void);

	public:
		Timer ( uint32_t _tiempo , void (*_Handler) (void) , bool _recargar = false );
		~Timer();

		void start ();
		void stop ();
		void handler ( void );
		void setTime(uint32_t _time);

};

extern Timer **timers;

#endif /* TIMER_H_ */
