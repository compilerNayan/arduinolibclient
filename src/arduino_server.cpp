#ifdef ARDUINO

#include <Arduino.h>

#include "ISpringBootApp.h"



void setup() {
    Serial.begin(115200);
    /// @Autowired
    ISpringBootAppPtr springBootApp;

    springBootApp->StartApp();
}

void loop() {
    /// @Autowired
    ISpringBootAppPtr springBootApp;

    springBootApp->ListenToRequest();
}

#endif // ARDUINO

