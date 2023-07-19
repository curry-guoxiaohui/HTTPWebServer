/**
 * @file test_util.cpp
 * @brief util与macro测试
 * @version 0.1
 * @date 2021-06-12
 */
#include "gxh/gxh.h"

gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

void test2() {
    std::cout << gxh::BacktraceToString() << std::endl;
}
void test1() {
    test2();
}

void test_backtrace() {
    test1();
}

int main() {
    GXH_LOG_INFO(g_logger) << gxh::GetCurrentMS();
    GXH_LOG_INFO(g_logger) << gxh::GetCurrentUS();
    GXH_LOG_INFO(g_logger) << gxh::ToUpper("hello");
    GXH_LOG_INFO(g_logger) << gxh::ToLower("HELLO");
    GXH_LOG_INFO(g_logger) << gxh::Time2Str();
    GXH_LOG_INFO(g_logger) << gxh::Str2Time("1970-01-01 00:00:00"); // -28800

    std::vector<std::string> files;
    gxh::FSUtil::ListAllFile(files, "../gxh", ".cpp");
    for (auto &i : files) {
        GXH_LOG_INFO(g_logger) << i;
    }

    // todo, more...

    test_backtrace();

    GXH_ASSERT2(false, "assert");
    return 0;
}