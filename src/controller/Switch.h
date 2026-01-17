#ifndef SWITCH_H
#define SWITCH_H

#include <StandardDefines.h>
#include "../SwitchState.h"

/* @Entity */
class Switch {

    /* @Id */
    /* @NotNull */
    Public optional<int> id;

    Public optional<StdString> virtualState;

    /**
     * @brief Convert SwitchState enum to string representation
     * @param state The switch state to convert
     * @return "ON" if state is On, "OFF" if state is Off
     */
    Private Static inline StdString StateToString(SwitchState state) {
        return (state == SwitchState::On) ? "ON" : "OFF";
    }

    /**
     * @brief Convert string representation to SwitchState enum
     * @param stateStr The string state ("ON" or "OFF")
     * @return SwitchState::On if "ON", SwitchState::Off if "OFF" or empty
     */
    Private Static inline SwitchState StringToState(CStdString stateStr) {
        if (stateStr == "ON") {
            return SwitchState::On;
        }
        return SwitchState::Off;
    }

    /**
     * @brief Default constructor
     */
    Public Switch() : id(), virtualState() {}

    /**
     * @brief Parameterized constructor
     * @param id The switch ID
     * @param virtualState The virtual state of the switch
     */
    Public Switch(optional<int> id, optional<SwitchState> virtualState) 
        : id(id), virtualState(virtualState.has_value() ? optional<StdString>(StateToString(virtualState.value())) : optional<StdString>()) {}

    /**
     * @brief Get the switch ID
     * @return The switch ID
     */
    Public optional<int> GetId() const {
        return id;
    }

    /**
     * @brief Set the switch ID
     * @param id The switch ID to set
     */
    Public Void SetId(optional<int> id) {
        this->id = id;
    }

    /**
     * @brief Get the virtual state
     * @return The virtual state of the switch as enum
     */
    Public optional<SwitchState> GetVirtualState() const {
        if (!virtualState.has_value()) {
            return optional<SwitchState>();
        }
        return optional<SwitchState>(StringToState(virtualState.value()));
    }

    /**
     * @brief Set the virtual state
     * @param virtualState The virtual state to set as enum
     */
    Public Void SetVirtualState(optional<SwitchState> virtualState) {
        if (virtualState.has_value()) {
            this->virtualState = optional<StdString>(StateToString(virtualState.value()));
        } else {
            this->virtualState = optional<StdString>();
        }
    }
};

#endif

