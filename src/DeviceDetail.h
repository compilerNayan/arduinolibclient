#ifndef DEVICEDETAIL_H
#define DEVICEDETAIL_H

#include <StandardDefines.h>

class DeviceDetail {
    Public Int id;
    Public Int pin;

    /**
     * @brief Default constructor
     */
    Public DeviceDetail() : id(0), pin(0) {}

    /**
     * @brief Parameterized constructor
     * @param id The device ID
     * @param pin The device pin number
     */
    Public DeviceDetail(Int id, Int pin) : id(id), pin(pin) {}
};

#endif // DEVICEDETAIL_H

