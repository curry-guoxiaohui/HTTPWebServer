/**
 * @file test_http_server.cc
 * @brief HttpServer测试
 * @version 0.1
 * @date 2021-09-28
 */
#include "gxh/gxh.h"

static gxh::Logger::ptr g_logger = GXH_LOG_ROOT();

#define XX(...) #__VA_ARGS__

gxh::IOManager::ptr worker;

void run() {
    g_logger->setLevel(gxh::LogLevel::ERROR);
    //gxh::http::HttpServer::ptr server(new gxh::http::HttpServer(true, worker.get(), gxh::IOManager::GetThis()));
    gxh::http::HttpServer::ptr server(new gxh::http::HttpServer(true));
    gxh::Address::ptr addr = gxh::Address::LookupAnyIPAddress("0.0.0.0:8022");
    while (!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/gxh/xx", [](gxh::http::HttpRequest::ptr req, gxh::http::HttpResponse::ptr rsp, gxh::http::HttpSession::ptr session) {
        rsp->setBody(req->toString());
        return 0;
    });

    sd->addGlobServlet("/gxh/*", [](gxh::http::HttpRequest::ptr req, gxh::http::HttpResponse::ptr rsp, gxh::http::HttpSession::ptr session) {
        rsp->setBody("Path:"+ req->getPath() +" \r\nGlob:\r\n" + req->toString());
        

 
        return 0;
    });

    sd->addGlobServlet("/gxhx/*", [](gxh::http::HttpRequest::ptr req, gxh::http::HttpResponse::ptr rsp, gxh::http::HttpSession::ptr session) {
        rsp->setBody(XX(<html>
                                <head><title> 404 Not Found</ title></ head>
                                <body>
                                <center><h1> 404 Not Found</ h1></ center>
                                <hr><center>
                                    nginx /
                                1.16.0 <
                            / center >
                            <h1>sorry</h1>
                            </ body>
                            </ html> < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >
                            < !--a padding to disable MSIE and
                        Chrome friendly error page-- >));
        return 0;
    });

    server->start();
}

int main(int argc, char **argv) {
    gxh::EnvMgr::GetInstance()->init(argc, argv);
    gxh::Config::LoadFromConfDir(gxh::EnvMgr::GetInstance()->getConfigPath());
    
    gxh::IOManager iom(1, true, "main");
    worker.reset(new gxh::IOManager(3, false, "worker"));
    iom.schedule(run);
    return 0;
}
