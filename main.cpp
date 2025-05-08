#include "mbed.h"
#include "arm_book_lib.h"

#define TIME_DELAY_MS  10

//Acxtivo solo usar CPP
#define ONLY_C  //ONLY_CPP

#ifdef ONLY_CPP

int mainCpp()
{
    DigitalIn gasDetector(D2);
    DigitalIn overTempDetector(D3);
    DigitalIn alarmOffButton(BUTTON1);

    DigitalOut alarmLed(LED1);

    gasDetector.mode(PullDown);
    overTempDetector.mode(PullDown);

    alarmLed = OFF;

    bool alarmState = OFF;

    while (true) {
        // Si se detecta gas o sobre temepratura -> alarma state ON
        // alarmLed <- alarmState
        // Si se preciona el boton de apagado alamState OFF.

        if ( gasDetector || overTempDetector ) {
            alarmState = ON;
        }
        
        //Muestro el estado de la alarma con alarmLed.
        alarmLed = alarmState;

        if ( alarmOffButton ) {
            alarmState = OFF;
        }
    }
}
#else

//################# DigitalIn - C #############################################

/**
 * @brief Estructura para encapsular un pin de entrada digital.
 */
typedef struct {
    gpio_t gpio;
} DigitalIn_t;

//static==privado, inline== remplaza la funcion por su contenido.
/**
 * @brief Inicializa un pin como entrada digital.
 * @param obj Puntero al objeto DigitalIn_t.
 * @param pin Pin físico a configurar como entrada.
 */
static inline void DigitalIn_init(DigitalIn_t *obj, PinName pin) {
    gpio_init_in(&obj->gpio, pin);
}

/**
 * @brief Lee el pin digital.
 * @param obj Puntero al objeto DigitalIn_t.
 * @return Valor lógico leido 
 */
static inline int DigitalIn_read(DigitalIn_t *obj) {
    return gpio_read(&obj->gpio);
}

/**
 * @brief Configura el modo del pin de entrada (ej. PullUp, PullDown)..
 * @param obj Puntero al objeto DigitalIn_t.
 * @param mode Mode de entrada para el pin.
 */
static inline void DigitalIn_mode(DigitalIn_t *obj, PinMode mode) {
    gpio_mode(&obj->gpio, mode);
}


//################# DigitalOut - C #############################################
/**
 * @brief Estructura para encapsular un pin de salida digital.
 */
typedef struct {
    gpio_t gpio;
} DigitalOut_t;

/**
 * @brief Inicializa la salida digital.
 * @param obj Puntero al objeto DigitalIn_t.
 * @param pin Pin físico a configurar como entrada.
 */
//static==privado, inline== remplaza la funcion por su contenido.
static inline void DigitaOut_init(DigitalOut_t *obj, PinName pin) {
    gpio_init_out(&obj->gpio, pin);
}

/**
 * @brief Inicializa la salida digital.
 * @param obj Puntero al objeto DigitalIn_t.
 * @param pin Pin físico a configurar como entrada.
 */
static inline void DigitalOut_write(DigitalOut_t * obj, int value){
    // Thread safe / atomic HAL call
    gpio_write(&obj->gpio, value);
}

/**
 * @brief Lee la salida digital.
 * @param obj Puntero al objeto DigitalIn_t.
 * @return Valor actual del pin.
 */
static inline int DigitalOut_read(DigitalOut_t *obj) {
    return gpio_read(&obj->gpio);
}


//##################################################
//9b) Ventajas: Al ser directamente C, se peude secir que es mas rapido, consume menos recursos que con c++
// en c++ se tiene que crear la clase, y una vtable con referencias a todos los metodos, en general la estructura se puede decir que ocupa mas memoria.
// en cambio con C uno se ahorra todo eso y gana un poco mas en eficiencia y velocidad (si por ejemplo estoy trabajndo con intterrupciones es mejor todo C).
// Desventajas: la sintaxis es mas complicada, codigo mas largo, las inicializaciones tienen que ser mas especificas y ademas si quiero laburar con memoria dinamica
// c++ tiene funcionalidad mejores para automatizar la liberacion y pedido de memoria.
// Quiza la mayor ventaja de C++ seria que se puede usar clases y eso permite encapsular varios metodo dentro y variables dentro de la misma, eso es lo que se pierde usando solo C.

int main() {
    // Entradas
    DigitalIn_t gasDetector;
    DigitalIn_t overTempDetector;
    DigitalIn_t alarmOffButton;

    DigitalIn_init(&gasDetector     , D2);
    DigitalIn_init(&overTempDetector, D3);
    DigitalIn_init(&alarmOffButton  , BUTTON1);

    DigitalIn_mode(&overTempDetector, PullDown);
    DigitalIn_mode(&gasDetector, PullDown);


    DigitalOut_t alarmLed;
    DigitaOut_init(&alarmLed, LED1);

    int alarmState = OFF;
    int gas = 0, temp=0, btn=0; 
    int timeDebug = 0;
    while (true) {
        // Leer entradas
        int gas  = DigitalIn_read(&gasDetector);
        int temp = DigitalIn_read(&overTempDetector);
        int btn  = DigitalIn_read(&alarmOffButton);

        // Lógica de activación
        if (gas || temp)
            alarmState = ON;

        DigitalOut_write(&alarmLed, alarmState);

        if (btn) 
            alarmState = OFF;

        timeDebug += TIME_DELAY_MS;
        if (timeDebug > 500){
            //9.c)
            printf("Gas: %d, Temp: %d, Boton: %d, LED: %d\n",
               gas, temp, btn, DigitalOut_read(&alarmLed));
        }

        delay(TIME_DELAY_MS);
    }

}

#endif