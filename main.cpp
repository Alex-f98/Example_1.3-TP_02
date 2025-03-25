#include "mbed.h"
#include "arm_book_lib.h"

int main()
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