// Copyright 2023 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by jadjer on 01.10.23.
//

#pragma once

#include <functional>

#include "executor/Node.hpp"
#include "gpio/PinLevel.hpp"
#include "gpio/interface/IInputPin.hpp"

enum ExternalPowerState
{
    EXTERNAL_POWER_ON = 0,
    EXTERNAL_POWER_OFF = 1,
    EXTERNAL_POWER_COUNT = 2
};

using PinState = gpio::PinLevel;
using ExternalPowerCallbackFunction = std::function<void(ExternalPowerState)>;

class ExternalPower : public executor::Node
{
public:
    ExternalPower();
    ~ExternalPower() override = default;

public:
    void setCallback(ExternalPowerCallbackFunction const& callback);

private:
    void process() override;

private:
    ExternalPowerCallbackFunction m_callback = nullptr;

private:
    PinState m_externalPowerLastLevel;
    IInputPinPtr<PinState> m_externalPowerPin;
};
