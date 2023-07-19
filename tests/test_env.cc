/**
 * @file test_env.cc
 * @brief 环境变量测试
 * @version 0.1
 * @date 2021-06-13
 */
#include "gxh/gxh.h"

gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

gxh::Env *g_env = gxh::EnvMgr::GetInstance();

int main(int argc, char *argv[]) {
    g_env->addHelp("h", "print this help message");

    bool is_print_help = false;
    if(!g_env->init(argc, argv)) {
        is_print_help = true;
    }
    if(g_env->has("h")) {
        is_print_help = true;
    }

    if(is_print_help) {
        g_env->printHelp();
        return false;
    }

    GXH_LOG_INFO(g_logger)<< "exe: " << g_env->getExe();
    GXH_LOG_INFO(g_logger) <<"cwd: " << g_env->getCwd();
    GXH_LOG_INFO(g_logger) <<"program: " << g_env->getProgram();
    GXH_LOG_INFO(g_logger) << "absoluth path of test: " << g_env->getAbsolutePath("test");
    GXH_LOG_INFO(g_logger) << "conf path:" << g_env->getConfigPath();

    g_env->add("key1", "value1");
    GXH_LOG_INFO(g_logger) << "key1: " << g_env->get("key1");

    g_env->setEnv("key2", "value2");
    GXH_LOG_INFO(g_logger) << "key2: " << g_env->getEnv("key2");

    GXH_LOG_INFO(g_logger) << g_env->getEnv("PATH");

    return 0;
}