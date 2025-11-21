#ifndef DISPLAYLCD_H_
#define DISPLAYLCD_H_

//CONTROLADOR HITACHI HD4780
//LEER MANUAL SI SE TIENE TIEMPO...
//LCD 1602 CON EXPANSOR I2C...
#include "i2cCom.h"
#include "bufferCirc.h"
#include "PerifericoTemporizado.h"

//DIRECCIONES DEL CURSOR...
#define L1C1 0x00
#define L1C2 0x01
#define L1C3 0x02
#define L1C4 0x03
#define L1C5 0x04
#define L1C6 0x05
#define L1C7 0x06
#define L1C8 0x07
#define L1C9 0x08
#define L1C10 0x09
#define L1C11 0x0a
#define L1C12 0x0b
#define L1C13 0x0c
#define L1C14 0x0d
#define L1C15 0x0e
#define L1C16 0x0f
#define L2C1 0x40
#define L2C2 0x41
#define L2C3 0x42
#define L2C4 0x43
#define L2C5 0x44
#define L2C6 0x45
#define L2C7 0x46
#define L2C8 0x47
#define L2C9 0x48
#define L2C10 0x49
#define L2C11 0x4a
#define L2C12 0x4b
#define L2C13 0x4c
#define L2C14 0x4d
#define L2C15 0x4e
#define L2C16 0x4f
//MASCARAS DE COMANDOS//
#define CLEAR 0x01
#define HOME 0x02
//MASCARA PARA EDITAR MODO DE ENTRADA...
#define SET_ENTRY_MODE 0x04 //HACERLE UNA OR CON LOS BITS DE ENTRADA QUE SE QUIERAN CONFIGURAR...
//BITS DE MODO DE ENTRADA...
#define LEFT 0x02
#define RIGHT 0x00
#define SHIFT 0x01
#define NO_SHIFT 0x00
//MASCARA PARA CONTROL DE DISPLAY...
#define SET_LCD_CONTROL 0x08 //HACERLE UNA OR CON LOS BITS QUE SE QUIERAN CONFIGURAR...
//BITS DE CONTROL DE DISPLAY...
#define DISPLAY_ON 0x04
#define DISPLAY_OFF 0x00
#define CURSOR_ON 0x02
#define CURSOR_OFF 0x00
#define BLINK_ON 0x01
#define BLINK_OFF 0x00
//MASCARA PARA SETEAR SHIFT DEL CURSOR O DISPLAY...
#define SET_SHIFT 0x10 //HACERLE UNA OR CON LOS BITS QUE SE QUIERAN CONFIGURAR...
//BITS DE CONTROL DEL SHIFT...
#define DISPLAY_SHIFT 0x08
#define CURSOR_MOVE 0x00
#define TO_THE_LEFT 0x00
#define TO_THE_RIGHT 0x04
//MASCARA PARA SETEAR LA FUNCION DEL DISPLAY...
#define SET_FUNCTION 0x20 //HACERLE UNA OR CON LOS BITS QUE SE QUIERAN CONFIGURAR...
//BITS DE FUNCION...
#define M_8BITS 0x10
#define M_4BITS 0x00
#define LINES_1 0x00
#define LINES_2 0x08
#define SIZE_5x10 0x04
#define SIZE_5x8 0x00
//BITS DE ENABLE,RS Y RW...
#define ENABLE 0x04
#define RS 0x01
#define RW 0x02
//BITS DE BACKLIGHT
#define BACKLIGHT 0x08
#define NO_BACKLIGHT 0x00
//MASCARAS PARA SETEAR DIRECCIONES...
#define SET_DDRAM_ADDRESS 0x80 //HACERLE UNA OR CON LA DIRECCION - POSICIONAMIENTO DEL CURSOR...
#define SET_CGRAM_ADDRESS 0x40 //HACERLE UNA OR CON LA DIRECCION - CREACION DE CARACTER...

class displayLCD : public PerifericoTemporizado{
	public:
		displayLCD(); //BUILDER, INICIALIZADOR...
		~displayLCD(); //DESTROYER...
		enum mode{On,Out};
		void home(); //CURSOR AL INICIO...
		void clear(); //BORRO TODO - VOY AL INICIO...
		void turnDisplay(uint8_t mode); //APAGO O PRENDO DISPLAY...
		void turnCursor(uint8_t mode); //APAGO O PRENDO CURSOR...
		void setBlink(uint8_t mode); //APAGO O PRENDO PARPADEO...
		void setCursor(uint8_t address); //SETTER DEL CURSOR...
		void print(const uint8_t * string,uint8_t size); //IMPRIME EN PANTALLA...
	private:
		I2C_COM expander; //EXPANSOR I2C...
		uint8_t * text; //TEXTO ACTUAL...
		uint8_t size; //LARGO DEL TEXTO ACTUAL...
		uint32_t tiempo;
		bool flag;
		//uint8_t delay; //TIEMPO DE ESPERA POR INSTRUCCIONES...
		void sendNibble(uint8_t nibble,bool rs);
		void sendByte(uint8_t byte,bool command);
		void handler();
};
#endif /* DISPLAYLCD_H_ */
