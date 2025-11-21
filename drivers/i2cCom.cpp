#include "i2cCom.h"

// Punteros globales por instancia
static I2C_COM* i2c_global[4] = { nullptr, nullptr, nullptr, nullptr };
static gpio debugPin(1, 2, gpio::SALIDA, gpio::LOW);

I2C_COM::I2C_COM(uint8_t nro,uint8_t sda_pin,uint8_t scl_pin,uint8_t slave)
    : buffer_tx(64),
      VCC(1,5,gpio::SALIDA,gpio::HIGH),
      SDA_PIN(sda_pin),
      SCL_PIN(scl_pin),
      slave_address(slave),
      start(false),
      sent_data(false),
	  nro_i2c(nro)
{
    i2c_global[nro] = this;

    // --- Selección del periférico ---
    switch(nro){
        case 0: i2c_direccion = I2C0; break;
        case 1: i2c_direccion = I2C1; break;
        case 2: i2c_direccion = I2C2; break;
        case 3: i2c_direccion = I2C3; break;
    }

    // Power ON
    VCC.setPIN();

    // --- Configuración de pines según I2C ---
    // (igual que tu código original, solo corregidos comentarios y orden)
    SYSCON->SYSAHBCLKCTRL0 |= (1 << 7); // habilito switch matrix

    if(nro == 0){
        // Pines fijos P0_10 y P0_11
        PINENABLE0 &= ~(3 << 12);

        IOCON->PIO[IOCON_INDEX[0][10]] &= ~(3 << 8);
        IOCON->PIO[IOCON_INDEX[0][11]] &= ~(3 << 8);

        SYSCON->FCLKSEL[5] = 0;
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 5);

        SYSCON->PRESETCTRL0 &= ~(1 << 5);
        SYSCON->PRESETCTRL0 |=  (1 << 5);
    }
    else if(nro == 1){
        PINASSIGN9 &= ~(0xFFFF << 16);
        PINASSIGN9 |= (SDA_PIN << 16) | (SCL_PIN << 24);

        IOCON->PIO[IOCON_INDEX[0][SDA_PIN]] &= ~(3<<3);
        IOCON->PIO[IOCON_INDEX[0][SCL_PIN]] &= ~(3<<3);

        IOCON->PIO[IOCON_INDEX[0][SDA_PIN]] |= (2<<3) | (1<<10);
        IOCON->PIO[IOCON_INDEX[0][SCL_PIN]] |= (2<<3) | (1<<10);

        SYSCON->FCLKSEL[6] = 0;
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 21);

        SYSCON->PRESETCTRL0 &= ~(1 << 21);
        SYSCON->PRESETCTRL0 |=  (1 << 21);
    }
    else if(nro == 2){
        PINASSIGN10 &= ~0xFFFF;
        PINASSIGN10 |= (sda_pin | (scl_pin << 8));

        IOCON->PIO[IOCON_INDEX[0][SDA_PIN]] &= ~(3<<3);
        IOCON->PIO[IOCON_INDEX[0][SCL_PIN]] &= ~(3<<3);

        IOCON->PIO[IOCON_INDEX[0][SDA_PIN]] |= (2<<3) | (1<<10);
        IOCON->PIO[IOCON_INDEX[0][SCL_PIN]] |= (2<<3) | (1<<10);

        SYSCON->FCLKSEL[7] = 0;
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 22);

        SYSCON->PRESETCTRL0 &= ~(1 << 22);
        SYSCON->PRESETCTRL0 |=  (1 << 22);
    }
    else if(nro == 3){
        PINASSIGN10 &= ~(0xFFFF << 16);
        PINASSIGN10 |= (sda_pin << 16) | (scl_pin << 24);

        IOCON->PIO[IOCON_INDEX[0][SDA_PIN]] &= ~(3<<3);
        IOCON->PIO[IOCON_INDEX[0][SCL_PIN]] &= ~(3<<3);

        IOCON->PIO[IOCON_INDEX[0][SDA_PIN]] |= (2<<3) | (1<<10);
        IOCON->PIO[IOCON_INDEX[0][SCL_PIN]] |= (2<<3) | (1<<10);

        SYSCON->FCLKSEL[8] = 0;
        SYSCON->SYSAHBCLKCTRL0 |= (1 << 23);

        SYSCON->PRESETCTRL0 &= ~(1 << 23);
        SYSCON->PRESETCTRL0 |=  (1 << 23);
    }

    // --- Frecuencia I2C = 100 kHz ---
    i2c_direccion->CLKDIV = 74;
    i2c_direccion->MSTTIME = 0;

    // Habilitar maestro
    i2c_direccion->CFG |= 1;

    // No habilito IRQ del NVIC aquí, solo cuando enviamos.
}

