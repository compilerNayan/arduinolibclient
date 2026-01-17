#ifndef SWITCHCONTROLLER_H
#define SWITCHCONTROLLER_H

#include <StandardDefines.h>
#include "ISwitchController.h"
#include "SwitchDto.h"
#include "ResponseEntity.h"
#include "HttpStatus.h"
#include "../service/ISwitchService.h"

/* @RestController */
/* @RequestMapping("/switch") */
class SwitchController final : public ISwitchController {
    /* @Autowired */
    Private ISwitchServicePtr switchService;

    Public SwitchController() = default;

    Public Virtual ~SwitchController() = default;

    /* @PutMapping("/{id}/on") */
    Public Virtual ResponseEntity<SwitchDto> TurnOnSwitch(/* @PathVariable("id") */ Int id) override {
        optional<SwitchState> finalState = switchService->TurnOnSwitch(id);
        if (!finalState.has_value()) {
            return ResponseEntity<SwitchDto>::NotFound(SwitchDto());
        }

        SwitchDto dto(id, finalState.value());
        return ResponseEntity<SwitchDto>::Ok(dto);
    }

    /* @PutMapping("/{id}/off") */
    Public Virtual ResponseEntity<SwitchDto> TurnOffSwitch(/* @PathVariable("id") */ Int id) override {
        optional<SwitchState> finalState = switchService->TurnOffSwitch(id);
        if (!finalState.has_value()) {
            return ResponseEntity<SwitchDto>::NotFound(SwitchDto());
        }

        SwitchDto dto(id, finalState.value());
        return ResponseEntity<SwitchDto>::Ok(dto);
    }

    /* @PutMapping("/{id}/toggle") */
    Public Virtual ResponseEntity<SwitchDto> ToggleSwitch(/* @PathVariable("id") */ Int id) override {
        optional<SwitchState> finalState = switchService->ToggleSwitch(id);
        if (!finalState.has_value()) {
            return ResponseEntity<SwitchDto>::NotFound(SwitchDto());
        }

        SwitchDto dto(id, finalState.value());
        return ResponseEntity<SwitchDto>::Ok(dto);
    }

    /* @GetMapping("/{id}") */
    Public Virtual ResponseEntity<SwitchDto> GetSwitchStateById(/* @PathVariable("id") */ Int id) override {
        optional<SwitchState> state = switchService->GetSwitchStateById(id);
        if (!state.has_value()) {
            return ResponseEntity<SwitchDto>::NotFound(SwitchDto());
        }

        SwitchDto dto(id, state.value());
        return ResponseEntity<SwitchDto>::Ok(dto);
    }

    /* @GetMapping */
    Public Virtual ResponseEntity<Vector<SwitchDto>> GetAllSwitchState() override {
        Vector<SwitchDto> switchDtos = switchService->GetAllSwitchState();
        return ResponseEntity<Vector<SwitchDto>>::Ok(switchDtos);
    }
};

#endif // SWITCHCONTROLLER_H

