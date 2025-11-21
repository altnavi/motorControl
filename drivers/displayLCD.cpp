#include "displayLCD.h"
//LOS DELAYS EN MICROSEGUNDOS SE APROXIMAN AL VALOR MAS CERCANO EN MILISEGUNDOS...
//FORMATO DEL ENVIADO POR I2C: D7 - D6 - D5 - D4 - BL - E - RW - RS
displayLCD::displayLCD() : expander(0,11,10,0x27){
	text = nullptr;
	size = 0;
	tiempo = 0;
	flag = false;
	//tasks = nullptr;
	/*
    // Esperar 40ms luego de encender
    delay = 40;
    while(delay);

    sendNibble(0x03, false);
    delay = 5;
    while(delay);

    sendNibble(0x03, false);
    delay = 5;
    while(delay);

    sendNibble(0x03, false);
    delay = 1;
    while(delay);

    sendNibble(0x02, false);

    sendByte(SET_FUNCTION | M_4BITS | LINES_2 | SIZE_5x8, true);  // 0x28
    sendByte(SET_LCD_CONTROL | DISPLAY_OFF, true);                // 0x08
    sendByte(CLEAR, true);                                       // 0x01
    sendByte(SET_ENTRY_MODE | LEFT | NO_SHIFT, true);            // 0x06
    sendByte(SET_LCD_CONTROL | DISPLAY_ON | CURSOR_ON | BLINK_ON, true); // 0x0f*/
}
void displayLCD::sendNibble(uint8_t nibble,bool rs){
	uint8_t dat = (nibble << 4);
	dat |= BACKLIGHT;
	if(rs)
		dat |= RS;
	dat |= ENABLE; //ENABLE EN 1...

	expander.write(dat);
	expander.write(dat);
	//delay = 1;
	//while(delay);
	dat &= ~ENABLE; //ENABLE EN 0...
	expander.write(dat);
	expander.write(dat);
	//delay = 1;
	//while(delay);

	//LAS LLAMO 2 VECES - ENTRE UNA Y OTRA PASA UN TIEMPO EN uS MAYOR A 450 nS...*/
}

void displayLCD::sendByte(uint8_t byte,bool command){
	uint8_t high = byte >> 4;
	uint8_t low = byte & 0x0f;

	sendNibble(high,command);
	//delay = 1;
	//while(delay);
	sendNibble(low,command);
	if(byte == CLEAR || byte == HOME){

		//while(delay);
	}
	else{

		//while(delay);
	}
}

void displayLCD::handler(){

	if(tiempo>=1)
	{
		flag= true;
	}

}

displayLCD::~displayLCD(){
	delete [] text;
}

void displayLCD::home(){
	//addProcess(HOME,true,2);
}

void displayLCD::clear(){
	//addProcess(CLEAR,true,2);
}

void displayLCD::turnDisplay(uint8_t mode){
	if(mode == On){
		//addProcess(SET_LCD_CONTROL | DISPLAY_ON | CURSOR_ON | BLINK_ON, true);
	}
	else if(mode == Out){
		//addProcess(SET_LCD_CONTROL | DISPLAY_OFF | CURSOR_OFF | BLINK_OFF, true);
	}
}

void displayLCD::setCursor(uint8_t address){
	if((address >= L1C1 && address <= L1C16) || (address >= L2C1 && address <= L2C16)){
		//addProcess(SET_DDRAM_ADDRESS | address,true);
	}
}

void displayLCD::print(const uint8_t * string,uint8_t size_){
	if(size_ >= 32){
		size_ = 32;
	}
	size = size_;
	text = new uint8_t [size];
	if(size >= 16 && size <= 32){ //EL TEXTO POSEE MAS DE 16 CARACTERES...
		for(uint8_t i = 0;i < 16;i++){
			text[i] = string[i];
			//addProcess(text[i],false); //PRIMERO ACUMULO LOS CARACTERES QUE QUIERO IMPRIMIR EN PRIMERA LINEA...
		}
		//addProcess(SET_DDRAM_ADDRESS | L2C1,true); //ACUMULO LA INSTRUCCION DE SETEO DEL CURSOR EN NUEVA LINEA...
		for(uint8_t i = 16;i < size;i++){
			text[i] = string[i];
			//addProcess(text[i],false); //ACUMULO LOS CARACTERES QUE QUIERO IMPRIMIR EN SEGUNDA LINEA...
		}
	}
	else{ //EL TEXTO POSEE MENOS DE 16 CARACTERES...
		for(uint8_t i = 0;i < size;i++){
			text[i] = string[i];
			//addProcess(text[i],false); //PRIMERO ACUMULO LOS CARACTERES QUE QUIERO IMPRIMIR EN PRIMERA LINEA...
		}
	}
}
