#include "mbed.h"
#include "arm_book_lib.h"


typedef struct {
    gpio_t gpio;
} DigitalIn_t;

//static==privado, inline== remplaza la funcion por su contenido.
static inline void DigitalIn_init(DigitalIn_t *obj, PinName pin) {
    gpio_init_in(&obj->gpio, pin);
}

//static inline void DigitalIn_init_ex(DigitalIn_t *obj, PinName pin, PinMode mode) {
//    gpio_init_in_ex(&obj->gpio, pin, mode);
//}

static inline int DigitalIn_read(DigitalIn_t *obj) {
    return gpio_read(&obj->gpio);
}

static inline void DigitalIn_mode(DigitalIn_t *obj, PinMode mode) {
    gpio_mode(&obj->gpio, mode);
}

//static inline int DigitalIn_is_connected(DigitalIn_t *obj) {
//    return gpio_is_connected(&obj->gpio);
//}

int main() {
    // Entradas
    DigitalIn_t gasDetector;
    DigitalIn_t overTempDetector;
    DigitalIn_t alarmOffButton;

    DigitalIn_init(&gasDetector, D2);
    DigitalIn_mode(&gasDetector, PullDown);

    DigitalIn_init(&overTempDetector, D3);
    DigitalIn_mode(&overTempDetector, PullDown);

    DigitalIn_init(&alarmOffButton, BUTTON1);


    DigitalOut alarmLed(LED1);  //C++
    int alarmState = OFF;

    while (1) {
        if (DigitalIn_read(&gasDetector) || DigitalIn_read(&overTempDetector)) {
            alarmState = ON;
            printf("Gas detectado: %s ; alarmLed: %s\n", gasDetector ? "true" : "false", alarmLed ? "true" : "false");

        }
        alarmLed = alarmState;
        if (DigitalIn_read(&alarmOffButton)) {
            alarmState = OFF;
            printf("Gas detectado: %s ; alarmLed: %s\n", gasDetector ? "true" : "false", alarmLed ? "true" : "false");

        }
    }

    return 0;
}


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