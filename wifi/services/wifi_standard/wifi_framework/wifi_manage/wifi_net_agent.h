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

#ifndef WIFI_NET_AGENT_H
#define WIFI_NET_AGENT_H

#include <memory>
#include <string>
#include <singleton.h>
#include <utility>
#include <vector>

#include "i_net_conn_service.h"
#include "net_all_capabilities.h"
#include "net_supplier_callback_base.h"
#include "wifi_net_conn_event_handler.h"
#include "wifi_internal_msg.h"
#include "sta_service_callback.h"
#include "wifi_log.h"

namespace OHOS {
namespace Wifi {
class WifiNetAgent : public DelayedRefSingleton<WifiNetAgent> {
    DECLARE_DELAYED_REF_SINGLETON(WifiNetAgent);

public:
    /**
     * Register the network information with the network management module
     *
     * @return true if register success else return false;
     */
    bool RegisterNetSupplier();
    /**
     * Register the network callback with the network management module
     *
     * @return true if register success else return false;
     */
    bool RegisterNetSupplierCallback();

    /**
     * Cancel the registration information to the network management
     */
    void UnregisterNetSupplier();

    /**
     * Update network information
     *
     * @param supplierId network unique identity id returned after network registration
     * @param netSupplierInfo network data information
     */
    void UpdateNetSupplierInfo(const sptr<NetManagerStandard::NetSupplierInfo> &netSupplierInfo);

    /**
     * Update link information
     *
     * @param wifiIpInfo wifi network link data information
     * @param wifiProxyConfig wifi network link proxy information
     */
    void UpdateNetLinkInfo(IpInfo &wifiIpInfo, WifiProxyConfig &wifiProxyConfig);

    /**
     * Add route
     *
     * @param interface interface name
     * @param ipAddress IP address
     * @param prefixLength prefix length
     */
    bool AddRoute(const std::string interface, const std::string ipAddress, int prefixLength);

    /**
     * Add OnStaMachineUpdateNetLinkInfo
     *
     * @param wifiIpInfo wifi network link data information
     * @param wifiProxyConfig wifi network link proxy information
     */
    void OnStaMachineUpdateNetLinkInfo(IpInfo &wifiIpInfo, WifiProxyConfig &wifiProxyConfig);

    /**
     * Add OnStaMachineUpdateNetSupplierInfo
     *
     * @param netSupplierInfo net Supplier Info
     */
    void OnStaMachineUpdateNetSupplierInfo(const sptr<NetManagerStandard::NetSupplierInfo> &netSupplierInfo);

    /**
     * Add OnStaMachineWifiStart
     *
     * @param
     */
    void OnStaMachineWifiStart();

    /**
     * Add OnStaMachineNetManagerRestart
     *
     * @param netSupplierInfo net Supplier Info
     */
    void OnStaMachineNetManagerRestart(const sptr<NetManagerStandard::NetSupplierInfo> &netSupplierInfo);

public:
    class NetConnCallback : public NetManagerStandard::NetSupplierCallbackBase {
    public:
        /**
         * @Description : Construct a new NetConn object
         *
         */
        explicit NetConnCallback();

        /**
         * @Description : Destroy the NetConn object
         *
         */
        ~NetConnCallback() override;

        /**
         * @Description : Connection Management triggers the open automatic connection feature.
         *
         * @param ident - identity
         * @param netCaps - Net capability to request a network
         */
        int32_t RequestNetwork(const std::string &ident, const std::set<NetManagerStandard::NetCap> &netCaps) override;
        /**
         * @Description : Connection Management triggers the close automatic connection feature.
         *
         * @param ident - identity
         * @param netCaps - Net capability to request a network
         */
        int32_t ReleaseNetwork(const std::string &ident, const std::set<NetManagerStandard::NetCap> &netCaps) override;
    private:
        void LogNetCaps(const std::string &ident, const std::set<NetManagerStandard::NetCap> &netCaps) const;
    };
private:
    void CreateNetLinkInfo(sptr<NetManagerStandard::NetLinkInfo> &netLinkInfo, IpInfo &wifiIpInfo,
        WifiProxyConfig &wifiProxyConfig);

private:
    uint32_t supplierId;
    std::shared_ptr<AppExecFwk::EventRunner> netConnEventRunner_ = nullptr;
    std::shared_ptr<WifiNetConnEventHandler> netConnEventHandler_ = nullptr;
};
} // namespace Wifi
} // namespace OHOS
#endif // CELLULAR_DATA_NET_AGENT_H
