#ifndef __GXH_HTTP_CGI__
#define __GXH_HTTP_CGI__
#include <iostream>
#include <memory>
#include <string>
#include "../mutex.h"
class http_cgi
{
public:
    typedef std::shared_ptr<http_cgi> ptr;
    typedef gxh::Mutex MutexType;
    http_cgi(){}
    ~http_cgi(){}
    const std::string ROOT =  "/home/guoxiaohui/sylar-from-scratch/tests";

    std::string getfile(int cgi,std::string&filename);



private:
    int m_cgi;
    std::string m_filename;
};
#endif