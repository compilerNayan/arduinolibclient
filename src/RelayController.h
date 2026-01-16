#ifdef ARDUINO
#ifndef RELAYCONTROLLER_H
#define RELAYCONTROLLER_H

#include <StandardDefines.h>
#include "IRelayController.h"
#include "SwitchState.h"
#include <Arduino.h>
#include "ILogger.h"
#include "Tag.h"

/* @Component */
class RelayController : public IRelayController {
    Public Virtual ~RelayController() = default;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public Virtual Void TurnOn(Int pin) override {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        if (logger != nullptr) {
            StdString message = "Turned on relay at pin " + std::to_string(pin);
            StdString functionName = "TurnOn";
            logger->Info(Tag::Untagged, message, functionName);
        }
    }

    Public Virtual Void TurnOff(Int pin) override {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        if (logger != nullptr) {
            StdString message = "Turned off relay at pin " + std::to_string(pin);
            StdString functionName = "TurnOff";
            logger->Info(Tag::Untagged, message, functionName);
        }
    }

    Public Virtual SwitchState GetState(Int pin) override {
        pinMode(pin, INPUT);
        Bool isHigh = digitalRead(pin) == HIGH;
        SwitchState state = isHigh ? SwitchState::On : SwitchState::Off;
        if (logger != nullptr) {
            StdString message = "Get state of pin " + std::to_string(pin) + ": " + (isHigh ? "ON" : "OFF");
            StdString functionName = "GetState";
            logger->Info(Tag::Untagged, message, functionName);
        }
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

