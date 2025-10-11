#ifndef DETECTORGIRO_H_
#define DETECTORGIRO_H_

#include "LPC845.h"
#include "PinInt.h"
#include "Timer.h"

void handler_sensor1(void);
void handler_sensor2(void);
void update(void);

class DetectorGiro {

public:
		enum giro{HORARIO, ANTIHORARIO, NO_DETECTADO};

		DetectorGiro(uint8_t port1, uint8_t pin1, uint8_t port2, uint8_t pin2);
		uint32_t getRPM();
		uint8_t getSentidoGiro();
		virtual ~DetectorGiro();
		friend void handler_sensor1(void);
		friend void handler_sensor2(void);
		friend void update(void);

protected:
		Timer t1;
		PinInt sensor1;
		PinInt sensor2;

		uint32_t pulse1;
		uint32_t pulse2;
		uint32_t pulse1_seg; //lo actualizo cada 1 seg
		uint32_t pulse2_seg; // por ahora no lo utilizo

		bool flag_sen1;
		bool flag_sen2;

		bool sentido_g;

		volatile giro sentido = NO_DETECTADO;
		volatile uint8_t ultimo_sensor = 0; // 0 = ninguno, 1 = sensor1, 2 = sensor2
};

#endif /* DETECTORGIRO_H_ */
