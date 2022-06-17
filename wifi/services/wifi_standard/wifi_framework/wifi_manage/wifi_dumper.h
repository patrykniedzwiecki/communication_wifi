/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef OHOS_WIFI_DUMPER_H
#define OHOS_WIFI_DUMPER_H

#include <vector>
#include <string>

namespace OHOS {
namespace Wifi {
class WifiDumper {
public:
    bool DeviceDump(std::function<void(std::string&)> saBasicDumpFunc,
        const std::vector<std::string> &vecArgs, std::string &result);

    bool ScanDump(std::function<void(std::string&)> saBasicDumpFunc,
        const std::vector<std::string> &vecArgs, std::string &result);

    bool P2pDump(std::function<void(std::string&)> saBasicDumpFunc,
        const std::vector<std::string> &vecArgs, std::string &result);

    bool HotspotDump(std::function<void(std::string&)> saBasicDumpFunc,
        const std::vector<std::string> &vecArgs, std::string &result);

private:
    std::string ShowDeviceDumpUsage() const;
    std::string ShowScanDumpUsage() const;
    std::string ShowP2pDumpUsage() const;
    std::string ShowHotspotDumpUsage() const;
    void PrintArgs(const std::vector<std::string>& vecArgs);
};
}  // namespace Wifi
}  // namespace OHOS
#endif