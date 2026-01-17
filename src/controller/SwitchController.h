#ifndef SWITCHCONTROLLER_H
#define SWITCHCONTROLLER_H

#include <StandardDefines.h>
#include "ISwitchController.h"
#include "SwitchDto.h"
#include "ResponseEntity.h"
#include "HttpStatus.h"
#include "../IDeviceCollection.h"
#include "../ISwitchDevice.h"

/* @RestController */
/* @RequestMapping("/switch") */
class SwitchController final : public ISwitchController {
    /* @Autowired */
    Private IDeviceCollectionPtr deviceCollection;

    Public SwitchController() = default;

    Public Virtual ~SwitchController() = default;

    /* @PutMapping("/{id}/on") */
    Public Virtual ResponseEntity<SwitchDto> TurnOnSwitch(/* @PathVariable("id") */ Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return ResponseEntity<SwitchDto>::NotFound(SwitchDto());
        }

        SwitchState finalState = device->TurnOn();
        SwitchDto dto(id, finalState);
        return ResponseEntity<SwitchDto>::Ok(dto);
    }

    /* @PutMapping("/{id}/off") */
    Public Virtual ResponseEntity<SwitchDto> TurnOffSwitch(/* @PathVariable("id") */ Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return ResponseEntity<SwitchDto>::NotFound(SwitchDto());
        }

        SwitchState finalState = device->TurnOff();
        SwitchDto dto(id, finalState);
        return ResponseEntity<SwitchDto>::Ok(dto);
    }

    /* @PutMapping("/{id}/toggle") */
    Public Virtual ResponseEntity<SwitchDto> ToggleSwitch(/* @PathVariable("id") */ Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return ResponseEntity<SwitchDto>::NotFound(SwitchDto());
        }

        SwitchState finalState = device->Toggle();
        SwitchDto dto(id, finalState);
        return ResponseEntity<SwitchDto>::Ok(dto);
    }

    /* @GetMapping("/{id}") */
    Public Virtual ResponseEntity<SwitchDto> GetSwitchStateById(/* @PathVariable("id") */ Int id) override {
        ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(id);
        if (device == nullptr) {
            return ResponseEntity<SwitchDto>::NotFound(SwitchDto());
        }

        SwitchDto dto(id, device->GetState());
        return ResponseEntity<SwitchDto>::Ok(dto);
    }

    /* @GetMapping */
    Public Virtual ResponseEntity<Vector<SwitchDto>> GetAllSwitchState() override {
        Vector<SwitchDto> switchDtos;
        
        // Get all devices from the collection
        // Since we don't have a GetAllDevices method, we'll need to iterate through known IDs
        // For now, we'll use the DeviceInfoProvider pattern or add a method to get all IDs
        // Let's assume we can get devices with IDs 1, 2, 3 (from DeviceInfoProvider)
        for (Int i = 1; i <= 3; i++) {
            ISwitchDevicePtr device = deviceCollection->GetSwitchDeviceById(i);
            if (device != nullptr) {
                SwitchDto dto(device->GetId(), device->GetState());
                switchDtos.push_back(dto);
            }
        }
        
        return ResponseEntity<Vector<SwitchDto>>::Ok(switchDtos);
    }
};

#endif // SWITCHCONTROLLER_H

