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

#include <memory>
#include <chrono>
#include <iostream>

#include "esp_sleep.h"

#include "executor/Executor.hpp"

#include "Pump.hpp"
#include "Timer.hpp"
#include "DistanceSensor.hpp"
#include "ExternalPower.hpp"

using namespace std::chrono_literals;

constexpr const auto pumpStartTime = 10s;
constexpr const auto pumpWorkTime = 60s;
constexpr const auto pumpEnableDelay = 30min;
constexpr const auto pumpEnableDistance_InKilometers = 100;

extern "C" void app_main()
{
    auto pumpPtr = std::make_shared<Pump>();
    pumpPtr->enable(pumpStartTime);

    auto timerDelayPtr = std::make_shared<Timer>();
    timerDelayPtr->setCallback(
        [&]
        {
            std::cout << "==================================================" << std::endl;
            std::cout << "Timer timeout" << std::endl;
            std::cout << "Pump enable for " << pumpWorkTime << std::endl;

            pumpPtr->enable(pumpWorkTime);
        });

    auto distanceSensorPtr = std::make_shared<DistanceSensor>();
    distanceSensorPtr->setCallback(
        [&](float const distance_InKilometers)
        {
            std::cout << "==================================================" << std::endl;
            std::cout << "Distance: " << distance_InKilometers << "km" << std::endl;

            if (distance_InKilometers < pumpEnableDistance_InKilometers)
            {
                return;
            }

            std::cout << "Distance large then " << pumpEnableDistance_InKilometers << "km" << std::endl;
            std::cout << "Pump enable for " << pumpWorkTime << std::endl;

            pumpPtr->enable(pumpWorkTime);
        });

    auto externalPowerPtr = std::make_shared<ExternalPower>();
    externalPowerPtr->setCallback(
        [&](ExternalPowerState const externalPowerState)
        {
            std::cout << "==================================================" << std::endl;

            if (externalPowerState == EXTERNAL_POWER_ON)
            {
                std::cout << "External Power ON" << std::endl;
                std::cout << "Pump enable for " << pumpWorkTime << std::endl;
                std::cout << "Timer start for " << pumpEnableDelay << " (with repeat)" << std::endl;

                pumpPtr->enable(pumpStartTime);
                timerDelayPtr->start(pumpEnableDelay, true);
            }

            if (externalPowerState == EXTERNAL_POWER_OFF)
            {
                std::cout << "External Power OFF" << std::endl;
                std::cout << "Pump disabled" << std::endl;
                std::cout << "Timer stopped" << std::endl;
                std::cout << "Sleep" << std::endl;

                pumpPtr->disable();
                timerDelayPtr->stop();

//                esp_deep_sleep_start();
            }
        });

    auto executorPtr = std::make_unique<executor::Executor>();
    executorPtr->addNode(pumpPtr);
    executorPtr->addNode(timerDelayPtr);
    executorPtr->addNode(distanceSensorPtr);
    executorPtr->addNode(externalPowerPtr);
    executorPtr->spin();
}
