#include "http_cgi.h"
#include <fstream>
#include "../log.h"

static gxh::Logger::ptr g_logger = GXH_LOG_NAME("name");

std::string http_cgi::getfile(int cgi,std::string&filename)
{
    std::ifstream fin(ROOT + filename);
    if(!fin.is_open())
    {
        GXH_LOG_INFO(g_logger) << "open fail";
    }
    //将文件读入到ostringstream对象buf中
    std::ostringstream buf;
    char ch;
    while(buf&&fin.get(ch))
    buf.put(ch);
    //返回与流对象buf关联的字符串
    return buf.str();
}