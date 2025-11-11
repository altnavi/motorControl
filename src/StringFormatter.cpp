#include "StringFormatter.h"
#include <stdbool.h> // Necesario para 'bool'

/*
 * ===================================================================
 * Funciones auxiliares (privadas a este archivo)
 * Las marcamos como 'static' para que el compilador sepa
 * que solo se usan aquí. Esto ayuda a optimizar.
 * ===================================================================
 */

/**
 * Escribe una cadena (src) en el puntero del búfer (p_puntero)
 * y avanza ese puntero.
 * Comprueba que no se escriba más allá del puntero 'fin'.
 * Devuelve 'false' si no hay espacio.
 */
static bool append_str(char** p_puntero, char* fin, const char* src) {
    char* p = *p_puntero;
    while (*src) {
        if (p >= fin) return false;
        *p = *src;
        p++;
        src++;
    }
    *p_puntero = p; // Actualiza el puntero original
    return true;
}

/**
 * Escribe un entero (num) en el puntero del búfer (p_puntero).
 * Devuelve 'false' si no hay espacio.
 */
static bool append_int(char** p_puntero, char* fin, int32_t num) {
    char* p = *p_puntero;
    char temp[11]; // Suficiente para un int32_t con signo
    int i = 0;

    if (num < 0) {
        num = -num;
        if (p >= fin) return false;
        *p = '-'; // Escribe el signo
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

    *p_puntero = p; // Actualiza el puntero original
    return true;
}


/*
 * ===================================================================
 * Funciones Públicas (declaradas en StringFormatter.h)
 * ===================================================================
 */

/**
 * Implementación de la función de formato JSON.
 */
int formato_json_economico(
    char* buffer,
    size_t buffer_size,
    int rpm,
    float temperatura_motor,
    int sentido_giro,
    bool flag_alarmatimer
) {
    // Punteros para escritura segura
    char* p = buffer;
    char* fin = buffer + buffer_size - 1; // Deja 1 byte para el '\0'

    // {"rpm":
    if (!append_str(&p, fin, "{\"rpm\": ")) return -1;
    if (!append_int(&p, fin, rpm)) return -1;

    // ,"temp":
    if (!append_str(&p, fin, ", \"temp\": ")) return -1;

    // --- Manejo del flotante (%.1f) ---
    int32_t temp_int = (int32_t)(temperatura_motor * 10.0f);

    if (!append_int(&p, fin, temp_int / 10)) return -1; // Parte entera
    if (!append_str(&p, fin, ".")) return -1;

    int32_t decimal = temp_int % 10;
    if (decimal < 0) decimal = -decimal; // abs()
    if (!append_int(&p, fin, decimal)) return -1; // Parte decimal

    // ,"sentido":
    if (!append_str(&p, fin, ", \"sentido\": ")) return -1;
    if (!append_int(&p, fin, sentido_giro)) return -1;

    // ,"alarma":
    if (!append_str(&p, fin, ", \"alarma\": ")) return -1;
    if (!append_str(&p, fin, (flag_alarmatimer ? "true" : "false"))) return -1;

    // }\r\n
    if (!append_str(&p, fin, "}\r\n")) return -1;

    *p = '\0'; // Terminador nulo
    return p - buffer; // Devuelve el total de bytes escritos
}


/**
 * Implementación de la función de formato CIPSEND.
 */
int formato_cipsend_economico(
    char* buffer,
    size_t buffer_size,
    int lenPayload
) {
    // Punteros para escritura segura
    char* p = buffer;
    char* fin = buffer + buffer_size - 1; // Deja 1 byte para el '\0'

    if (!append_str(&p, fin, "AT+CIPSEND=0,")) return -1;
    if (!append_int(&p, fin, lenPayload)) return -1;
    if (!append_str(&p, fin, "\r\n")) return -1;

    *p = '\0'; // Terminador nulo
    return p - buffer; // Devuelve el total de bytes escritos
}
