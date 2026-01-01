#ifdef ARDUINO

#include <Arduino.h>
#include "controller/UserRepositoryTests.h"


void setup() {
    Serial.begin(115200);
    delay(1000);
    // Call RunAllTests with no command-line arguments (as if nothing was passed)
    char* empty_argv[] = { (char*)"arduino_main" };
    RunAllTests(1, empty_argv);
}

void loop() {
}

#endif // ARDUINO

