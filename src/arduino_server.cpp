#ifdef ARDUINO

#include <Arduino.h>

#include "IArduinoSpringBootApp.h"
#include "tests/TestUtils.h"
#include "thread_tests/ThreadPoolTests.h"
#include "thread_tests/ThreadPoolMathExampleTests.h"

void setup() {
    Serial.begin(115200);

    // Run only thread pool tests on Arduino (other suites need desktop/filesystem)
    std_println("");
    std_println("========================================");
    std_println("  Arduino: Thread Pool Tests Only");
    std_println("========================================");
    RunAllThreadPoolTests();
    RunAllThreadPoolMathExampleTests();
    std_println("========================================");
    std_println("  Thread pool tests done.");
    std_println("========================================");

    /* @Autowired */
   // IArduinoSpringBootAppPtr springBootApp;

    //springBootApp->StartApp();
}

void loop() {
    ///* @Autowired */
    //IArduinoSpringBootAppPtr springBootApp;

    //springBootApp->ListenToRequest();
}

#endif // ARDUINO

