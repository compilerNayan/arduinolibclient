#ifndef SWITCHSERVICE_H
#define SWITCHSERVICE_H

#include <StandardDefines.h>
#include "ISwitchService.h"
#include "../IDeviceCollection.h"
#include "../ISwitchDevice.h"
#include "../controller/SwitchDto.h"
#include "../SwitchState.h"

/* @Service */
class SwitchService : public ISwitchService {
    /* @Autowired */
    Private IDeviceCollectionPtr deviceCollection;

    Public SwitchService() = default;

    Public Virtual ~SwitchService() = default;

    Public Virtual optional<SwitchState> TurnOnSwitch(Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return optional<SwitchState>();
        }

        SwitchState finalState = device->TurnOn();
        return optional<SwitchState>(finalState);
    }

    Public Virtual optional<SwitchState> TurnOffSwitch(Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return optional<SwitchState>();
        }

        SwitchState finalState = device->TurnOff();
        return optional<SwitchState>(finalState);
    }

    Public Virtual optional<SwitchState> ToggleSwitch(Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return optional<SwitchState>();
        }

        SwitchState finalState = device->Toggle();
        return optional<SwitchState>(finalState);
    }

    Public Virtual optional<SwitchState> GetSwitchStateById(Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return optional<SwitchState>();
        }

        SwitchState state = device->GetState();
        return optional<SwitchState>(state);
    }

    Public Virtual Vector<SwitchDto> GetAllSwitchState() override {
        Vector<SwitchDto> switchDtos;
        
        // Get all devices from the collection
        // Iterate through known IDs (1, 2, 3 from DeviceInfoProvider)
        for (Int i = 1; i <= 3; i++) {
            ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(i);
            if (device != nullptr) {
                SwitchDto dto(device->GetId(), device->GetState());
                switchDtos.push_back(dto);
            }
        }
        
        return switchDtos;
    }
};

#endif // SWITCHSERVICE_H

