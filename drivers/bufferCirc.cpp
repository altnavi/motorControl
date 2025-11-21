// Improved bufferCirc implementation
// - push() returns bool (true = stored, false = buffer full)
// - pop() unchanged (returns int16_t, NO_DATA = -1)
// - added available(), freeSpace(), clear()
// - internal array size is fixed; 'tam' defines used range

#include <bufferCirc.h>

bufferCirc::bufferCirc(uint8_t cant_bytes)
{
    tam   = cant_bytes;
    in    = 0;
    out   = 0;
    lleno = false;
}

// Returns true if stored, false if full
bool bufferCirc::push(uint8_t dato)
{
    if (lleno)
        return false;

    buffer[in] = dato;
    in = (in + 1) % tam;

    if (in == out)
        lleno = true;

    return true;
}

int16_t bufferCirc::pop(void)
{
    int16_t ret = NO_DATA;

    if ((in != out) || lleno)
    {
        ret = buffer[out];
        out = (out + 1) % tam;
        lleno = false;
    }

    return ret;
}

bool bufferCirc::hayLugar()
{
    return !lleno;
}

uint8_t bufferCirc::available() const
{
    if (lleno)
        return tam;
    if (in >= out)
        return in - out;
    return tam - (out - in);
}

uint8_t bufferCirc::freeSpace() const
{
    return tam - available();
}

void bufferCirc::clear()
{
    in = 0;
    out = 0;
    lleno = false;
}
