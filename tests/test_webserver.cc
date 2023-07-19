/**
 * @file test_http_server.cc
 * @brief HttpServer测试
 * @version 0.1
 * @date 2021-09-28
 */
#include "gxh/gxh.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

static gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

#define XX(...) #__VA_ARGS__

gxh::IOManager::ptr worker;
std::string str;
long bufSize = 10*1024;	//缓冲区大小
char* buffer;	//缓冲区保存文件数据
void run() {
    g_logger->setLevel(gxh::LogLevel::INFO);
    //gxh::http::HttpServer::ptr server(new gxh::http::HttpServer(true, worker.get(), gxh::IOManager::GetThis()));
    gxh::http::HttpServer::ptr server(new gxh::http::HttpServer(true));
    gxh::Address::ptr addr = gxh::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while (!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/gxh/xx", [](gxh::http::HttpRequest::ptr req, gxh::http::HttpResponse::ptr rsp, gxh::http::HttpSession::ptr session) {
        rsp->setBody(req->toString());
        return 0;
    });

    
    sd->addGlobServlet("/gxh/*", [](gxh::http::HttpRequest::ptr req, gxh::http::HttpResponse::ptr rsp, gxh::http::HttpSession::ptr session) {
        // std::string filename= "/home/guoxiaohui/sylar-from-scratch/tests" + req->getPath().substr(req->getPath().find_last_of("/"));
        http_cgi::ptr cgi = std::shared_ptr<http_cgi>(new http_cgi);
        std::string path = req->getPath();
         GXH_LOG_INFO(g_logger) << cgi->getfile(0,path);
        rsp->setBody(cgi->getfile(0,path));
        return 0;
    });


    server->start();
}

int main(int argc, char **argv) {
    gxh::EnvMgr::GetInstance()->init(argc, argv);
    gxh::Config::LoadFromConfDir(gxh::EnvMgr::GetInstance()->getConfigPath());
    struct stat st;
    
    if(lstat("/home/guoxiaohui/sylar-from-scratch/tests/index.html",&st) < 0)
    {
        GXH_LOG_INFO(g_logger) << "file not exists";
    }


    gxh::IOManager iom(1, true, "main");
    worker.reset(new gxh::IOManager(3, false, "worker"));
    iom.schedule(run);
    return 0;
}
