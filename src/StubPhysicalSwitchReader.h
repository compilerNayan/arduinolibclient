#ifndef ARDUINO
#ifndef STUBPHYSICALSWITCHREADER_H
#define STUBPHYSICALSWITCHREADER_H

#include <StandardDefines.h>
#include "IPhysicalSwitchReader.h"
#include "IRelayController.h"
#include "SwitchState.h"
#include "ILogger.h"
#include "Tag.h"
#include <map>

/* @Component */
class StubPhysicalSwitchReader : public IPhysicalSwitchReader {
    /* @Autowired */
    Private IRelayControllerPtr relayController;

    /* @Autowired */
    Private ILoggerPtr logger;

    Public StubPhysicalSwitchReader() = default;

    Public Virtual ~StubPhysicalSwitchReader() = default;

    Public Virtual SwitchState ReadPhysicalState(Int pin) override {
        if (relayController == nullptr) {
            if (logger != nullptr) {
                StdString message = "RelayController is null, cannot read physical state from pin " + std::to_string(pin);
                StdString functionName = "ReadPhysicalState";
                logger->Error(Tag::Untagged, message, functionName);
            }
            return SwitchState::Off;
        }

        // Use RelayController to read the state from the pin
        SwitchState state = relayController->GetState(pin);
        
        if (logger != nullptr) {
            StdString message = "Read physical state from pin " + std::to_string(pin) + ": " + 
                               (state == SwitchState::On ? "ON" : "OFF");
            StdString functionName = "ReadPhysicalState";
            logger->Info(Tag::Untagged, message, functionName);
        }
        
        return state;
    }

    public: static IPhysicalSwitchReaderPtr GetInstance() {
        static IPhysicalSwitchReaderPtr instance(new StubPhysicalSwitchReader());
        return instance;
    }
};

template <>
struct Implementation<IPhysicalSwitchReader> {
    using type = StubPhysicalSwitchReader;
};

template <>
struct Implementation<IPhysicalSwitchReader*> {
    using type = StubPhysicalSwitchReader*;
};

#endif // STUBPHYSICALSWITCHREADER_H
#endif // ARDUINO

