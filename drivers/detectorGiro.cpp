#include <DetectorGiro.h>


DetectorGiro *p1 = nullptr;

DetectorGiro::DetectorGiro(uint8_t port1, uint8_t pin1,uint8_t port2, uint8_t pin2)
:  sensor1(port1, pin1, PinInt::flanco_desc,handler_sensor1),
   sensor2(port2, pin2, PinInt::flanco_desc, handler_sensor2)
{
  p1 = this;
  system_millis = 0;

  // Inicialización de la nueva lógica
  isr_flag_s1 = false;
  isr_flag_s2 = false;
  tiempo_debounce_S1 = 0;
  tiempo_debounce_S2 = 0;
  estado_encoder = 0; // Estado inicial: Reposo

  // Inicialización de variables de cálculo
  periodo_ms = 0;
  tiempo_ultimo_pulso_ms = 0;
  sentido = DETENIDO;
}

DetectorGiro::~DetectorGiro() {

}

static inline void disable_irq(void)
{
    uint32_t primask = 1;
    __asm volatile ("msr primask, %0" : : "r" (primask) : "memory");
}

static inline void enable_irq(void)
{
    uint32_t primask = 0;
    __asm volatile ("msr primask, %0" : : "r" (primask) : "memory");
}

uint8_t DetectorGiro::getSentidoGiro()
{
	uint8_t sentido_local;

    // Lectura atómica: deshabilita IRQs brevemente
    // para asegurar que leemos un valor completo.
    disable_irq();
    sentido_local = this->sentido;
    enable_irq();

	return sentido_local;
}

uint32_t DetectorGiro::getRPM()
{
    uint32_t periodo_local;
    uint32_t tiempo_ultimo_pulso_local;
    uint32_t system_millis_local;

    //Copia Atómica
    disable_irq();
    periodo_local = this->periodo_ms;
    tiempo_ultimo_pulso_local = this->tiempo_ultimo_pulso_ms;
    system_millis_local = system_millis;
    enable_irq();

    // Lógica de Timeout
    uint32_t tiempo_desde_ultimo_pulso = system_millis_local - tiempo_ultimo_pulso_local;

    if (tiempo_desde_ultimo_pulso > TIMEOUT_MOTOR_DETENIDO_MS)
    {
        // Limpiamos el historial de períodos
        for (uint8_t i = 0; i < PROMEDIO; i++)
            periodos_hist[i] = 0;
        idx_periodo = 0;

        this->estado_encoder = 0;
        this->sentido = DETENIDO;
        return 0; // Motor detenido
    }

    //Filtro de Promedio
    periodos_hist[idx_periodo++] = periodo_local;
    idx_periodo %= PROMEDIO;

    uint32_t suma = 0;
    for (uint8_t i = 0; i < PROMEDIO; i++)
        suma += periodos_hist[i];

    uint32_t promedio_periodo = suma / PROMEDIO;

    if (promedio_periodo == 0)
        return 0;

    //Cálculo Final de RPM
    return (60000 / promedio_periodo) / 2;
}

void DetectorGiro::handler(void)
{
 system_millis++;
}

void DetectorGiro::procesar()
{
    uint32_t ahora = system_millis;
    bool s1_valido = false;
    bool s2_valido = false;

    if (isr_flag_s1)
    {
        isr_flag_s1 = false;
        if ((ahora - tiempo_debounce_S1) > ANTIREBOTE_MS)
        {
            s1_valido = true;
            tiempo_debounce_S1 = ahora;
        }
    }
    if (isr_flag_s2)
    {
        isr_flag_s2 = false;
        if ((ahora - tiempo_debounce_S2) > ANTIREBOTE_MS)
        {
            s2_valido = true;
            tiempo_debounce_S2 = ahora;
        }
    }


    // Ruido: ambos pulsos a la vez. Resetear y salir.
    if (s1_valido && s2_valido)
    {
        estado_encoder = 0; // Estado de reposo/error
        return;
    }

    // Logica de S1
    if (s1_valido)
    {
        if (estado_encoder == 2) // CASO 1: Secuencia S2 -> S1
        {
            sentido = HORARIO;
            calcularPeriodo(ahora);
            estado_encoder = 0; // Secuencia completada, reset
        }
        else if (estado_encoder == 0) // CASO 2: S1 (Inicio de secuencia)
        {
            estado_encoder = 1;
        }
        else if (estado_encoder == 1) // CASO 3: S1 -> S1 (Error/Reversión)
        {
            estado_encoder = 0;
        }
    }
    // Logica de S2
    else if (s2_valido)
    {
        if (estado_encoder == 1) // CASO 1: Secuencia S1 -> S2
        {
            sentido = ANTIHORARIO;
            calcularPeriodo(ahora);
            estado_encoder = 0; // Secuencia completada, reset
        }
        else if (estado_encoder == 0) // CASO 2: S2 (Inicio de secuencia)
        {
            estado_encoder = 2;
        }
        else if (estado_encoder == 2) // CASO 3: S2 -> S2 (Error/Reversión)
        {
            estado_encoder = 0;
        }
    }
}


  //Helper interno para actualizar el período.
void DetectorGiro::calcularPeriodo(uint32_t ahora)
{
    uint32_t delta = ahora - tiempo_ultimo_pulso_ms;

    if (delta < TIMEOUT_MOTOR_DETENIDO_MS)
    {
        // Protegemos la escritura de periodo_ms
        disable_irq();
        this->periodo_ms = delta;
        enable_irq();
    }

    // tiempo_ultimo_pulso_ms solo se escribe aca,
    // pero getRPM lo lee asi que tambien lo protegemos.
    disable_irq();
    this->tiempo_ultimo_pulso_ms = ahora;
    enable_irq();
}


void handler_sensor1(void)
{
    if (p1 == nullptr) return;

    p1->isr_flag_s1 = true;
}

void handler_sensor2(void)
{
    if (p1 == nullptr) return;

    p1->isr_flag_s2 = true;
}
