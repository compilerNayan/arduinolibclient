#ifndef SWITCHDTO_H
#define SWITCHDTO_H

#include <StandardDefines.h>
#include "../SwitchState.h"

/* @Serializable */
class SwitchDto {
    Public optional<Int> id;
    Public optional<StdString> switchState;

    /**
     * @brief Convert SwitchState enum to string representation
     * @param state The switch state to convert
     * @return "ON" if state is On, "OFF" if state is Off
     */
    Private Static inline StdString StateToString(SwitchState state) {
        return (state == SwitchState::On) ? "ON" : "OFF";
    }

    /**
     * @brief Default constructor
     */
    Public SwitchDto() : id(), switchState() {}

    /**
     * @brief Parameterized constructor
     * @param id The switch ID
     * @param switchState The switch state enum
     */
    Public SwitchDto(Int id, SwitchState switchState) : id(id), switchState(StateToString(switchState)) {}
};

#endif // SWITCHDTO_H

