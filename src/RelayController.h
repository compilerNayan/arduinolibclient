#ifdef ARDUINO
#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include <StandardDefines.h>
#include "IRelayController.h"
#include <Arduino.h>

// Define print macros for Arduino
#define std_print(x) Serial.print(x)
#define std_println(x) Serial.println(x)

/* @Component */
class RelayController : public IRelayController {
    Public Virtual ~RelayController() = default;

    Public Virtual Void SetHigh(Int pin) override {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        std_print("[RelayController] Set pin ");
        std_print(std::to_string(pin).c_str());
        std_println(" to HIGH");
    }

    Public Virtual Void SetLow(Int pin) override {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        std_print("[RelayController] Set pin ");
        std_print(std::to_string(pin).c_str());
        std_println(" to LOW");
    }

    Public Virtual Void Write(Int pin, Bool isHigh) override {
        if (isHigh) {
            SetHigh(pin);
        } else {
            SetLow(pin);
        }
    }

    Public Virtual Bool Read(Int pin) override {
        pinMode(pin, INPUT);
        Bool state = digitalRead(pin) == HIGH;
        std_print("[RelayController] Read pin ");
        std_print(std::to_string(pin).c_str());
        std_print(" state: ");
        std_println(state ? "HIGH" : "LOW");
        return state;
    }

    public: static IRelayControllerPtr GetInstance() {
        static IRelayControllerPtr instance(new RelayController());
        return instance;
    }
};

template <>
struct Implementation<IRelayController> {
    using type = RelayController;
};

template <>
struct Implementation<IRelayController*> {
    using type = RelayController*;
};

#endif // RELAYCONTROLLER_H
#endif // ARDUINO

