/**
 * @file test_socket.cc
 * @brief 测试Socket类，tcp服务器
 * @version 0.1
 * @date 2021-09-18
 */
#include <gxh/gxh.h>

static gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

void test_tcp_server() {
    int ret;

    auto addr = gxh::Address::LookupAnyIPAddress("0.0.0.0:12345");
    GXH_ASSERT(addr);

    auto socket = gxh::Socket::CreateTCPSocket();
    GXH_ASSERT(socket);

    ret = socket->bind(addr);
    GXH_ASSERT(ret);
    
    GXH_LOG_INFO(g_logger) << "bind success";

    ret = socket->listen();
    GXH_ASSERT(ret);

    GXH_LOG_INFO(g_logger) << socket->toString() ;
    GXH_LOG_INFO(g_logger) << "listening...";

    while(1) {
        auto client = socket->accept();
        GXH_ASSERT(client);
        GXH_LOG_INFO(g_logger) << "new client: " << client->toString();
        client->send("hello world", strlen("hello world"));
        client->close();
    }
}

int main(int argc, char *argv[]) {
    gxh::EnvMgr::GetInstance()->init(argc, argv);
    gxh::Config::LoadFromConfDir(gxh::EnvMgr::GetInstance()->getConfigPath());

    gxh::IOManager iom(2);
    iom.schedule(&test_tcp_server);

    return 0;
}