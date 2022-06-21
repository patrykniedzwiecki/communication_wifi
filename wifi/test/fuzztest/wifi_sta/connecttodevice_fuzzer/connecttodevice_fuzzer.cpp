/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "connecttodevice_fuzzer.h"
#include "wifi_device.h"

namespace OHOS {
namespace Wifi {
    std::unique_ptr<WifiDevice> devicePtr = WifiDevice::GetInstance(WIFI_DEVICE_ABILITY_ID);
    bool ConnectToDeviceFuzzerTest(const uint8_t* data, size_t size)
    {
        if (devicePtr == nullptr) {
            return false;
        }

        WifiDeviceConfig config;
        config.ssid = std::string(reinterpret_cast<const char*>(data), size);
        config.bssid = std::string(reinterpret_cast<const char*>(data), size);
        config.preSharedKey = std::string(reinterpret_cast<const char*>(data), size);
        config.keyMgmt = std::string(reinterpret_cast<const char*>(data), size);
        devicePtr->ConnectToDevice(config);
        return true;
    }
}  // namespace Wifi
}  // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    OHOS::Wifi::ConnectToDeviceFuzzerTest(data, size);
    return 0;
}

