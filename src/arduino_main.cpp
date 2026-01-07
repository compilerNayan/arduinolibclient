#ifdef ARDUINO

#include <Arduino.h>
#include <WiFi.h>

#include "controller/UserRepositoryTests.h"
#include "IHttpRequestManager.h"    
#include "controller/06-MyEntityRepository.h"

/* @Autowired */
IHttpRequestManagerPtr requestManager;

// std_println macro for Arduino
#define std_println2(x) Serial.println(x)
#define std_print2(x) Serial.print(x)

// WiFi credentials
const char* ssid = "Garfield";
const char* password = "123Madhu$$";

/// @Autowired
MyEntityRepositoryPtr myEntityRepository;


void setup() {
    Serial.begin(115200);
    delay(1000);
    // Call RunAllTests with no command-line arguments (as if nothing was passed)
    char* empty_argv[] = { (char*)"arduino_main" };
    RunAllTests(1, empty_argv);
    // Connect to WiFi
    std_println2("Connecting to WiFi");
    WiFi.begin(ssid, password);

    // Wait for connection, print "." while connecting
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        std_print(".");
    }

    // Print connected message when connected
    std_println2("\nConnected!");
    std_println2("IP Address: ");
    std_println2(WiFi.localIP());

    Val myEntity = MyEntity();
    myEntity.id = 1;
    myEntity.username = "test";
    myEntity.password = "test";
    myEntityRepository->Save(myEntity);

    Val myEntity2 = myEntityRepository->FindById(1);
    std_println2("myEntity2: ");
    std_println2(myEntity2->Serialize().c_str());

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

