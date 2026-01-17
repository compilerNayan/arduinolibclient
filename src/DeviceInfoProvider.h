#ifndef DEVICEINFOPROVIDER_H
#define DEVICEINFOPROVIDER_H

#include <StandardDefines.h>
#include "IDeviceInfoProvider.h"
#include "DeviceDetail.h"

/* @Component */
class DeviceInfoProvider : public IDeviceInfoProvider {
    Public Virtual ~DeviceInfoProvider() = default;

    Public Virtual Vector<DeviceDetail> GetAllSwitchDetails() override {
        Vector<DeviceDetail> devices;
        
        // Return 3 devices with id 1,2,3 and pin 101,102,103
        devices.push_back(DeviceDetail(1, 101));
        devices.push_back(DeviceDetail(2, 102));
        devices.push_back(DeviceDetail(3, 103));
        
        return devices;
    }
};

#endif // DEVICEINFOPROVIDER_H

