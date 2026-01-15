#ifdef ARDUINO

#include <Arduino.h>

#include "IArduinoSpringBootApp.h"



void setup() {
    Serial.begin(115200);
    /* @Autowired */
    IArduinoSpringBootAppPtr springBootApp;

    springBootApp->StartApp();
}

void loop() {
    /* @Autowired */
    IArduinoSpringBootAppPtr springBootApp;

    springBootApp->ListenToRequest();
}

#endif // ARDUINO

