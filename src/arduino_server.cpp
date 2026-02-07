#ifdef ARDUINO

#include <Arduino.h>

#include "IArduinoSpringBootApp.h"
//#include "tests/AllTests.h"
#include "service/ISwitchService.h"

void setup() {
    Serial.begin(115200);

    // Run all test suites (ThreadPoolTests, ThreadPoolMathExampleTests, etc.)
    //RunAllTestSuites(0, nullptr);

    /* @Autowired */
    IArduinoSpringBootAppPtr springBootApp;

    springBootApp->StartApp();
}

void loop() {
    /* @Autowired */
    IArduinoSpringBootAppPtr springBootApp;

    springBootApp->ListenToRequest();

    /* @Autowired */
    ISwitchServicePtr switchService;
    switchService->RefreshAllSwitches();
}

#endif // ARDUINO

