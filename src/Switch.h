#ifndef SWITCH_H
#define SWITCH_H

#include <StandardDefines.h>
#include "ISwitch.h"
#include "SwitchState.h"
#include "IPhysicalSwitchReader.h"
#include "IRelayController.h"
#include "ILogger.h"
#include "Tag.h"

/* @Component */
class Switch : public ISwitch {
    Private Int id;
    Private Int pin;
    Private SwitchState virtualState;
    Private SwitchState relayState;

    /* @Autowired */
    Private IPhysicalSwitchReaderPtr physicalSwitchReader;

    /* @Autowired */
    Private IRelayControllerPtr relayController;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public Switch() : id(22), pin(22), virtualState(SwitchState::Off), relayState(SwitchState::Off) {}

    Public Virtual ~Switch() = default;

    Public Virtual Void TurnOn() override {
        // Turn relay ON
        relayController->SetState(pin, SwitchState::On);
        
        // Save relay state
        relayState = SwitchState::On;
        
        // Read current physical state
        SwitchState physicalState = ReadPhysicalState();
        
        // To achieve final ON: virtual and physical must match
        // If physical is ON, set virtual to ON (1+1=1)
        // If physical is OFF, set virtual to OFF (0+0=1)
        virtualState = physicalState;
        
        StdString message = "Turned on switch (virtual: " + 
                           (virtualState == SwitchState::On ? "ON" : "OFF") + 
                           ", physical: " + 
                           (physicalState == SwitchState::On ? "ON" : "OFF") + 
                           ", actual: " + 
                           (GetState() == SwitchState::On ? "ON" : "OFF") + ")";
        StdString functionName = "TurnOn";
        logger->Info(Tag::Untagged, message, functionName);
    }

    Public Virtual Void TurnOff() override {
        // Turn relay OFF
        relayController->SetState(pin, SwitchState::Off);
        
        // Save relay state
        relayState = SwitchState::Off;
        
        // Read current physical state
        SwitchState physicalState = ReadPhysicalState();
        
        // To achieve final OFF: virtual and physical must differ
        // If physical is ON, set virtual to OFF (1+0=0)
        // If physical is OFF, set virtual to ON (0+1=0)
        virtualState = (physicalState == SwitchState::On) ? SwitchState::Off : SwitchState::On;
        
        StdString message = "Turned off switch (virtual: " + 
                           (virtualState == SwitchState::On ? "ON" : "OFF") + 
                           ", physical: " + 
                           (physicalState == SwitchState::On ? "ON" : "OFF") + 
                           ", actual: " + 
                           (GetState() == SwitchState::On ? "ON" : "OFF") + ")";
        StdString functionName = "TurnOff";
        logger->Info(Tag::Untagged, message, functionName);
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
        
        StdString message = "Toggled switch to " + 
                           (GetState() == SwitchState::On ? "ON" : "OFF");
        StdString functionName = "Toggle";
        logger->Info(Tag::Untagged, message, functionName);
    }

    Public Virtual SwitchState GetState() override {
        // Read physical state from pin
        SwitchState physicalState = ReadPhysicalState();
        
        // Actual state is ON if virtual and physical states match
        // Actual state is OFF if virtual and physical states differ
        SwitchState actualState = (virtualState == physicalState) ? SwitchState::On : SwitchState::Off;
        
        StdString message = "Get switch state: " + 
                           (actualState == SwitchState::On ? "ON" : "OFF") + 
                           " (virtual: " + 
                           (virtualState == SwitchState::On ? "ON" : "OFF") + 
                           ", physical: " + 
                           (physicalState == SwitchState::On ? "ON" : "OFF") + 
                           ", pin: " + std::to_string(pin) + ")";
        StdString functionName = "GetState";
        logger->Info(Tag::Untagged, message, functionName);
        
        return actualState;
    }

    Public Virtual Void Refresh() override {
        // Get current actual state
        SwitchState currentState = GetState();
        
        // If actual state doesn't match relay state, update relay state
        if (currentState != relayState) {
            relayState = currentState;
            
            
            StdString message = "Refreshed relay state to " + 
                               (relayState == SwitchState::On ? "ON" : "OFF") + 
                               " (was: " + 
                               (currentState == SwitchState::On ? "ON" : "OFF") + ")";
            StdString functionName = "Refresh";
            logger->Info(Tag::Untagged, message, functionName);
        }
    }

    Private SwitchState ReadPhysicalState() {
        return physicalSwitchReader->ReadPhysicalState(pin);
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

