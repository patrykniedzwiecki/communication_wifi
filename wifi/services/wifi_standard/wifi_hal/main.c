/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#include <securec.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "server.h"
#include "wifi_common_def.h"
#include "wifi_hal_adapter.h"
#include "wifi_hal_ap_interface.h"
#include "wifi_hal_crpc_server.h"
#include "wifi_hal_p2p_interface.h"
#include "wifi_hal_sta_interface.h"
#include "wifi_hdi_sta_impl.h"
#include "wifi_hostapd_hal.h"
#include "wifi_log.h"
#include "wifi_hal_module_manage.h"

#undef LOG_TAG
#define LOG_TAG "WifiHalService"

#define BUF_LEN 32
#define INVALID_PID (-1)
#define WIFI_SERVICE_NAME "wifi_manager_se"

const int SLEEP_TIME_US = 1000 * 500; // 100ms

pid_t GetPID(const char *pidFile)
{
    struct stat sb;
    if (stat(pidFile, &sb) != 0) {
        LOGW("GetPID() pidFile:%{public}s stat error:%{public}d!", pidFile, errno);
        return -1;
    }
    LOGI("GetPID() pidFile:%{public}s stat st_size:%{public}d.", pidFile, (int)sb.st_size);

    int fd;
    if ((fd = open(pidFile, O_RDONLY)) < 0) {
        LOGE("GetPID() failed, open pidFile:%{public}s error!", pidFile);
        return -1;
    }

    lseek(fd, 0, SEEK_SET);

    char buf[PID_MAX_LENGTH] = {0};
    ssize_t bytes;
    if ((bytes = read(fd, buf, sb.st_size)) < 0) {
        LOGE("GetPID() failed, read pidFile:%{public}s error, bytes:%{public}zd!", pidFile, bytes);
        close(fd);
        return -1;
    }
    LOGI("GetPID() read pidFile:%{public}s, buf:%{public}s, bytes:%{public}zd.", pidFile, buf, bytes);
    close(fd);

    return atoi(buf);
}

void StopProcess(void)
{
    char pidFile[DIR_MAX_LENGTH] = {0, };
    int n = snprintf_s(pidFile, DIR_MAX_LENGTH, DIR_MAX_LENGTH - 1, "%s/%s.pid", CONFIG_ROOR_DIR, WIFI_MANAGGER_PID_NAME);
    if (n < 0) {
        LOGE("InitPidfile: construct pidFile name failed.");
        return;
    }

    LOGI("StopProcess() begin, pidFile:%{public}s.", pidFile);
    pid_t pid = GetPID(pidFile);
    if (pid <= 0) {
        LOGW("StopProcess() GetPID pidFile:%{public}s, pid == -1!", pidFile);
        return;
    }

    LOGI("StopProcess() sending signal SIGTERM to process:%{public}d.", pid);
    if (kill(pid, SIGTERM) == -1) {
        if (ESRCH == errno) {
            LOGW("StopProcess() pidFile:%{public}s,pid:%{public}d is not exist.", pidFile, pid);
            unlink(pidFile);
            return;
        }
        LOGE("StopProcess() cmd: [kill %{public}d] failed, kill error:%{public}d!", pid, errno);
        return;
    }

    unlink(pidFile);
    return;
}

static void SignalExit(int sig)
{
    LOGI("Caught signal %{public}d", sig);
    RpcServer *server = GetRpcServer();
    if (server != NULL) {
        StopEventLoop(server->loop);
    }
    return;
}

// static void RestartWpaSuppilicant(ModuleInfo *p)
// {
//     if (p == NULL) {
//         return;
//     }

//     if (p->referenceCount > 1) {
//         if (Stop() != WIFI_HAL_SUCCESS) {
//             LOGE("Sta stop failed.");
//         }
//         if (P2pStop() != WIFI_HAL_SUCCESS) {
//             LOGE("P2p stop failed.");
//         }

//         usleep(SLEEP_TIME_US);

//         if (Start() != WIFI_HAL_SUCCESS) {
//             LOGE("Sta restart failed.");
//         }
//         if (P2pStart() != WIFI_HAL_SUCCESS) {
//             LOGE("P2p restart failed.");
//         }
//     } else {
//         if (Stop() != WIFI_HAL_SUCCESS) {
//             LOGE("Sta stop failed.");
//         }
//         usleep(SLEEP_TIME_US);
//         if (Start() != WIFI_HAL_SUCCESS) {
//             LOGE("Sta restart failed.");
//         }
//     }
// }

// static void RestartSoftAp(ModuleInfo *p)
// {
//     if (p == NULL) {
//         return;
//     }

//     int apCount = p->referenceCount;
//     int stopCount = 0;
//     while (stopCount < apCount) {
//         if (StopSoftAp(stopCount) != WIFI_HAL_SUCCESS) {
//             LOGE("Ap instance %{public}d stop failed.", stopCount);
//         }
//         stopCount += 1;
//     }
//     usleep(SLEEP_TIME_US);

//     int startCount = 0;
//     while (startCount < apCount) {
//         if (StartSoftAp(startCount) != WIFI_HAL_SUCCESS) {
//             LOGE("Ap instance %{public}d restart failed.", startCount); 
//         }
//         startCount += 1;
//     }
// }

// static void SignalChildExit(int sig)
// {
//     return;
//     // LOGI("Caught child process stop signal %{public}d", sig);
//     // ModuleInfo *p = NULL;
//     // p = GetStartedModule();
//     // if (p == NULL) {
//     //     LOGI("No wpa process need to restart.");
//     //     return;
//     // }

//     // if (strcmp(p->szModuleName, WPA_SUPPLICANT_NAME) == 0) {
//     //     RestartWpaSuppilicant(p);
//     // } else if (strcmp(p->szModuleName, WPA_HOSTAPD_NAME) == 0) {
//     //     RestartSoftAp(p);
//     // }
// }

int main(void)
{
    LOGI("Wifi hal service starting...");
    char rpcSockPath[] = CONFIG_ROOR_DIR"/unix_sock.sock";
    if (access(rpcSockPath, 0) == 0) {
        unlink(rpcSockPath);
    }
    if (InitRpcFunc() < 0) {
        LOGE("Init Rpc Function failed!");
        return -1;
    }
    RpcServer *server = CreateRpcServer(rpcSockPath);
    if (server == NULL) {
        LOGE("Create RPC Server by %{public}s failed!", rpcSockPath);
        return -1;
    }
    SetRpcServerInited(server);
    setvbuf(stdout, NULL, _IOLBF, 0);
    signal(SIGINT, SignalExit);
    signal(SIGTERM, SignalExit);
    signal(SIGPIPE, SIG_IGN);
    // signal(SIGCHLD, SignalChildExit);
#ifdef HDI_INTERFACE_SUPPORT
    HdiStaInit();
#endif
    StopProcess();
    RunRpcLoop(server);
    /* stop wpa_supplicant, hostapd, and other resources */
#ifdef HDI_INTERFACE_SUPPORT
    HdiStaUnInit();
#endif
    ForceStop();
    for (int id = 0; id < AP_MAX_INSTANCE; id++) {
        StopSoftAp(id);
    }
    P2pForceStop();
    ReleaseWifiHalVendorInterface();
    /* clear RPC Server */
    SetRpcServerInited(NULL);
    ReleaseRpcServer(server);
    ReleaseRpcFunc();
    LOGI("hal service exists!");
    return 0;
}
