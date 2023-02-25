/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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
#include <gtest/gtest.h>
#include "i_wifi_chip.h"
#include "i_wifi_hotspot_iface.h"
#include "i_wifi_iface.h"

using namespace testing::ext;

namespace OHOS {
namespace Wifi {
constexpr int LENTH = 5;
constexpr int LENMAC = 17;

class IWifiIfaceTest : public testing::Test {
public:
    static void SetUpTestCase()
    {}
    static void TearDownTestCase()
    {}
    virtual void SetUp()
    {}
    virtual void TearDown()
    {}

    static void onStaJoinOrLeaveTest(const CStationInfo *info, int id)
    {
    }
};

HWTEST_F(IWifiIfaceTest, GetNameTest, TestSize.Level1)
{
    char ifname[LENTH] = "test";
    int32_t size = LENTH;
    GetName(ifname, size);
}

HWTEST_F(IWifiIfaceTest, GetTypeTest, TestSize.Level1)
{
    int32_t type = 1;
    GetType(&type);
}

HWTEST_F(IWifiIfaceTest, StartSoftApTest, TestSize.Level1)
{
    int id = 0;
    StartSoftAp(id);
}

HWTEST_F(IWifiIfaceTest, StopSoftApTest, TestSize.Level1)
{
    int id = 0;
    StopSoftAp(id);
}

HWTEST_F(IWifiIfaceTest, SetHostapdConfigTest, TestSize.Level1)
{
    HostapdConfig config;
    int id = 0;
    SetHostapdConfig(&config, id);
}

HWTEST_F(IWifiIfaceTest, GetStaInfosTest, TestSize.Level1)
{
    char infos[] = "GetStaInfosTest";
    int32_t size = 1;
    int id = 0;
    GetStaInfos(infos, &size, id);
}

HWTEST_F(IWifiIfaceTest, SetMacFilterTest, TestSize.Level1)
{
    unsigned char mac[] = "00:00:00:00:00:00";
    int lenMac = LENMAC;
    int id = 0;
    SetMacFilter(mac, lenMac, id);
}

HWTEST_F(IWifiIfaceTest, DelMacFilterTest, TestSize.Level1)
{
    unsigned char mac[] = "00:00:00:00:00:00";
    int lenMac = LENMAC;
    int id = 0;
    DelMacFilter(mac, lenMac, id);
}

HWTEST_F(IWifiIfaceTest, DisassociateStaTest, TestSize.Level1)
{
    unsigned char mac[] = "00:00:00:00:00:00";
    int lenMac = LENMAC;
    int id = 0;
    DisassociateSta(mac, lenMac, id);
}

HWTEST_F(IWifiIfaceTest, GetValidFrequenciesForBandTest, TestSize.Level1)
{
    int32_t band = 1;
    int frequencies = 1;
    int32_t size = 1;
    int id = 0;
    GetValidFrequenciesForBand(band, &frequencies, &size, id);
}

HWTEST_F(IWifiIfaceTest, SetCountryCodeTest, TestSize.Level1)
{
    const char code;
    int id = 0;
    SetCountryCode(&code, id);
}

HWTEST_F(IWifiIfaceTest, RegisterAsscociatedEventTest, TestSize.Level1)
{
    IWifiApEventCallback callback;
    int id = 0;
    RegisterAsscociatedEvent(callback, id)
    callback.onStaJoinOrLeave = onStaJoinOrLeaveTest;
    RegisterAsscociatedEvent(callback, id);
}

HWTEST_F(IWifiIfaceTest, WpaSetPowerModelTest, TestSize.Level1)
{
    const int model = 1;
    int id = 0;
    WpaSetPowerModel(model, id);
}

HWTEST_F(IWifiIfaceTest, WpaGetPowerModelTest, TestSize.Level1)
{
    int model = 1;
    int id = 0;
    WpaGetPowerModel(&model, id);
}

HWTEST_F(IWifiIfaceTest, IsChipSupportCsaTest, TestSize.Level1)
{
    bool isSupport = false;
    IsChipSupportCsa(&isSupport);
}

HWTEST_F(IWifiIfaceTest, IsChipSupportRadarDetectTest, TestSize.Level1)
{
    bool isSupport = false;
    IsChipSupportRadarDetect(&isSupport);
}

HWTEST_F(IWifiIfaceTest, IsChipSupportDfsChannelTest, TestSize.Level1)
{
    bool isSupport = false;
    IsChipSupportDfsChannel(&isSupport);
}

HWTEST_F(IWifiIfaceTest, IsChipSupportIndoorChannelTest, TestSize.Level1)
{
    bool isSupport = false;
    IsChipSupportIndoorChannel(&isSupport);
}
}  // namespace Wifi
}  // namespace OHOS