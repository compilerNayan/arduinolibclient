#ifdef ARDUINO

#include <Arduino.h>
#include <WiFi.h>

#include "controller/UserRepositoryTests.h"
#include "IHttpRequestManager.h"    

/// @Autowired
IHttpRequestManagerPtr requestManager;

// println macro for Arduino
#define println(x) Serial.println(x)

// WiFi credentials
const char* ssid = "Garfield";
const char* password = "123Madhu$$";


void setup() {
    Serial.begin(115200);
    delay(1000);
    // Call RunAllTests with no command-line arguments (as if nothing was passed)
    char* empty_argv[] = { (char*)"arduino_main" };
    RunAllTests(1, empty_argv);
    // Connect to WiFi
    println("Connecting to WiFi");
    WiFi.begin(ssid, password);

    // Wait for connection, print "." while connecting
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        print(".");
    }

    // Print connected message when connected
    println("\nConnected!");
    println("IP Address: ");
    println(WiFi.localIP());
    requestManager = Implementation<IHttpRequestManager>::type::GetInstance();
    //s2->Start(8080);
    delay(7000);

}

void loop() {
}

#endif // ARDUINO

