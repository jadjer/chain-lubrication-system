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

#include "executor/Executor.hpp"

#include "Pump.hpp"
#include "Timer.hpp"
#include "ExternalPower.hpp"

using namespace std::chrono_literals;

extern "C" void app_main()
{
    auto pumpPtr = std::make_shared<Pump>();

    auto timerDelayPtr = std::make_shared<Timer>();
    timerDelayPtr->setCallback([&]() { pumpPtr->enable(60s); });

    auto externalPowerPtr = std::make_shared<ExternalPower>();
    externalPowerPtr->setCallback(
        [&](ExternalPowerState externalPowerState)
        {
            if (externalPowerState == EXTERNAL_POWER_ON)
            {
                std::cout << "External power ON" << std::endl;

                pumpPtr->enable(60s);
                timerDelayPtr->start(10min);
            }

            if (externalPowerState == EXTERNAL_POWER_OFF)
            {
                std::cout << "External power OFF" << std::endl;
                timerDelayPtr->stop();
            }
        });

    auto executorPtr = std::make_unique<executor::Executor>();
    executorPtr->addNode(pumpPtr);
    executorPtr->addNode(timerDelayPtr);
    executorPtr->addNode(externalPowerPtr);
    executorPtr->spin();
}