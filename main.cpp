#include "mbed.h"
#include "arm_book_lib.h"

#define TIME_DELAY_MS  10

//################# DigitalIn - C #############################################
typedef struct {
    gpio_t gpio;
} DigitalIn_t;

//static==privado, inline== remplaza la funcion por su contenido.
static inline void DigitalIn_init(DigitalIn_t *obj, PinName pin) {
    gpio_init_in(&obj->gpio, pin);
}

static inline int DigitalIn_read(DigitalIn_t *obj) {
    return gpio_read(&obj->gpio);
}

static inline void DigitalIn_mode(DigitalIn_t *obj, PinMode mode) {
    gpio_mode(&obj->gpio, mode);
}


//################# DigitalOut - C #############################################
typedef struct {
    gpio_t gpio;
} DigitalOut_t;

//static==privado, inline== remplaza la funcion por su contenido.
static inline void DigitaOut_init(DigitalOut_t *obj, PinName pin) {
    gpio_init_out(&obj->gpio, pin);
}

static inline void DigitalOut_write(DigitalOut_t * obj, int value){
    // Thread safe / atomic HAL call
    gpio_write(&obj->gpio, value);
}

static inline int DigitalOut_read(DigitalOut_t *obj) {
    return gpio_read(&obj->gpio);
}


//##################################################

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
            printf("Gas: %d, Temp: %d, Boton: %d, LED: %d\n",
               gas, temp, btn, DigitalOut_read(&alarmLed));
        }

        delay(TIME_DELAY_MS);
    }

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