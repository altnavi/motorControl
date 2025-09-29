#include <DetectorGiro.h>

DetectorGiro *DetectorGiro::p1 = nullptr;

DetectorGiro::DetectorGiro(uint8_t port1, uint8_t pin1,uint8_t port2, uint8_t pin2)
: sensor1(port1, pin1, PinInt::flanco_desc,handler_sensor1),
  sensor2(port2, pin2, PinInt::flanco_desc, handler_sensor2),
  t1(1000, update)

{
  p1 = this;
  t1.start();
}

DetectorGiro::~DetectorGiro() {

}

uint32_t DetectorGiro::getRPM()
{
	uint32_t aux = pulse1_seg * 60;
	return aux;
}

uint8_t DetectorGiro::getSentidoGiro()
{
	return sentido;
}

void handler_sensor1(void)
{
	DetectorGiro::p1->pulse1++;
	DetectorGiro::p1->flag_sen1 = true;

	if(!DetectorGiro::p1->flag_sen2)
	{
		DetectorGiro::p1->sentido = DetectorGiro::p1->HORARIO;
	}
	else
	{
		DetectorGiro::p1->sentido = DetectorGiro::p1->ANTIHORARIO;
	}

	DetectorGiro::p1->flag_sen2 = false;
}

void handler_sensor2(void)
{
	//DetectorGiro::p1->pulse2++;
	DetectorGiro::p1->flag_sen2 = true;

	if(!DetectorGiro::p1->flag_sen1)
	{
		DetectorGiro::p1->sentido = DetectorGiro::p1->ANTIHORARIO;
	}
	else
	{
		DetectorGiro::p1->sentido = DetectorGiro::p1->HORARIO;
	}

	DetectorGiro::p1->flag_sen1 = false;
}

void update(void) //reinicio los flags
{
		DetectorGiro::p1->pulse1_seg = DetectorGiro::p1->pulse1; //cantidad de pulsos por seg para calcular las rpm
		DetectorGiro::p1->pulse1 = 0;
		DetectorGiro::p1->t1.start(); // reinicio el timer
}
