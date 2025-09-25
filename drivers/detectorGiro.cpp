#include <detectorGiro.h>

void handler_detectorGiro (void)
{

}


detectorGiro::detectorGiro(uint8_t port, uint8_t pin):sensor(port, pin, PinInt::flanco_asc, handler)
{

}
