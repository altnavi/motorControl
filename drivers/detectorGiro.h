#ifndef DETECTORGIRO_H_
#define DETECTORGIRO_H_

#include "LPC845.h"
#include "PinInt.h"
#include "Timer.h"
#include "PerifericoTemporizado.h"

void handler_sensor1(void);
void handler_sensor2(void);
void update(void);

class DetectorGiro : PerifericoTemporizado {

public:
		DetectorGiro(uint8_t port1, uint8_t pin1, uint8_t port2, uint8_t pin2);
		uint32_t getRPM();
		uint8_t getSentidoGiro();
		virtual ~DetectorGiro();
		friend void handler_sensor1(void);
		friend void handler_sensor2(void);
		void procesar();
		void handler(void);

		enum Sentido { HORARIO, ANTIHORARIO, DETENIDO};

protected:
		PinInt sensor1;
		PinInt sensor2;

	    static constexpr uint32_t TIMEOUT_MOTOR_DETENIDO_MS = 5000;
	    static constexpr uint32_t ANTIREBOTE_MS = 5         <0;
	    static constexpr uint8_t PROMEDIO = 4; // número de periodos a promediar

	    uint8_t sentido;
	    uint8_t estado_encoder;

	    volatile bool isr_flag_s1;
	    volatile bool isr_flag_s2;

	    uint32_t periodos_hist[PROMEDIO] = {0};
	    uint8_t idx_periodo = 0;

		volatile uint32_t tiempo_ultimo_pulso_ms; // Hora del último pulso
		volatile uint32_t periodo_ms;             // Tiempo entre los dos últimos pulsos
		volatile uint32_t system_millis;
		uint32_t tiempo_debounce_S1;
		uint32_t tiempo_debounce_S2;

		void calcularPeriodo(uint32_t ahora);
};

#endif /* DETECTORGIRO_H_ */
