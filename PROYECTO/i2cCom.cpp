#include "i2cCom.h"

I2C_COM * i2c_global[4] = {nullptr, nullptr, nullptr, nullptr};

// Índices para mapeo de pines (asumiendo tu estructura anterior)
// Asegúrate de que IOCON_INDEX esté definido en algún lado accesible
extern uint8_t IOCON_INDEX[1][32]; // O como lo tengas definido

I2C_COM::I2C_COM(uint8_t nro, uint8_t sda_pin, uint8_t scl_pin, uint8_t slave)
    : buffer_tx(64), // Aumenté el buffer a 64 bytes para mensajes largos de LCD
      VCC(0, 20, gpio::SALIDA, gpio::HIGH)
{
    i2c_global[nro] = this;
    slave_address = slave;
    VCC.setPIN();
    SDA_PIN = sda_pin;
    SCL_PIN = scl_pin;

    // 1. Habilitar reloj de la Switch Matrix
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 7);

    if (nro == 0) {
        i2c_direccion = I2C0;
        _irq_bit = 8; // I2C0 usa el bit 8 del NVIC

        PINENABLE0 &= ~(3 << 12); // P0.10 y P0.11 fijos
        IOCON->PIO[IOCON_INDEX[0][10]] &= ~(3 << 8);
        IOCON->PIO[IOCON_INDEX[0][11]] &= ~(3 << 8);

        SYSCON->FCLKSEL[5] = 0; // Clock source = Main Clock
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 5); // Clock I2C0
        SYSCON->PRESETCTRL0 &= ~(1 << 5);   // Reset assert
        SYSCON->PRESETCTRL0 |= (1 << 5);    // Reset deassert
    }
    else if (nro == 1) {
        i2c_direccion = I2C1;
        _irq_bit = 7; // I2C1 usa el bit 7 del NVIC

        PINASSIGN9 &= ~(0xffff << 16);
        PINASSIGN9 |= (SDA_PIN << 16);
        PINASSIGN9 |= (SCL_PIN << 24);

        IOCON->PIO[IOCON_INDEX[0][SDA_PIN]] = (2 << 3) | (1 << 10); // Pull-up + Open Drain
        IOCON->PIO[IOCON_INDEX[0][SCL_PIN]] = (2 << 3) | (1 << 10);

        SYSCON->FCLKSEL[6] = 0;
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 21);
        SYSCON->PRESETCTRL0 &= ~(1 << 21);
        SYSCON->PRESETCTRL0 |= (1 << 21);
    }
    else if (nro == 2) {
        i2c_direccion = I2C2;
        _irq_bit = 21; // I2C2 usa el bit 21 del NVIC

        PINASSIGN10 &= ~0xffff;
        PINASSIGN10 |= SDA_PIN;
        PINASSIGN10 |= (SCL_PIN << 8);

        IOCON->PIO[IOCON_INDEX[0][SDA_PIN]] = (2 << 3) | (1 << 10);
        IOCON->PIO[IOCON_INDEX[0][SCL_PIN]] = (2 << 3) | (1 << 10);

        SYSCON->FCLKSEL[7] = 0;
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 22);
        SYSCON->PRESETCTRL0 &= ~(1 << 22);
        SYSCON->PRESETCTRL0 |= (1 << 22);
    }
    else if (nro == 3) {
        i2c_direccion = I2C3;
        _irq_bit = 22; // I2C3 usa el bit 22 del NVIC

        PINASSIGN10 &= ~(0xffff << 16);
        PINASSIGN10 |= (SDA_PIN << 16);
        PINASSIGN10 |= (SCL_PIN << 24);

        IOCON->PIO[IOCON_INDEX[0][SDA_PIN]] = (2 << 3) | (1 << 10);
        IOCON->PIO[IOCON_INDEX[0][SCL_PIN]] = (2 << 3) | (1 << 10);

        SYSCON->FCLKSEL[8] = 0;
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 23);
        SYSCON->PRESETCTRL0 &= ~(1 << 23);
        SYSCON->PRESETCTRL0 |= (1 << 23);
    }

    // Configuración de velocidad
    // Para 30MHz -> 100kHz, CLKDIV = 74
    i2c_direccion->CLKDIV = 74;
    i2c_direccion->MSTTIME = 0;

    // Habilitar Maestro
    i2c_direccion->CFG |= (1 << 0);

    // Habilitar interrupción en el NVIC (Global)
    // Usamos _irq_bit dinámicamente en lugar de hardcodear el 8
    ISER0 |= (1 << _irq_bit);
}

