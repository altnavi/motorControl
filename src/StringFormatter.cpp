#include "StringFormatter.h"

/*
 * ===================================================================
 * Funciones auxiliares (privadas a este archivo)
 * ===================================================================
 */

/**
 * Escribe una cadena (src) en el puntero del búfer (p_puntero)
 * y avanza ese puntero.
 */
static bool append_str(char** p_puntero, char* fin, const char* src) {
    char* p = *p_puntero;
    while (*src) {
        if (p >= fin) return false;
        *p = *src;
        p++;
        src++;
    }
    *p_puntero = p;
    return true;
}

/**
 * Escribe un entero (num) en el puntero del búfer (p_puntero).
 */
static bool append_int(char** p_puntero, char* fin, int32_t num) {
    char* p = *p_puntero;
    char temp[11]; // Suficiente para un int32_t con signo (-2147483648)
    int i = 0;

    if (num < 0) {
        if (num == -2147483648) {
             // Caso borde especial para INT_MIN
             return append_str(p_puntero, fin, "-2147483648");
        }
        num = -num;
        if (p >= fin) return false;
        *p = '-';
        p++;
    }

    // Genera dígitos al revés
    if (num == 0) {
        temp[i++] = '0';
    } else {
        while (num > 0) {
            temp[i++] = (num % 10) + '0';
            num /= 10;
        }
    }

    // Escribe los dígitos en el orden correcto
    while (i > 0) {
        if (p >= fin) return false;
        *p = temp[--i];
        p++;
    }

    *p_puntero = p;
    return true;
}


/*
 * ===================================================================
 * Funciones Públicas
 * ===================================================================
 */

// Función wrapper simple para usar en el LCD
void convertir_entero_a_texto(int32_t num, char* buffer) {
    char* p = buffer;
    // Pasamos un puntero de 'fin' muy lejano (virtualmente infinito)
    // ya que asumimos que el usuario dio un buffer suficiente (ej. 16 bytes).
    char* fin = buffer + 32;

    append_int(&p, fin, num);
    *p = '\0'; // Terminar string
}

int formato_json_economico(
    char* buffer,
    size_t buffer_size,
    int rpm,
    float temperatura_motor,
    int sentido_giro,
    bool flag_alarmatimer
) {
    char* p = buffer;
    char* fin = buffer + buffer_size - 1;

    if (!append_str(&p, fin, "{\"rpm\": ")) return -1;
    if (!append_int(&p, fin, rpm)) return -1;

    if (!append_str(&p, fin, ", \"temp\": ")) return -1;

    // --- Manejo del flotante (%.1f) ---
    int32_t temp_int = (int32_t)(temperatura_motor * 10.0f);
    if (temp_int < 0 && temp_int > -10) {
         // Caso especial para números entre -0.1 y -0.9 donde la división entera da 0
         if (!append_str(&p, fin, "-0")) return -1;
    } else {
         if (!append_int(&p, fin, temp_int / 10)) return -1;
    }

    if (!append_str(&p, fin, ".")) return -1;

    int32_t decimal = temp_int % 10;
    if (decimal < 0) decimal = -decimal;
    if (!append_int(&p, fin, decimal)) return -1;

    if (!append_str(&p, fin, ", \"sentido\": ")) return -1;
    if (!append_int(&p, fin, sentido_giro)) return -1;

    if (!append_str(&p, fin, ", \"alarma\": ")) return -1;
    if (!append_str(&p, fin, (flag_alarmatimer ? "true" : "false"))) return -1;

    if (!append_str(&p, fin, "}\r\n")) return -1;

    *p = '\0';
    return p - buffer;
}

int formato_cipsend_economico(char* buffer, size_t buffer_size, int lenPayload) {
    char* p = buffer;
    char* fin = buffer + buffer_size - 1;

    if (!append_str(&p, fin, "AT+CIPSEND=0,")) return -1;
    if (!append_int(&p, fin, lenPayload)) return -1;
    if (!append_str(&p, fin, "\r\n")) return -1;

    *p = '\0';
    return p - buffer;
}
