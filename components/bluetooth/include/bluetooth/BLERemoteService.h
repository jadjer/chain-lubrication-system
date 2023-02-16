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
// Created by jadjer on 16.02.23.
//

#pragma once

#include "sdkconfig.h"

#if defined(CONFIG_BLUEDROID_ENABLED)

#include "bluetooth/BLEClient.h"
#include "bluetooth/BLERemoteCharacteristic.h"
#include "bluetooth/BLEUUID.h"
#include "bluetooth/RTOS.h"

#include <map>

class BLEClient;
class BLERemoteCharacteristic;

/**
 * @brief A model of a remote %BLE service.
 */
class BLERemoteService {
   public:
    virtual ~BLERemoteService();

    // Public methods
    BLERemoteCharacteristic* getCharacteristic(const char* uuid);              // Get the specified characteristic reference.
    BLERemoteCharacteristic* getCharacteristic(BLEUUID uuid);                  // Get the specified characteristic reference.
    BLERemoteCharacteristic* getCharacteristic(uint16_t uuid);                 // Get the specified characteristic reference.
    std::map<std::string, BLERemoteCharacteristic*>* getCharacteristics();
    std::map<uint16_t, BLERemoteCharacteristic*>* getCharacteristicsByHandle();// Get the characteristics map.
    void getCharacteristics(std::map<uint16_t, BLERemoteCharacteristic*>** pCharacteristicMap);

    BLEClient* getClient(void);                                  // Get a reference to the client associated with this service.
    uint16_t getHandle();                                        // Get the handle of this service.
    BLEUUID getUUID(void);                                       // Get the UUID of this service.
    std::string getValue(BLEUUID characteristicUuid);            // Get the value of a characteristic.
    void setValue(BLEUUID characteristicUuid, std::string value);// Set the value of a characteristic.
    std::string toString(void);

   private:
    // Private constructor ... never meant to be created by a user application.
    BLERemoteService(esp_gatt_id_t srvcId, BLEClient* pClient, uint16_t startHandle, uint16_t endHandle);

    // Friends
    friend class BLEClient;
    friend class BLERemoteCharacteristic;

    // Private methods
    void retrieveCharacteristics(void);// Retrieve the characteristics from the BLE Server.
    esp_gatt_id_t* getSrvcId(void);
    uint16_t getStartHandle();         // Get the start handle for this service.
    uint16_t getEndHandle();           // Get the end handle for this service.

    void gattClientEventHandler(
        esp_gattc_cb_event_t event,
        esp_gatt_if_t gattc_if,
        esp_ble_gattc_cb_param_t* evtParam);

    void removeCharacteristics();

    // Properties

    // We maintain a map of characteristics owned by this service keyed by a string representation of the UUID.
    std::map<std::string, BLERemoteCharacteristic*> m_characteristicMap;

    // We maintain a map of characteristics owned by this service keyed by a handle.
    std::map<uint16_t, BLERemoteCharacteristic*> m_characteristicMapByHandle;

    bool m_haveCharacteristics;// Have we previously obtained the characteristics.
    BLEClient* m_pClient;
    FreeRTOS::Semaphore m_semaphoreGetCharEvt = FreeRTOS::Semaphore("GetCharEvt");
    esp_gatt_id_t m_srvcId;
    BLEUUID m_uuid;        // The UUID of this service.
    uint16_t m_startHandle;// The starting handle of this service.
    uint16_t m_endHandle;  // The ending handle of this service.
};                         // BLERemoteService

#endif                     /* CONFIG_BLUEDROID_ENABLED */