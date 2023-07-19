#ifndef __TINY_WEBSERVER__
#define __TINY_WEBSERVER__

#include "../gxh.h"

class TinyWebserver : public gxh::http::HttpServer {
public: 
    typedef gxh::RWMutex RWMutexType;
    typedef gxh::http::FunctionServlet::callback callback;
    typedef std::shared_ptr<TinyWebserver> ptr;

    TinyWebserver(bool keepalive = false
               ,gxh::IOManager* worker = gxh::IOManager::GetThis()
               ,gxh::IOManager* io_worker = gxh::IOManager::GetThis()
               ,gxh::IOManager* accept_worker = gxh::IOManager::GetThis());

    virtual void handleClient(gxh::Socket::ptr client) override;


    ///定义
    #define XX(fun) \
    int32_t fun(gxh::http::HttpRequest::ptr req \
                    ,gxh::http::HttpResponse::ptr rsp \
                    ,gxh::http::HttpSession::ptr session);
        XX(main_page)
        XX(register_page)
        XX(log_page)
        XX(check_register_page)
        XX(check_log_page)
        XX(picture_1_page)
        XX(video_1_page)
        XX(video_1_resource)
        XX(picture_1_resource)
        XX(favicon_1_resource)
    #undef XX

    /**
     * @brief: 判断是否含有cookies
     */    
    bool checkCookie(gxh::http::HttpRequest::ptr req);

    /**
     * @brief: 删除登陆中的用户 
     */    
    void erase(std::string user) { m_userpass.erase(user); }

private:
    ///存放body文件
    std::unordered_map<std::string, std::string> m_userpass;
    RWMutexType m_mutex;
    gxh::ByteArray::ptr m_ba;
    gxh::Mysql_pool *m_sql_pool;
};


#endif