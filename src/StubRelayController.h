#ifndef ARDUINO
#ifndef STUBRELAYCONTROLLER_H
#define STUBRELAYCONTROLLER_H

#include <StandardDefines.h>
#include "IRelayController.h"
#include <map>
#include <iostream>

/* @Component */
class StubRelayController : public IRelayController {
    Private:
        // Store pin states for testing purposes
        std::map<Int, Bool> pinStates;

    Public Virtual ~StubRelayController() = default;

    Public Virtual Void SetHigh(Int pin) override {
        pinStates[pin] = true;
        std::cout << "[StubRelayController] Set pin " << pin << " to HIGH" << std::endl;
    }

    Public Virtual Void SetLow(Int pin) override {
        pinStates[pin] = false;
        std::cout << "[StubRelayController] Set pin " << pin << " to LOW" << std::endl;
    }

    Public Virtual Void Write(Int pin, Bool isHigh) override {
        if (isHigh) {
            SetHigh(pin);
        } else {
            SetLow(pin);
        }
    }

    Public Virtual Bool Read(Int pin) override {
        // Return stored state, default to LOW if not set
        Bool state = (pinStates.find(pin) != pinStates.end()) ? pinStates[pin] : false;
        std::cout << "[StubRelayController] Read pin " << pin << " state: " << (state ? "HIGH" : "LOW") << std::endl;
        return state;
    }

    public: static IRelayControllerPtr GetInstance() {
        static IRelayControllerPtr instance(new StubRelayController());
        return instance;
    }
};

template <>
struct Implementation<IRelayController> {
    using type = StubRelayController;
};

template <>
struct Implementation<IRelayController*> {
    using type = StubRelayController*;
};

#endif // STUBRELAYCONTROLLER_H
#endif // ARDUINO

