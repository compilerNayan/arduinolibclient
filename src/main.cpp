#ifdef DEAD_CODE
#ifdef ARDUINO
#include <Arduino.h>
#include <WiFi.h>

// println macro for Arduino
#define println(x) Serial.println(x)
#include <IHttpRequestManager.h>

// WiFi credentials
const char* ssid = "Garfield";
const char* password = "123Madhu$$";

// /// @Autowired
IHttpRequestManagerPtr requestManager;
//auto s2 = ServerFactory::GetDefaultServer();


void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    delay(1000);

    // Connect to WiFi
    std_println("Connecting to WiFi");
    WiFi.begin(ssid, password);
    
    // Wait for connection, print "." while connecting
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
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
    /*auto message = s2->ReceiveMessage();
    if (message) {
        Serial.println(message->GetBody().c_str());
    } */
    requestManager->RetrieveRequest();
    requestManager->ProcessRequest();
}
#else 

#define _Entity
#define _Id_
#include <iostream>
#include <IFileManager.h>
#include "controller/UserRepository.h"

// /// @Autowired
UserRepositoryPtr userRepository = Implementation<UserRepository>::type::GetInstance();

// println macro for non-Arduino (uses cout)
#define println(x) std::cout << x << std::endl
//#include <IHttpRequestManager.h>

// /// @Autowired
//IHttpRequestManagerPtr requestManager = Implementation<IHttpRequestManager>::type::GetInstance();

int main() {
    std::cout << "Running DTO Corner Case Tests..." << std::endl;
    User user;
    user.id = 1;
    user.name = StdString("John Doe");
    userRepository->Save(user);
    /*while(true) {
        requestManager->RetrieveRequest();
        requestManager->ProcessRequest();
    } */
    return 0;
}

#endif
#endif