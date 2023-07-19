/**
 * @file test_socket_tcp_client.cc
 * @brief 测试Socket类，tcp客户端
 * @version 0.1
 * @date 2021-09-18
 */
#include<gxh/gxh.h>

static gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

void test_tcp_client() {
    int ret;

    auto socket = gxh::Socket::CreateTCPSocket();
    GXH_ASSERT(socket);

    auto addr = gxh::Address::LookupAnyIPAddress("0.0.0.0:12345");
    GXH_ASSERT(addr);

    ret = socket->connect(addr);
    GXH_ASSERT(ret);

    GXH_LOG_INFO(g_logger) << "connect success, peer address: " << socket->getRemoteAddress()->toString();

    std::string buffer;
    buffer.resize(1024);
    socket->recv(&buffer[0], buffer.size());
    GXH_LOG_INFO(g_logger) << "recv: " << buffer;
    socket->close();

    return;
}

int main(int argc, char *argv[]) {
    gxh::EnvMgr::GetInstance()->init(argc, argv);
    gxh::Config::LoadFromConfDir(gxh::EnvMgr::GetInstance()->getConfigPath());

    gxh::IOManager iom;
    iom.schedule(&test_tcp_client);

    return 0;
}