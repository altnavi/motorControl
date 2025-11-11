#ifndef STRING_FORMATTER_H
#define STRING_FORMATTER_H

// Incluimos las librerías estándar necesarias para los tipos de datos
#include <stddef.h> // Para size_t
#include <LPC845.h>
/*
 * Usamos extern "C" para asegurarnos de que el compilador de C++
 * no "decore" los nombres de estas funciones. Esto las hace
 * más predecibles y compatibles.
 */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reemplazo ultra-económico de snprintf para tu formato JSON específico.
 * Es segura contra desbordamiento de búfer.
 * No usa casi nada de memoria Flash (código).
 * No usa heap (RAM dinámica).
 *
 * @param buffer Búfer de salida donde se escribirá la cadena.
 * @param buffer_size Tamaño total del búfer de salida.
 * @param rpm Valor de RPM (entero).
 * @param temperatura_motor Valor de temperatura (flotante).
 * @param sentido_giro Valor de sentido (entero).
 * @param flag_alarmatimer Estado de la alarma (booleano).
 * @return El número de bytes escritos, o -1 si hubo un error (búfer muy pequeño).
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
 * Reemplazo ultra-económico de snprintf para el formato "AT+CIPSEND=0,%d\r\n"
 * Es segura contra desbordamiento de búfer.
 *
 * @param buffer Búfer de salida donde se escribirá la cadena.
 * @param buffer_size Tamaño total del búfer de salida.
 * @param lenPayload El número de bytes a enviar.
 * @return El número de bytes escritos, o -1 si hubo un error (búfer muy pequeño).
 */
int formato_cipsend_economico(
    char* buffer,
    size_t buffer_size,
    int lenPayload
);


#ifdef __cplusplus
}
#endif

#endif // STRING_FORMATTER_H
