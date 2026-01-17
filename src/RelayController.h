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

    Public Virtual Void SetState(Int pin, SwitchState state) override {
        pinMode(pin, OUTPUT);
        if (state == SwitchState::On) {
            digitalWrite(pin, HIGH);
        } else {
            digitalWrite(pin, LOW);
        }
        
        StdString message = "Set relay at pin " + std::to_string(pin) + " to " + 
                           (state == SwitchState::On ? "ON" : "OFF");
        StdString functionName = "SetState";
        logger->Info(Tag::Untagged, message, functionName);
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

