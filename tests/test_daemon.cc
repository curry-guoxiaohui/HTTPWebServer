/**
 * @file test_daemon.cc
 * @brief 守护进程测试
 * @version 0.1
 * @date 2021-12-09
 */

#include "gxh/gxh.h"

static gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

gxh::Timer::ptr timer;
int server_main(int argc, char **argv) {
    GXH_LOG_INFO(g_logger) << gxh::ProcessInfoMgr::GetInstance()->toString();
    gxh::IOManager iom(1);
    timer = iom.addTimer(
        1000, []() {
            GXH_LOG_INFO(g_logger) << "onTimer";
            static int count = 0;
            if (++count > 10) {
                exit(1);
            }
        },
        true);
    return 0;
}

int main(int argc, char **argv) {
    return gxh::start_daemon(argc, argv, server_main, argc != 1);
}
