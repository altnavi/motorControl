#include "Temperatura.h"
const float R_TERM_DATA_KILOHM[101] = {
    31.77, 30.25, 28.82, 27.45, 26.16, 24.94, 23.77, 22.67, 21.62, 20.63, // 0-9
    19.68, 18.78, 17.93, 17.12, 16.35, 15.62, 14.93, 14.26, 13.63, 13.04, // 10-19
    12.47, 11.92, 11.41, 10.91, 10.45, 10.00, 9.575, 9.170, 8.784, 8.416, // 20-29
    8.064, 7.730, 7.410, 7.106, 6.815, 6.538, 6.273, 6.020, 5.778, 5.548, // 30-39
    5.327, 5.117, 4.915, 4.723, 4.539, 4.363, 4.195, 4.034, 3.880, 3.733, // 40-49
    3.592, 3.457, 3.328, 3.204, 3.086, 2.972, 2.863, 2.759, 2.659, 2.564, // 50-59
    2.472, 2.384, 2.299, 2.218, 2.141, 2.066, 1.994, 1.926, 1.860, 1.796, // 60-69
    1.735, 1.677, 1.621, 1.567, 1.515, 1.465, 1.417, 1.371, 1.326, 1.284, // 70-79
    1.243, 1.203, 1.165, 1.128, 1.093, 1.059, 1.027, 0.9955, 0.9654, 0.9363, // 80-89
    0.9083, 0.8812, 0.8550, 0.8297, 0.8052, 0.7816, 0.7587, 0.7366, 0.7152, 0.6945, // 90-99
    0.6744 // 100
};

Temperatura::Temperatura(){
	valor_temperatura = 0;

	for (int i = 0; i < CANT_DIVISIONES; i++)
	{
		tabla[i].min = R_TERM_DATA_KILOHM[i];     	//Resistencia superior del rango
		tabla[i].max = R_TERM_DATA_KILOHM[i + 1]; 	//Resistencia inferior del rango
		tabla[i].valor = i; 						//Temperatura es el índice (0 a 100)
	}

}

uint32_t Temperatura::getTemperatura() {
    // --- CÁLCULO DE RESISTENCIA ---

    // Asumimos que la lectura del ADC (cuentas) ya está en 'cuentas'
    // Cuentas de ejemplo (2500, ajustar según tu main/ADC)
    cuentas = 2500.0; 							//DEBE SER EL VALOR "RESULTADO" DE ADC.CPP

    float Vin = (cuentas / 4096.0) * Vref;

    if (Vin == 0.0) {			//Si Vin es 0 pongo un valor alto
        Rterm_kOhm = 1000.0;
    } else {
        Rterm_kOhm = Rfija_kOhm * (Vref / Vin - 1.0);
    }

    for (int i = 0; i < CANT_DIVISIONES; ++i) {
        // En un NTC, la resistencia disminuye al aumentar la temperatura.
        // La tabla[i] cubre el rango de temperatura entre i y i+1.
        // Por lo tanto, el Rterm calculado debe estar entre:
        // tabla[i].max (Resistencia a T=i+1) y tabla[i].min (Resistencia a T=i)

        if (Rterm_kOhm >= tabla[i].max && Rterm_kOhm <= tabla[i].min) {
            // El valor_temperatura i representa el límite inferior del rango.
            valor_temperatura = tabla[i].valor;
            break;
        }
    }

    // --- MANEJO DE LÍMITES (0°C y 100°C) ---
    // Si Rterm_kOhm es mayor que R@0°C, estamos por debajo de 0°C
    if (Rterm_kOhm > R_TERM_DATA_KILOHM[0]) {
        valor_temperatura = 0;
    }
    // Si Rterm_kOhm es menor que R@100°C, estamos por encima de 100°C
    else if (Rterm_kOhm < R_TERM_DATA_KILOHM[100]) {
        valor_temperatura = 100;
    }

    // Devolvemos el valor entero. Si usaste la interpolación, haz un casting:
    return (uint32_t)valor_temperatura;
}

void Temperatura::handler(){

}
