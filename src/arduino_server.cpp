#ifdef ARDUINO

#include <Arduino.h>
#include <WiFi.h>

#include "tests/AllTests.h"
#include "IHttpRequestManager.h"    
#include "INetworkManager.h"

/* @Autowired */
IHttpRequestManagerPtr requestManager;

// WiFi credentials
const char* ssid = "Garfield";
const char* password = "123Madhu$$";


void setup() {
    Serial.begin(115200);
    delay(1000);

  /*  // Run all test suites
    char* empty_argv[] = { (char*)"arduino_main" };

    RunAllTestSuites(1, empty_argv);

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
    std_println(WiFi.localIP()); */

    /// @Autowired
    INetworkManagerPtr networkManager;
    networkManager->ConnectNetwork();
    delay(7000);

    requestManager = Implementation<IHttpRequestManager>::type::GetInstance();
    delay(7000);
    std_println("Listening now...");

}

void loop() {
    requestManager->RetrieveRequest();
    requestManager->ProcessRequest();
    requestManager->ProcessResponse();
}

#endif // ARDUINO

