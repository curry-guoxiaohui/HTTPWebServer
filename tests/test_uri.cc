/**
 * @file test_uri.cc 
 * @brief URI类测试
 * @version 0.1
 * @date 2021-11-14
 */

#include "gxh/gxh.h"
#include <iostream>

int main(int argc, char * argv[]) {
    auto uri = gxh::Uri::Create("http://a:b@host.com:8080/p/a/t/h?query=string#hash");
    std::cout << uri->toString() << std::endl;
    return 0;
}