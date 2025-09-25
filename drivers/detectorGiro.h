#ifndef DETECTORGIRO_H_
#define DETECTORGIRO_H_

#include "../firmware/LPC845.h"
#include "../drivers/PinInt.h"

void handler(void);

class detectorGiro {
	PinInt sensor;
	uint32_t rpm;

public:
	detectorGiro(uint8_t port, uint8_t pin);
	virtual ~detectorGiro();
	friend void handler(void);
};

#endif /* DETECTORGIRO_H_ */
