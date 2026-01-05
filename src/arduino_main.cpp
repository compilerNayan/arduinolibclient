#ifdef ARDUINO

#include <Arduino.h>
#include <WiFi.h>

#include "controller/UserRepositoryTests.h"
#include "IHttpRequestManager.h"    

/* @Autowired */
IHttpRequestManagerPtr requestManager;

// std_println macro for Arduino
#define std_println(x) Serial.println(x)
#define std_print(x) Serial.print(x)

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
    std_println("Connecting to WiFi");
    WiFi.begin(ssid, password);

    // Wait for connection, print "." while connecting
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        std_print(".");
    }

    // Print connected message when connected
    std_println("\nConnected!");
    std_println("IP Address: ");
    std_println(WiFi.localIP());
    requestManager = Implementation<IHttpRequestManager>::type::GetInstance();
    //s2->Start(8080);
    delay(7000);

}

void loop() {
    requestManager->RetrieveRequest();
    requestManager->ProcessRequest();
    requestManager->ProcessResponse();
}

#endif // ARDUINO

