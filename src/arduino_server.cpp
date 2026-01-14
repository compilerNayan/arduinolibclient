#ifdef ARDUINO

#include <Arduino.h>

#include "ISpringBootApp.h"

/// @Autowired
ISpringBootAppPtr springBootApp;


void setup() {
    springBootApp->StartApp();
}

void loop() {
    springBootApp->ListenToRequest();
}

#endif // ARDUINO