I2C_COM::~I2C_COM(){
    // Nada en particular (bufferCirc se destruye solo)
}

// =======================================================
// ==============   ESCRITURA I2C (TX)   =================
// =======================================================

void I2C_COM::write(uint8_t byte)
{
    if(!buffer_tx.push(byte)) return;

    if(start) return; // Si ya está andando, solo cargamos al buffer

    if(buffer_tx.available() == 0) return;

    start = true;
    sent_data = false;

    // 1. Cargar dirección y bit de escritura
    i2c_direccion->MSTDAT = (slave_address << 1);
    // 2. Iniciar la transacción (START)
    i2c_direccion->MSTCTL = 0x02; // Bit 1 = Start (0x02 es Start normal, 0x01 es Continue)
    // NOTA: En LPC845, escribir 1 en bit 1 de MSTCTL fuerza el START.

    // 3. Habilitar Interrupción
    i2c_direccion->INTENSET = (1 << 0); // MSTPENDINGEN

    // 4. Habilitar NVIC (Usar |= para no borrar otras interrupciones)
    ISER0 |= (1 << (8 + nro_i2c));
}

void I2C_COM::I2C_handler()
{
    // CORRECCIÓN 1: Leer el estado correctamente (bits 3:1)
    uint8_t stat = (i2c_direccion->STAT >> 1) & 0x07;

    // Definiciones de estado según manual LPC845 (valores decimales tras el shift)
    const uint8_t I2C_STAT_IDLE = 0;
    const uint8_t I2C_STAT_RXRDY = 1;
    const uint8_t I2C_STAT_TXRDY = 2;
    const uint8_t I2C_STAT_NACK_ADDR = 3;
    const uint8_t I2C_STAT_NACK_DATA = 4;

    // Debug: Parpadear LED para ver vida
    if(stat == I2C_STAT_TXRDY) debugPin.togglePIN(); // Toggle

    switch(stat)
    {
        case I2C_STAT_IDLE:
            // Si estamos Idle pero hay datos, forzamos START
            if(buffer_tx.available() > 0) {
                i2c_direccion->MSTDAT = (slave_address << 1);
                i2c_direccion->MSTCTL = 0x02; // Start
                start = true;
            } else {
                start = false;
            }
        break;

        case I2C_STAT_TXRDY: // (Estado 2)
        {
            int16_t data = buffer_tx.pop();
            if(data != bufferCirc::NO_DATA)
            {
                i2c_direccion->MSTDAT = (uint8_t)data;
                i2c_direccion->MSTCTL = 0x01; // Continue
            }
            else
            {
                i2c_direccion->MSTCTL = 0x04; // Stop (Bit 2)
                start = false;
            }
        }
        break;

        case I2C_STAT_NACK_ADDR: // (Estado 3)
        case I2C_STAT_NACK_DATA: // (Estado 4)
            i2c_direccion->MSTCTL = 0x04; // Stop
            start = false;
            debugPin.clrPIN(); // Apagar LED si falla
        break;

        default:
            // Si pasa algo raro, Stop por seguridad
             i2c_direccion->MSTCTL = 0x04;
        break;
    }

    // Limpiar flag de interrupción si fuera necesario (en I2C LPC845 suele ser automático al escribir MSTCTL)
}

// =======================================================
// ===============    IRQ HANDLERS    ====================
// =======================================================

extern "C"{

void I2C0_IRQHandler(void){
    if(i2c_global[0]) i2c_global[0]->I2C_handler();
}
void I2C1_IRQHandler(void){
    if(i2c_global[1]) i2c_global[1]->I2C_handler();
}
void I2C2_IRQHandler(void){
    if(i2c_global[2]) i2c_global[2]->I2C_handler();
}
void I2C3_IRQHandler(void){
    if(i2c_global[3]) i2c_global[3]->I2C_handler();
}

}
