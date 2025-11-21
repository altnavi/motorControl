#ifndef I2CCOM_H_
#define I2CCOM_H_

#if defined (__cplusplus)
extern "C"{
	void I2C0_IRQHandler(void);
	void I2C1_IRQHandler(void);
	void I2C2_IRQHandler(void);
	void I2C3_IRQHandler(void);
}
#endif

#define IDLE 0x1
#define RECEIVE_READY 0x3
#define TRANSMIT_READY 0x5
#define NACK_ADDRESS 0x7
#define NACK_DATA 0x9

#include "timer.h"
#include "bufferCirc.h"
#include "gpio.h"
//CONTROLADOR PCF8574T...
//CLASE PARA I2C0...
class I2C_COM{
	public:

		I2C_COM(uint8_t nro = 0,uint8_t sda_pin = 11,uint8_t scl_pin = 10,uint8_t slave = 0x27); //BUILDER...
		~I2C_COM(); //DESTROYER...
		void write(uint8_t byte); //ESCRITURA...

	private:

		bufferCirc buffer_tx; //BUFFER CIRCULAR DE TRANSMISION...
		gpio VCC; //PIN DE ALIMENTACION...
		uint8_t SDA_PIN; //PIN PARA SEÑAL DEL CLOCK...
		uint8_t SCL_PIN; //PIN PARA SEÑAL DE DATOS...
		uint8_t slave_address; //DIRECCION DE ESCLAVO...
		I2C * i2c_direccion; //PERIFERICO I2C CORRESPONDIENTE...
		uint8_t _irq_n;
		uint8_t _irq_bit;
		void I2C_handler(void); //HANDLER...
		friend void I2C0_IRQHandler(void);
		friend void I2C1_IRQHandler(void);
		friend void I2C2_IRQHandler(void);
		friend void I2C3_IRQHandler(void);
		const uint8_t IOCON_INDEX [2][32] = {
			{17,11,6,5,4,3,16,15,14,13,8,7,2,1,18,10,9,0,30,29,28,27,26,25,24,23,22,21,20,50,51,35},
			{36,37,38,41,42,43,46,49,31,32,55,54,33,34,39,40,44,45,47,48,52,53,0,0,0,0,0,0,0,0,0,0}
		};
};

#endif /* I2CCOM_H_ */
