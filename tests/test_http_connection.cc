/**
 * @file test_http_connection.cc
 * @brief HTTP客户端类测试
 * @version 0.1
 * @date 2021-12-09
 */
#include "gxh/gxh.h"
#include <iostream>

static gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

void test_pool() {
    gxh::http::HttpConnectionPool::ptr pool(new gxh::http::HttpConnectionPool(
        "www.midlane.top", "", 80, 10, 1000 * 30, 5));

    gxh::IOManager::GetThis()->addTimer(
        1000, [pool]() {
            auto r = pool->doGet("/", 300);
            std::cout << r->toString() << std::endl;
        },
        true);
}

void run() {
    gxh::Address::ptr addr = gxh::Address::LookupAnyIPAddress("www.midlane.top:80");
    if (!addr) {
        GXH_LOG_INFO(g_logger) << "get addr error";
        return;
    }

    gxh::Socket::ptr sock = gxh::Socket::CreateTCP(addr);
    bool rt                 = sock->connect(addr);
    if (!rt) {
        GXH_LOG_INFO(g_logger) << "connect " << *addr << " failed";
        return;
    }

    gxh::http::HttpConnection::ptr conn(new gxh::http::HttpConnection(sock));
    gxh::http::HttpRequest::ptr req(new gxh::http::HttpRequest);
    req->setPath("/");
    req->setHeader("host", "www.midlane.top");
    // 小bug，如果设置了keep-alive，那么要在使用前先调用一次init
    req->setHeader("connection", "keep-alive");
    req->init();
    std::cout << "req:" << std::endl
              << *req << std::endl;

    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if (!rsp) {
        GXH_LOG_INFO(g_logger) << "recv response error";
        return;
    }
    std::cout << "rsp:" << std::endl
              << *rsp << std::endl;

    std::cout << "=========================" << std::endl;

    auto r = gxh::http::HttpConnection::DoGet("http://www.midlane.top/wiki/", 300);
    std::cout << "result=" << r->result
              << " error=" << r->error
              << " rsp=" << (r->response ? r->response->toString() : "")
              << std::endl;

    std::cout << "=========================" << std::endl;
    test_pool();
}

int main(int argc, char **argv) {
    gxh::IOManager iom(2);
    iom.schedule(run);
    return 0;
}
