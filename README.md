# motorControl
Este es nuestro trabajo práctico final obligatorio de la materia Informática 2.\
Consiste en un sistema de control de motores, y se pretende realizar acciones según las mediciones realizadas a un
durante su tiempo de funcionamiento. Es decir, que según los parámetros medidos y
cómo se encuentre funcionando el motor, vamos a realizar diversas acciones que por lo
general se llevan a cabo habitualmente en las instalaciones que operan con estos equipos,
sólo que vamos a implementar hardware de dimensiones más cómodas para trabajar.

## Compilación
Utilizamos también los siguientes archivos `cr_cpp_config.cpp` `cr_startup_lpc84x.cpp` provistos por NXP en MCUXpresso.\
No utilizamos RomDivide, Micro Trace Buffer, Code Read Protection.

## Objetivos
### Principales
- [ ] Encender una alarma de emergencia y una luz intermitente por exceder cierto número de RPM.

- [ ] Registrar en una base de datos, las veces que el motor realizó movimientos poco convencionales, ya sea paradas bruscas o arranques muy rápidos. Según el número de casos registrados, vamos a detener el motor mediante un pulsador de parada de emergencia.
- [ ] Llevar a cabo un conteo del tiempo de uso del motor, con el fin de detenerlo y realizarle mantenimiento y/o cambios en su estructura. Este tiempo también lo vamos a registrar en un archivo, y dependiendo de los valores leídos del mismo, vamos a parar o no el funcionamiento.

- [ ] Mostrar mediante una interfaz gráfica los estados actuales del motor, así como también los valores de velocidad a los que el mismo gira y los sentidos para los cuales lo hace
### TODO
- [ ] Clase de comunicación
    - [ ] Protocolo UART
    - [ ] Clase para ESP8266
- [ ] Clase de motor
    - [ ] Relés
    - [ ] Sensor de RPM
- [ ] Clase de sensor de temperatura
