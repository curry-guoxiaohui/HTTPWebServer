/**
 * @file test_log.cpp
 * @brief 日志类测试
 * @version 0.1
 * @date 2021-06-10
 */

#include "gxh/gxh.h"

#include <unistd.h>

gxh::Logger::ptr g_logger = GXH_LOG_ROOT(); // 默认INFO级别

int main(int argc, char *argv[]) {
    gxh::EnvMgr::GetInstance()->init(argc, argv);
    gxh::Config::LoadFromConfDir(gxh::EnvMgr::GetInstance()->getConfigPath());

    GXH_LOG_FATAL(g_logger) << "fatal msg";
    GXH_LOG_ERROR(g_logger) << "err msg";
    GXH_LOG_INFO(g_logger) << "info msg";
    GXH_LOG_DEBUG(g_logger) << "debug msg";

    GXH_LOG_FMT_FATAL(g_logger, "fatal %s:%d", __FILE__, __LINE__);
    GXH_LOG_FMT_ERROR(g_logger, "err %s:%d", __FILE__, __LINE__);
    GXH_LOG_FMT_INFO(g_logger, "info %s:%d", __FILE__, __LINE__);
    GXH_LOG_FMT_DEBUG(g_logger, "debug %s:%d", __FILE__, __LINE__);
   
    sleep(1);
    gxh::SetThreadName("brand_new_thread");

    g_logger->setLevel(gxh::LogLevel::WARN);
    GXH_LOG_FATAL(g_logger) << "fatal msg";
    GXH_LOG_ERROR(g_logger) << "err msg";
    GXH_LOG_INFO(g_logger) << "info msg"; // 不打印
    GXH_LOG_DEBUG(g_logger) << "debug msg"; // 不打印


    gxh::FileLogAppender::ptr fileAppender(new gxh::FileLogAppender("./log.txt"));
    g_logger->addAppender(fileAppender);
    GXH_LOG_FATAL(g_logger) << "fatal msg";
    GXH_LOG_ERROR(g_logger) << "err msg";
    GXH_LOG_INFO(g_logger) << "info msg"; // 不打印
    GXH_LOG_DEBUG(g_logger) << "debug msg"; // 不打印

    gxh::Logger::ptr test_logger = GXH_LOG_NAME("test_logger");
    gxh::StdoutLogAppender::ptr appender(new gxh::StdoutLogAppender);
    gxh::LogFormatter::ptr formatter(new gxh::LogFormatter("%d:%rms%T%p%T%c%T%f:%l %m%n")); // 时间：启动毫秒数 级别 日志名称 文件名：行号 消息 换行
    appender->setFormatter(formatter);
    test_logger->addAppender(appender);
    test_logger->setLevel(gxh::LogLevel::WARN);

    GXH_LOG_ERROR(test_logger) << "err msg";
    GXH_LOG_INFO(test_logger) << "info msg"; // 不打印

    // 输出全部日志器的配置
    g_logger->setLevel(gxh::LogLevel::INFO);
    GXH_LOG_INFO(g_logger) << "logger config:" << gxh::LoggerMgr::GetInstance()->toYamlString();

    return 0;
}