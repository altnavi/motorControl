/*
 * LcdI2C.h
 *
 *  Created on: 22 nov. 2025
 *      Author: Luciano
 */


#ifndef LCDI2C_H_
#define LCDI2C_H_

#include <i2cCom.h>
#include <StringFormatter.h> // Para imprimir números
#include <LPC845.h>

// Comandos
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// Flags para Entry Mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Flags para Display Control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// Flags para Display/Cursor Shift
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// Flags para Function Set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

// Backlight
#define LCD_BACKLIGHT           0x08
#define LCD_NOBACKLIGHT         0x00

class LcdI2C {
public:
    // Constructor: Recibe puntero al driver I2C existente
	LcdI2C(I2C_COM* i2c_interface);
    // Inicialización
    void init(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
    void clear();
    void home();

    // Control de Pantalla
    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();

    // Scroll y Orientación
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();
    void setCursor(uint8_t col, uint8_t row);

    void backlight();
    void noBacklight();

    void print(const char* str);
    void print(uint32_t num);
    void print(float num, int decimals = 1); //imprime floats

private:
    void command(uint8_t value);
    void send(uint8_t value, uint8_t mode);
    void write4bits(uint8_t value);
    void expanderWrite(uint8_t _data);
    void pulseEnable(uint8_t _data);

    // funciones para retardo
    void delay_ms(uint32_t ms);
    void delay_us(uint32_t us);

    I2C_COM* _i2c;
    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;
    uint8_t _cols;
    uint8_t _rows;
    uint8_t _backlightval;
};

#endif
