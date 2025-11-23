#ifndef STRING_FORMATTER_H
#define STRING_FORMATTER_H

#include <stddef.h>
#include <LPC845.h>

/**
 * Convierte un entero a texto y lo guarda en el buffer.
 * Útil para imprimir números en el LCD sin usar sprintf.
 */
void convertir_entero_a_texto(int32_t num, char* buffer);

/**
 * Genera el JSON de estado del sistema.
 */
int formato_json_economico(
    char* buffer,
    size_t buffer_size,
    int rpm,
    float temperatura_motor,
    int sentido_giro,
    bool flag_alarmatimer
);

/**
 * Genera el comando AT+CIPSEND.
 */
int formato_cipsend_economico(
    char* buffer,
    size_t buffer_size,
    int lenPayload
);

#endif
