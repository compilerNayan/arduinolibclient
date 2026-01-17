#ifndef SWITCHDEVICE_H
#define SWITCHDEVICE_H

#include <StandardDefines.h>
#include "ISwitchDevice.h"
#include "SwitchState.h"
#include "IPhysicalSwitchReader.h"
#include "IRelayController.h"
#include "ILogger.h"
#include "Tag.h"
#include "controller/SwitchRepository.h"
#include "controller/Switch.h"

class SwitchDevice : public ISwitchDevice {
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

    /* @Autowired */
    Private SwitchRepositoryPtr switchRepository;

    Public SwitchDevice() : id(22), pin(22), virtualState(SwitchState::Off), relayState(SwitchState::Off) {
        // Initialize virtualState from repository
        optional<Switch> switchEntity = switchRepository->FindById(id);
        if (switchEntity.has_value() && switchEntity.value().GetVirtualState().has_value()) {
            SwitchState savedVirtualState = switchEntity.value().GetVirtualState().value();
            virtualState = savedVirtualState;
        }
    
        // Initialize relayState from relay controller
        relayState = relayController->GetState(pin);       

        RefreshRelayState();        
    }

    Public Virtual ~SwitchDevice() = default;

    Public Virtual Void TurnOn() override {
        // Read current physical state
        SwitchState physicalState = ReadPhysicalState();
        
        // To achieve final ON: virtual and physical must match
        // If physical is ON, set virtual to ON (1+1=1)
        // If physical is OFF, set virtual to OFF (0+0=1)
        virtualState = physicalState;
        
        // Save virtual state to repository
        switchRepository->Update(GetSwitchEntity());
        
        logger->Info(Tag::Untagged, GetOperationLogMessage("on"));
        
        // Refresh relay state based on virtual and physical states
        RefreshRelayState();
    }

    Public Virtual Void TurnOff() override {
        // Read current physical state
        SwitchState physicalState = ReadPhysicalState();
        
        // To achieve final OFF: virtual and physical must differ
        // If physical is ON, set virtual to OFF (1+0=0)
        // If physical is OFF, set virtual to ON (0+1=0)
        virtualState = (physicalState == SwitchState::On) ? SwitchState::Off : SwitchState::On;
        
        // Save virtual state to repository
        switchRepository->Update(GetSwitchEntity());
        
        logger->Info(Tag::Untagged, GetOperationLogMessage("off"));
        
        // Refresh relay state based on virtual and physical states
        RefreshRelayState();
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
        
        logger->Info(Tag::Untagged, GetOperationLogMessage("toggle"));
    }

    Public Virtual SwitchState GetState() override {
        // Read physical state from pin
        SwitchState physicalState = ReadPhysicalState();
        
        // Actual state is ON if virtual and physical states match
        // Actual state is OFF if virtual and physical states differ
        SwitchState actualState = (virtualState == physicalState) ? SwitchState::On : SwitchState::Off;
        
        logger->Info(Tag::Untagged, GetStateLogMessage(actualState, virtualState, physicalState, pin));
        
        return actualState;
    }

    Public Virtual Void Refresh() override {
        // Get current actual state
        SwitchState currentState = GetState();
        
        // If actual state doesn't match relay state, update relay state
        if (currentState != relayState) {
            relayState = currentState;
            
            
            logger->Info(Tag::Untagged, GetRefreshLogMessage(relayState, currentState));
        }
    }

    Private SwitchState ReadPhysicalState() {
        return physicalSwitchReader->ReadPhysicalState(pin);
    }

    /**
     * @brief Convert SwitchState to string representation
     * @param state The switch state to convert
     * @return "ON" if state is On, "OFF" if state is Off
     */
    Private Static inline StdString StateToString(SwitchState state) {
        return (state == SwitchState::On) ? "ON" : "OFF";
    }

    /**
     * @brief Generate log message for switch operations (on, off, toggle)
     * @param operation The operation type: "on", "off", or "toggle"
     * @return Formatted log message string
     */
    Private inline StdString GetOperationLogMessage(CStdString operation) {
        // Read current physical state
        SwitchState physicalState = ReadPhysicalState();
        
        // Calculate actual state from virtual and physical states
        SwitchState actualState = (virtualState == physicalState) ? SwitchState::On : SwitchState::Off;
        
        if (operation == "toggle") {
            return "Toggled switch to " + StateToString(actualState);
        } else {
            StdString operationText = (operation == "on") ? "Turned on" : "Turned off";
            return operationText + " switch (virtual: " + 
                   StateToString(virtualState) + 
                   ", physical: " + 
                   StateToString(physicalState) + 
                   ", actual: " + 
                   StateToString(actualState) + ")";
        }
    }

    /**
     * @brief Generate log message for GetState operation
     * @param actualState The actual state
     * @param virtualState The virtual state
     * @param physicalState The physical state
     * @param pin The pin number
     * @return Formatted log message string
     */
    Private Static inline StdString GetStateLogMessage(SwitchState actualState, SwitchState virtualState, SwitchState physicalState, Int pin) {
        return "Get switch state: " + 
               StateToString(actualState) + 
               " (virtual: " + 
               StateToString(virtualState) + 
               ", physical: " + 
               StateToString(physicalState) + 
               ", pin: " + std::to_string(pin) + ")";
    }

    /**
     * @brief Generate log message for Refresh operation
     * @param relayState The new relay state
     * @param currentState The previous state
     * @return Formatted log message string
     */
    Private Static inline StdString GetRefreshLogMessage(SwitchState relayState, SwitchState currentState) {
        return "Refreshed relay state to " + 
               StateToString(relayState) + 
               " (was: " + 
               StateToString(currentState) + ")";
    }

    /**
     * @brief Refresh the relay state based on virtual and physical states
     * Detects the actual state (virtual == physical ? On : Off) and sets relay accordingly
     */
    Private Void RefreshRelayState() {
        // Read current physical state
        SwitchState physicalState = ReadPhysicalState();
        
        // Actual state is ON if virtual and physical states match
        // Actual state is OFF if virtual and physical states differ
        SwitchState actualState = (virtualState == physicalState) ? SwitchState::On : SwitchState::Off;
        
        // Set relay to match the actual state
        relayController->SetState(pin, actualState);
        relayState = actualState;
    }

    /**
     * @brief Create a Switch entity with current id and virtualState
     * @return Switch entity with current id and virtualState
     */
    Private Switch GetSwitchEntity() {
        Switch switchEntity;
        switchEntity.SetId(optional<int>(id));
        switchEntity.SetVirtualState(optional<SwitchState>(virtualState));
        return switchEntity;
    }

};

#endif // SWITCHDEVICE_H

