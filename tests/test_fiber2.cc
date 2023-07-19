/**
 * @file test_fiber2.cc
 * @brief 协程测试，用于演示非对称协程
 * @version 0.1
 * @date 2021-06-15
 */
#include "gxh/fiber.h"
#include "gxh/gxh.h"
#include <string>
#include <vector>

gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

void run_in_fiber2() {
    GXH_LOG_INFO(g_logger) << "run_in_fiber2 begin";
    GXH_LOG_INFO(g_logger) << "run_in_fiber2 end";
}

void run_in_fiber() {
    GXH_LOG_INFO(g_logger) << "run_in_fiber begin";

    /**
     * 非对称协程，子协程不能创建并运行新的子协程，下面的操作是有问题的，
     * 子协程再创建子协程，原来的主协程就跑飞了
     */
    gxh::Fiber::ptr fiber(new gxh::Fiber(run_in_fiber2, 0, false));
    fiber->resume();

    GXH_LOG_INFO(g_logger) << "run_in_fiber end";
}

int main(int argc, char *argv[]) {
    gxh::EnvMgr::GetInstance()->init(argc, argv);
    gxh::Config::LoadFromConfDir(gxh::EnvMgr::GetInstance()->getConfigPath());

    GXH_LOG_INFO(g_logger) << "main begin";

    gxh::Fiber::GetThis();

    gxh::Fiber::ptr fiber(new gxh::Fiber(run_in_fiber, 0, false));
    fiber->resume();

    GXH_LOG_INFO(g_logger) << "main end";
    return 0;
}