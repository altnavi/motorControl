#include "Temperatura.h"

Temperatura::Temperatura(uint8_t canal) : A1(canal){
	Rfija = 4700.0;
	R0 = 6600.0;
	T0 = 292.75;
	beta = 4200.0;
	vref = 3.3;
	adc_res = 4095;
	muestras = 10;
	valor_temperatura = 0;
}

float Temperatura::getTemperatura() {
    // --- CÁLCULO DE RESISTENCIA ---

    // Asumimos que la lectura del ADC (cuentas) ya está en 'cuentas'
    // Cuentas de ejemplo (2500, ajustar según tu main/ADC)
    cuentas = A1.getValor(); 							//DEBE SER EL VALOR "RESULTADO" DE ADC.CPP

    float adc = cuentas;
    float vout = adc*(vref/adc_res);

    float rntc = Rfija * (vref-vout)/vout;
    float invT = (1.0/T0)+(1.0/beta)*log(rntc/R0);
    float Tkelvin = 1.0/invT;
    float Tcelsius = Tkelvin - 273.15;

    return Tcelsius;
}

void Temperatura::handler(){

}
