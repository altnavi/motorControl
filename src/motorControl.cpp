/*
╭──╦──╮
│IC║XC│
╠══╬══╣
│NI║KA│
╰──╩──╯
*/

#include <cr_section_macros.h>
#include <Init.h>
#include <LPC845.h>
#include <driverMotor.h>
#include <detectorGiro.h>
#include <MdEWifi.h>
#include <MdEMotor.h>
#include <Timer.h>
#include <Temperatura.h>
#include <StringFormatter.h>
#include <i2cCom.h>
#include <LcdI2C.h>


void actualizarGUI();
void actDisplay();

//DEFINES GPIOS
#define gpio_cambioSentido 			0,18
#define gpio_alarma 				0,13
#define gpio_motor 					0,19
#define gpio_sensor1 				0,16
#define gpio_sensor2 				0,17
#define	gpio_bot_conectar			0,4
#define gpio_ledWifiOk 				1,0
#define gpio_ledWifiConectado		1,1

#define canal_analog_temp			6

//DEFINES COM SERIE

#define port_pc						25,24
#define uart_pc						0
#define vel_pc						115200

#define port_esp					8,9
#define uart_esp					1
#define vel_esp						115200

//DEFINES TIEMPOS TIMERS (MS)

#define tiempo_led_wifi_inter 		300
#define tiempo_handler_envio_datos	1000
#define tiempo_RPMtimer				4000 //tiempo antes de pasar a falla intermitente
#define tiempo_alarmatimer			3000
#define tiempo_actualizar_datos		800 //actualiza variables mde motor
#define tiempo_buzzer_inter			100 //tiempo toggle buzzer cuando hay falla

//DEFINES I2C

#define port_i2c					0
#define pin_sda						11
#define pin_scl						10
#define dir_i2c						0x27


//DEFINES BUFFERS
#define TAMANO_BUFFER_ENVIO 128

//  GPIOS
gpio led_wifi_OK(gpio_ledWifiOk,gpio::SALIDA, gpio::LOW);
gpio led_wifi_conectado(gpio_ledWifiConectado,gpio::SALIDA, gpio::LOW);
gpio cambioSentido(gpio_cambioSentido,gpio::SALIDA, gpio::HIGH);
gpio alarma(gpio_alarma,gpio::SALIDA, gpio::HIGH);
gpio motor(gpio_motor,gpio::SALIDA, gpio::HIGH);

// BOTONES
DigitalInput bot(gpio_bot_conectar);

// COMUNICACION SERIE
serialCom esp(uart_esp, vel_esp, port_esp);
serialCom pc(uart_pc, vel_pc, port_pc);

// TIMERS
Timer led_wifi_inter(tiempo_led_wifi_inter, wifi_intermitente_handler);
Timer handler_envio_datos(tiempo_handler_envio_datos, actualizarGUI);
Timer RPMtimer(tiempo_RPMtimer,handler_RPMtimer);
Timer alarmatimer(tiempo_alarmatimer,handler_alarmatimer);
Timer actualizarDatos(tiempo_actualizar_datos,handler_actualizarDatos);
Timer buzzer_inter(tiempo_buzzer_inter, buzzer_intermitente);

// VARIABLES MdE
char buffer_envio[TAMANO_BUFFER_ENVIO];
float temperatura_motor = 0;

uint32_t rpm;
uint32_t velocidad;
uint8_t sentido_giro = 0;
uint8_t sentido_detectado = DetectorGiro::DETENIDO;

bool flag_RPMtimer = false;
bool flag_alarmatimer = false;
bool flag_actualizarDatos = false;
bool flag_boton_inicio = false;
bool flag_boton_parada = false;
bool flag_velocidad = false;
bool flag_sentido = false;

// MOTOR
driverMotor motor1(motor,cambioSentido);
DetectorGiro d1(gpio_sensor1, gpio_sensor2);
Temperatura t1(canal_analog_temp);

//DISPLAY
I2C_COM i2c(port_i2c,pin_sda, pin_scl, dir_i2c);
LcdI2C lcd(&i2c);


int main(void) {
	Inicializar();
	InicializarMdE();
    Wifi_Init();

    handler_envio_datos.start();

    lcd.init(16, 2);

    lcd.setCursor(0, 0);
    lcd.print("Estado:");

        // Opcional: Escribir algo en la linea 2 mientras esperamos datos
    lcd.setCursor(0, 1);
    lcd.print("Esperando..."); //espero hasta wifi conectado


    while(1) {
    	MdEMotor();
        Wifi_Manejar();
        d1.procesar();

    }

    return 0;
}

void actDisplay()
{
	lcd.setCursor(8,0);
	if(flag_alarmatimer)
	{
		lcd.print("     ");
		lcd.setCursor(8,0);
		lcd.print("FALLA");
	}
	else
	{
		lcd.print("     ");
		lcd.setCursor(8,0);
		lcd.print("OK");
	}

    lcd.setCursor(0, 1);
    lcd.print("RPM: ");
    lcd.print(rpm);
    lcd.print("    "); // limpiar restos


    lcd.setCursor(9,1);
    lcd.print("T: ");
    lcd.print(temperatura_motor);
}

void actualizarGUI()
{
	static uint8_t contador = 0;

	if (Wifi_EstaOcupado()) {
	    handler_envio_datos.start();
	    return;
	}

	int bytes_escritos = formato_json_economico(
	    buffer_envio,
	    TAMANO_BUFFER_ENVIO,
	    rpm,
	    temperatura_motor,
	    sentido_giro,
	    flag_alarmatimer
	);

	if (bytes_escritos > 0)
    {
        if(!Wifi_EnviarDato(buffer_envio))
        {
        		//no hago nada por el momento si esta ocupado
        }
    }

	if(contador>=2)
	{
		actDisplay();
		contador=0;
	}

	contador++;

	handler_envio_datos.start();
}
