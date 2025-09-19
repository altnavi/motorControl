/*
 * DR_Init.h
 *
 *  Created on: 8 ago. 2021
 *      Author: chor
 */

#ifndef DR_INIT_H_
#define DR_INIT_H_

//Registros de control del FRO y del CLK

//Direccion de la funcion de ROM para setear la frecuencia del oscilador:
#define FRO_SET_FRECUENCY_FUNC (void (*)(int))(0x0F0026F5U)

void Inicializar (void);

#endif /* DR_INIT_H_ */
