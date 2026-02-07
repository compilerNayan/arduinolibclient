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
            logger->Info(Tag::Untagged, "Nayan: Setting relay on pin " + std::to_string(pin) + " to HIGH");
            pinMode(pin, OUTPUT);
            digitalWrite(pin, HIGH);
        } else {
            logger->Info(Tag::Untagged, "Nayan: Setting relay on pin " + std::to_string(pin) + " to LOW");
            pinMode(pin, OUTPUT);
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
};

#endif // RELAYCONTROLLER_H
#endif // ARDUINO

