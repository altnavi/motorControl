#include "LcdI2C.h"

#define HOME_CLEAR_EXEC 2000

LcdI2C::LcdI2C(I2C_COM* i2c_interface) {
    _i2c = i2c_interface;
    _backlightval = LCD_BACKLIGHT;
}

// --- RETARDOS ---
void LcdI2C::delay_ms(uint32_t ms) {
    for(volatile uint32_t i = 0; i < (ms); i++) { __asm("NOP"); }
}
void LcdI2C::delay_us(uint32_t us) {
    for(volatile uint32_t i = 0; i < (us * 6); i++) { __asm("NOP"); }
}

// --- INIT ---
void LcdI2C::init(uint8_t cols, uint8_t rows, uint8_t charsize) {
    _cols = cols;
    _rows = rows;
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

    if (rows > 1) _displayfunction |= LCD_2LINE;
    if ((charsize != 0) && (rows == 1)) _displayfunction |= LCD_5x10DOTS;

    delay_ms(100);

    // Reset HD44780
    write4bits(0x03 << 4); delay_us(4500);
    write4bits(0x03 << 4); delay_us(4500);
    write4bits(0x03 << 4); delay_us(150);
    write4bits(0x02 << 4);

    command(LCD_FUNCTIONSET | _displayfunction);

    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();
    clear();

    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// --- ALTO NIVEL ---
void LcdI2C::clear() {
    command(LCD_CLEARDISPLAY);
    delay_us(HOME_CLEAR_EXEC);
}
void LcdI2C::home() {
    command(LCD_RETURNHOME);
    delay_us(HOME_CLEAR_EXEC);
}
void LcdI2C::setCursor(uint8_t col, uint8_t row) {
    const uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if (row >= _rows) row = _rows - 1;
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// --- PANTALLA ---
void LcdI2C::noDisplay() { _displaycontrol &= ~LCD_DISPLAYON; command(LCD_DISPLAYCONTROL | _displaycontrol); }
void LcdI2C::display() { _displaycontrol |= LCD_DISPLAYON; command(LCD_DISPLAYCONTROL | _displaycontrol); }
void LcdI2C::noCursor() { _displaycontrol &= ~LCD_CURSORON; command(LCD_DISPLAYCONTROL | _displaycontrol); }
void LcdI2C::cursor() { _displaycontrol |= LCD_CURSORON; command(LCD_DISPLAYCONTROL | _displaycontrol); }
void LcdI2C::noBlink() { _displaycontrol &= ~LCD_BLINKON; command(LCD_DISPLAYCONTROL | _displaycontrol); }
void LcdI2C::blink() { _displaycontrol |= LCD_BLINKON; command(LCD_DISPLAYCONTROL | _displaycontrol); }

// --- SCROLL ---
void LcdI2C::scrollDisplayLeft() { command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT); }
void LcdI2C::scrollDisplayRight() { command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT); }
void LcdI2C::leftToRight() { _displaymode |= LCD_ENTRYLEFT; command(LCD_ENTRYMODESET | _displaymode); }
void LcdI2C::rightToLeft() { _displaymode &= ~LCD_ENTRYLEFT; command(LCD_ENTRYMODESET | _displaymode); }
void LcdI2C::autoscroll() { _displaymode |= LCD_ENTRYSHIFTINCREMENT; command(LCD_ENTRYMODESET | _displaymode); }
void LcdI2C::noAutoscroll() { _displaymode &= ~LCD_ENTRYSHIFTINCREMENT; command(LCD_ENTRYMODESET | _displaymode); }


void LcdI2C::backlight() { _backlightval = LCD_BACKLIGHT; expanderWrite(0); }
void LcdI2C::noBacklight() { _backlightval = LCD_NOBACKLIGHT; expanderWrite(0); }

// --- PRINT ---
void LcdI2C::print(const char* str) {
    while (*str) send((uint8_t)*str++, 1);
}
void LcdI2C::print(uint32_t num) {
    char buffer[16];
    convertir_entero_a_texto(num, buffer);
    print(buffer);
}

void LcdI2C::print(float num, int decimals) {
    uint32_t parte_entera = (int)num;
    uint32_t parte_decimal = (int)((num - parte_entera) * 10);
    if(parte_decimal < 0) parte_decimal = -parte_decimal;
    print(parte_entera);
    print(".");
    print(parte_decimal);
}

// --- BAJO NIVEL ---
void LcdI2C::command(uint8_t value) { send(value, 0); }

void LcdI2C::send(uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xF0;
    uint8_t lownib = (value << 4) & 0xF0;
    write4bits((highnib) | mode);
    write4bits((lownib) | mode);
}

void LcdI2C::write4bits(uint8_t value) {
    expanderWrite(value);
    pulseEnable(value);
}

void LcdI2C::expanderWrite(uint8_t _data) {
    _i2c->write((_data) | _backlightval);
}

void LcdI2C::pulseEnable(uint8_t _data) {
    expanderWrite(_data | 0x04);
    expanderWrite(_data & ~0x04);

    // FIX DE BRILLO
    if (_backlightval == LCD_BACKLIGHT) _i2c->write(0xFB);
    else _i2c->write(0xF3);
}
