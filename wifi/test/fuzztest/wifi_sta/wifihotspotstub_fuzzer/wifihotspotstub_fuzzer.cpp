/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "wifip2pstub_fuzzer.h"

#include <cstddef>
#include <cstdint>

#include "wifi_p2p_stub.h"
#include "message_parcel.h"
#include "securec.h"
#include "define.h"

namespace OHOS {
namespace Wifi {
constexpr size_t FOO_MAX_LEN = 1024;
constexpr size_t U32_AT_SIZE = 4;
constexpr size_t MAP_NUMS = 21;

const std::u16string FORMMGR_INTERFACE_TOKEN = u"ohos.wifi.IWifiP2pService";

class WifiHotSpotStubTest : public WifiP2pStub {
public:
    WifiP2pStubTest() = default;

    virtual ~WifiP2pStubTest() = default;

    ErrCode IsHotspotActive(bool &bActive) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode IsHotspotDualBandSupported(bool &isSupported) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode GetHotspotConfig(HotspotConfig &config) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode GetHotspotState(int &state) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode SetHotspotConfig(const HotspotConfig &config) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode SetHotspotIdleTimeout(int time) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode GetStationList(std::vector<StationInfo> &result) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode DisassociateSta(const StationInfo &info) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode EnableHotspot(const ServiceType type = ServiceType::DEFAULT) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode DisableHotspot(const ServiceType type = ServiceType::DEFAULT) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode GetBlockLists(std::vector<StationInfo> &infos) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode AddBlockList(const StationInfo &info) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode DelBlockList(const StationInfo &info) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode GetValidBands(std::vector<BandType> &bands) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode GetValidChannels(BandType band, std::vector<int32_t> &validchannels) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode RegisterCallBack(const sptr<IWifiHotspotCallback> &callback,
        const std::vector<std::string> &event) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode GetSupportedFeatures(long &features) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode GetSupportedPowerModel(std::set<PowerModel>& setPowerModelList) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode GetPowerModel(PowerModel& model) override
    {
        return WIFI_OPT_SUCCESS;
    }

    ErrCode SetPowerModel(const PowerModel& model) override
    {
        return WIFI_OPT_SUCCESS;
    }

    bool IsRemoteDied()
    {
        return true;
    }
};

uint32_t GetU32Data(const char* ptr)
{
    return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | (ptr[3]);
}

void OnGetSupportedFeaturesTest(const char* data, size_t size)
{
    uint32_t code = GetU32Data(data) % MAP_NUMS + WIFI_SVR_CMD_ENABLE_WIFI_AP;
    MessageParcel datas;
    datas.WriteInterfaceToken(FORMMGR_INTERFACE_TOKEN);
    datas.WriteBuffer(data, size);
    datas.RewindRead(0);
    MessageParcel reply;
    MessageOption option;
    std::shared_ptr<WifiHotspotStub> pWifiHotspotStub = std::make_shared<WifiHotSpotStubTest>();
    pWifiHotspotStub->OnRemoteRequest(WIFI_SVR_CMD_P2P_ENABLE, datas, reply, option);
}

bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    uint32_t code = GetU32Data(data) % MAP_NUMS + WIFI_SVR_CMD_ENABLE_WIFI_AP;
    MessageParcel datas;
    datas.WriteInterfaceToken(FORMMGR_INTERFACE_TOKEN);
    datas.WriteInt32(0);
    datas.WriteBuffer(data, size);
    datas.RewindRead(0);
    MessageParcel reply;
    MessageOption option;
    std::shared_ptr<WifiHotspotStub> pWifiHotspotStub = std::make_shared<WifiP2pStubTest>();
	OnGetSupportedFeaturesTest(data, size);
    pWifiHotspotStub->OnRemoteRequest(code, datas, reply, option);
    return true;
}


/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    if (data == nullptr) {
        return 0;
    }

    if (size < OHOS::Wifi::U32_AT_SIZE) {
        return 0;
    }

    /* Validate the length of size */
    if (size == 0 || size > OHOS::Wifi::FOO_MAX_LEN) {
        return 0;
    }
    char* ch = (char *)malloc(size + 1);
    if (ch == nullptr) {
        return 0;
    }
	
    (void)memset_s(ch, size + 1, 0x00, size + 1);
    if (memcpy_s(ch, size, data, size) != EOK) {
        free(ch);
        ch = nullptr;
        return 0;
    }

    OHOS::Wifi::DoSomethingInterestingWithMyAPI(ch, size);
    free(ch);
    ch = nullptr;
    return 0;
}
}
}
