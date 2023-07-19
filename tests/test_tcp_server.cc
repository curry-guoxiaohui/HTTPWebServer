/**
 * @file test_tcp_server.cc
 * @brief TcpServer类测试
 * @version 0.1
 * @date 2021-09-18
 */
#include "gxh/gxh.h"

static gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

/**
 * @brief 自定义TcpServer类，重载handleClient方法
 */
class MyTcpServer : public gxh::TcpServer {
protected:
    virtual void handleClient(gxh::Socket::ptr client) override;
};

void MyTcpServer::handleClient(gxh::Socket::ptr client) {
    GXH_LOG_INFO(g_logger) << "new client: " << client->toString();
    static std::string buf;
    buf.resize(4096);
    client->recv(&buf[0], buf.length()); // 这里有读超时，由tcp_server.read_timeout配置项进行配置，默认120秒
    GXH_LOG_INFO(g_logger) << "recv: " << buf;
    client->close();
}

void run() {
    gxh::TcpServer::ptr server(new MyTcpServer); // 内部依赖shared_from_this()，所以必须以智能指针形式创建对象
    auto addr = gxh::Address::LookupAny("0.0.0.0:12345");
    GXH_ASSERT(addr);
    std::vector<gxh::Address::ptr> addrs;
    addrs.push_back(addr);

    std::vector<gxh::Address::ptr> fails;
    while(!server->bind(addrs, fails)) {
        sleep(2);
    }
    
    GXH_LOG_INFO(g_logger) << "bind success, " << server->toString();

    server->start();
}

int main(int argc, char *argv[]) {
    gxh::EnvMgr::GetInstance()->init(argc, argv);
    gxh::Config::LoadFromConfDir(gxh::EnvMgr::GetInstance()->getConfigPath());

    gxh::IOManager iom(2);
    iom.schedule(&run);

    return 0;
}