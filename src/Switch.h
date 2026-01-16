#ifndef SWITCH_H
#define SWITCH_H

#include <StandardDefines.h>
#include "ISwitch.h"
#include "SwitchState.h"
#include "ILogger.h"
#include "Tag.h"

/* @Component */
class Switch : public ISwitch {
    Private SwitchState virtualState;
    Private SwitchState physicalState;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public Switch() : virtualState(SwitchState::Off), physicalState(SwitchState::Off) {}

    Public Virtual ~Switch() = default;

    Public Virtual Void TurnOn() override {
        // To turn on: both states must be the same
        // Set both to On to achieve actual state = On
        virtualState = SwitchState::On;
        physicalState = SwitchState::On;
        
        if (logger != nullptr) {
            StdString message = "Turned on switch (virtual: ON, physical: ON)";
            StdString functionName = "TurnOn";
            logger->Info(Tag::Untagged, message, functionName);
        }
    }

    Public Virtual Void TurnOff() override {
        // To turn off: states must be different
        // Toggle one of them to make them different
        // We'll toggle the physical state
        if (virtualState == SwitchState::On) {
            physicalState = SwitchState::Off;
        } else {
            physicalState = SwitchState::On;
        }
        
        if (logger != nullptr) {
            StdString message = "Turned off switch (virtual: " + 
                               (virtualState == SwitchState::On ? "ON" : "OFF") + 
                               ", physical: " + 
                               (physicalState == SwitchState::On ? "ON" : "OFF") + ")";
            StdString functionName = "TurnOff";
            logger->Info(Tag::Untagged, message, functionName);
        }
    }

    Public Virtual Void Toggle() override {
        // Get current actual state
        SwitchState currentState = GetState();
        
        if (currentState == SwitchState::On) {
            // Currently on, turn it off
            TurnOff();
        } else {
            // Currently off, turn it on
            TurnOn();
        }
        
        if (logger != nullptr) {
            StdString message = "Toggled switch to " + 
                               (GetState() == SwitchState::On ? "ON" : "OFF");
            StdString functionName = "Toggle";
            logger->Info(Tag::Untagged, message, functionName);
        }
    }

    Public Virtual SwitchState GetState() override {
        // Actual state is ON if virtual and physical states match
        // Actual state is OFF if virtual and physical states differ
        SwitchState actualState = (virtualState == physicalState) ? SwitchState::On : SwitchState::Off;
        
        if (logger != nullptr) {
            StdString message = "Get switch state: " + 
                               (actualState == SwitchState::On ? "ON" : "OFF") + 
                               " (virtual: " + 
                               (virtualState == SwitchState::On ? "ON" : "OFF") + 
                               ", physical: " + 
                               (physicalState == SwitchState::On ? "ON" : "OFF") + ")";
            StdString functionName = "GetState";
            logger->Info(Tag::Untagged, message, functionName);
        }
        
        return actualState;
    }

    public: static ISwitchPtr GetInstance() {
        static ISwitchPtr instance(new Switch());
        return instance;
    }
};

template <>
struct Implementation<ISwitch> {
    using type = Switch;
};

template <>
struct Implementation<ISwitch*> {
    using type = Switch*;
};

#endif // SWITCH_H

