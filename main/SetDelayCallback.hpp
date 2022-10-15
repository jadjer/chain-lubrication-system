// Copyright 2022 Pavel Suprunov
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
// Created by jadjer on 28.09.22.
//

#pragma once

#include <BLECharacteristic.h>
#include <Preferences.h>

class SetDelayCallback : public BLECharacteristicCallbacks {
 public:
  SetDelayCallback();
  ~SetDelayCallback() override;

 public:
  int getValue();

 public:
  void onRead(BLECharacteristic* pCharacteristic) override;
  void onWrite(BLECharacteristic* pCharacteristic) override;

 private:
  uint32_t m_defaultValue;
  Preferences m_preferences;
  std::string m_preferenceKey;

 private:
  uint32_t getValueFromPreference();
};
