/**
 * @file test_timer.cc
 * @brief IO协程测试器定时器测试
 * @version 0.1
 * @date 2021-06-19
 */

#include "gxh/gxh.h"

static gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

static int timeout = 1000;
static gxh::Timer::ptr s_timer;

void timer_callback() {
    GXH_LOG_INFO(g_logger) << "timer callback, timeout = " << timeout;
    timeout += 1000;
    if(timeout < 5000) {
        s_timer->reset(timeout, true);
    } else {
        s_timer->cancel();
    }
}

void test_timer() {
    gxh::IOManager iom;

    // 循环定时器
    s_timer = iom.addTimer(1000, timer_callback, true);
    
    // 单次定时器
    iom.addTimer(500, []{
        GXH_LOG_INFO(g_logger) << "500ms timeout";
    });
    iom.addTimer(5000, []{
        GXH_LOG_INFO(g_logger) << "5000ms timeout";
    });
}

int main(int argc, char *argv[]) {
    gxh::EnvMgr::GetInstance()->init(argc, argv);
    gxh::Config::LoadFromConfDir(gxh::EnvMgr::GetInstance()->getConfigPath());

    test_timer();

    GXH_LOG_INFO(g_logger) << "end";

    return 0;
}