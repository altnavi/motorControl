#include <DetectorGiro.h>

DetectorGiro *p1 = nullptr;

DetectorGiro::DetectorGiro(uint8_t port1, uint8_t pin1,uint8_t port2, uint8_t pin2)
: t1(1000, update),
  sensor1(port1, pin1, PinInt::flanco_asc,handler_sensor1),
  sensor2(port2, pin2, PinInt::flanco_asc, handler_sensor2)

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
    if(p1!=nullptr)
    {
        p1->pulse1++;
        p1->flag_sen1 = true;

        if(!p1->flag_sen2)
        {
            p1->sentido = p1->HORARIO;
        }
        else
        {
            p1->sentido = p1->ANTIHORARIO;
        }

        p1->flag_sen2 = false;
    }
}

void handler_sensor2(void)
{
    if(p1!=nullptr)
    {
        p1->flag_sen2 = true;

        if(!p1->flag_sen1)
        {
            p1->sentido = p1->ANTIHORARIO;
        }
        else
        {
            p1->sentido = p1->HORARIO;
        }

        p1->flag_sen1 = false;
    }
}

void update(void) //reinicio los flags
{
		p1->pulse1_seg = p1->pulse1; //cantidad de pulsos por seg para calcular las rpm
		p1->pulse1 = 0;
		p1->t1.start(); // reinicio el timer
}