I2C_COM::~I2C_COM(){
    // Deshabilitar interrupción en el NVIC
    ICER0 |= (1 << _irq_bit);
    buffer_tx.~bufferCirc();
}

void I2C_COM::write(uint8_t byte){
    buffer_tx.push(byte);

    // Verificamos el estado del Maestro (MSTSTATE bits 1-3 del registro STAT)
    uint8_t state = (i2c_direccion->STAT >> 1) & 0x07;

    // Si el estado es IDLE (0), significa que nadie está usando el bus.
    // Debemos iniciar la transmisión manualmente.
    if (state == 0) {
        // Configuramos Address + Write
        i2c_direccion->MSTDAT = (slave_address << 1);
        // Start bit
        i2c_direccion->MSTCTL = (1 << 1);
        // Habilitamos interrupción del periférico (MSTPENDING)
        i2c_direccion->INTENSET = (1 << 0);
    }
    // Si NO es IDLE (ya está transmitiendo), la interrupción saltará sola
    // cuando termine el byte actual y tomará el nuevo dato del buffer.
}

void I2C_COM::I2C_handler(){
    uint32_t stat_reg = i2c_direccion->STAT;
    uint8_t state = (stat_reg >> 1) & 0x07; // MSTSTATE
    int16_t data = 0;

    // Estado 0: IDLE (No deberíamos estar aquí por interrupción usualmente, salvo error)
    if (state == 0) {
        i2c_direccion->INTENCLR = (1 << 0); // Apagar interrupciones
    }
    // Estado 1: RX Ready (No implementado para escribir al LCD)

    // Estado 2: TX READY (Se envió Dirección o Dato y recibimos ACK)
    else if (state == 2) {
        data = buffer_tx.pop(); // Sacar siguiente dato

        if (data != bufferCirc::NO_DATA) {
            // Aún hay datos: Enviar Dato + Continue
            i2c_direccion->MSTDAT = (uint8_t)data;
            i2c_direccion->MSTCTL = (1 << 0);
        }
        else {
            // Buffer vacío: Enviar STOP
            i2c_direccion->MSTCTL = (1 << 2);
            // IMPORTANTE: Deshabilitar interrupción del periférico para no quedar loopeando
            i2c_direccion->INTENCLR = (1 << 0);
        }
    }
    // Estado 3 (NACK Addr) o 4 (NACK Data)
    else if (state == 3 || state == 4) {
        i2c_direccion->MSTCTL = (1 << 2);   // STOP
        i2c_direccion->INTENCLR = (1 << 0); // Apagar interrupciones
    }

    // Limpiar flag de interrupción pendiente escribiendo 1 en STAT
    // (Aunque escribir MSTCTL o MSTDAT suele limpiarlo automáticamente, es buena práctica en algunos casos)
}

// Handlers Globales (Sin cambios, solo aseguran la llamada)
void I2C0_IRQHandler(void){ if(i2c_global[0]) i2c_global[0]->I2C_handler(); }
void I2C1_IRQHandler(void){ if(i2c_global[1]) i2c_global[1]->I2C_handler(); }
void I2C2_IRQHandler(void){ if(i2c_global[2]) i2c_global[2]->I2C_handler(); }
void I2C3_IRQHandler(void){ if(i2c_global[3]) i2c_global[3]->I2C_handler(); }
