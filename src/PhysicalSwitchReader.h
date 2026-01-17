#ifdef ARDUINO
#ifndef PHYSICALSWITCHREADER_H
#define PHYSICALSWITCHREADER_H

#include <StandardDefines.h>
#include "IPhysicalSwitchReader.h"
#include "SwitchState.h"
#include "ILogger.h"
#include "Tag.h"
#include <Arduino.h>

/* @Component */
class PhysicalSwitchReader : public IPhysicalSwitchReader {
    /* @Autowired */
    Private ILoggerPtr logger;

    Public PhysicalSwitchReader() = default;

    Public Virtual ~PhysicalSwitchReader() = default;

    Public Virtual SwitchState ReadPhysicalState(Int pin) override {
        // Set pin mode to INPUT
        pinMode(pin, INPUT);
        
        // Read the physical state directly from the GPIO pin
        Bool isHigh = digitalRead(pin) == HIGH;
        SwitchState state = isHigh ? SwitchState::On : SwitchState::Off;
        
        if (logger != nullptr) {
            StdString message = "Read physical state from pin " + std::to_string(pin) + ": " + 
                               (state == SwitchState::On ? "ON" : "OFF");
            logger->Info(Tag::Untagged, message);
        }
        
        return state;
    }
};

#endif // PHYSICALSWITCHREADER_H
#endif // ARDUINO

