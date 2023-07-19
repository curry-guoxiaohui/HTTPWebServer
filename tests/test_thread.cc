/**
 * @file test_thread.cc
 * @brief 线程模块测试
 * @version 0.1
 * @date 2021-06-15
 */
#include "gxh/gxh.h"

gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

int count = 0;
gxh::Mutex s_mutex;

void func1(void *arg) {
    GXH_LOG_INFO(g_logger) << "name:" << gxh::Thread::GetName()
        << " this.name:" << gxh::Thread::GetThis()->getName()
        << " thread name:" << gxh::GetThreadName()
        << " id:" << gxh::GetThreadId()
        << " this.id:" << gxh::Thread::GetThis()->getId();
    GXH_LOG_INFO(g_logger) << "arg: " << *(int*)arg;
    for(int i = 0; i < 10000; i++) {
        gxh::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

int main(int argc, char *argv[]) {
    gxh::EnvMgr::GetInstance()->init(argc, argv);
    gxh::Config::LoadFromConfDir(gxh::EnvMgr::GetInstance()->getConfigPath());

    std::vector<gxh::Thread::ptr> thrs;
    int arg = 123456;
    for(int i = 0; i < 3; i++) {
        // 带参数的线程用std::bind进行参数绑定
        gxh::Thread::ptr thr(new gxh::Thread(std::bind(func1, &arg), "thread_" + std::to_string(i)));
        thrs.push_back(thr);
    }

    for(int i = 0; i < 3; i++) {
        thrs[i]->join();
    }
    
    GXH_LOG_INFO(g_logger) << "count = " << count;
    return 0;
}

