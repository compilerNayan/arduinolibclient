#ifndef SWITCHDTO_H
#define SWITCHDTO_H

#include <StandardDefines.h>
#include "../SwitchState.h"

/* @Serializable */
class SwitchDto {
    Public Int id;
    Public SwitchState switchState;

    /**
     * @brief Default constructor
     */
    Public SwitchDto() : id(0), switchState(SwitchState::Off) {}

    /**
     * @brief Parameterized constructor
     * @param id The switch ID
     * @param switchState The switch state
     */
    Public SwitchDto(Int id, SwitchState switchState) : id(id), switchState(switchState) {}
};

#endif // SWITCHDTO_H

