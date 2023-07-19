## 项目简介
此项目基于Linux实现的一个高性能服务器框架，基于IO协程调度模块进行IO事件的注册和事件处理，IP层实现对各类地址的封装，传输层实现创建TCP和UDP套接字的封装，在应用层实现HTTP模块的封装。框架层面，借鉴了[sylar](https://github.com/sylar-yin/sylar)开源框架的很多思想。HTTP业务层面，是之前做过的一个TinyWebServer的集成。[GXHWebServer](https://github.com/curry-guoxiaohui/GXHWebserver),最后运行的效果和之前的TinyWebServer也一样，这里就不展示了。关于sylar项目文档都不是很详细，这里推荐一位还算好一点的文档[zhongluqiang](https://www.midlane.top/wiki/pages/viewpage.action?pageId=10060952)
## 项目启动
- 开发环境
Ubutun 22.04.1  gcc 11.3 内核 5.19  MySQL 8.0.33
- 项目依赖
安装boost库
```shell
sudo apt install libboost-dev
```
安装yaml-cpp
```shell
git clone git@github.com:jbeder/yaml-cpp.git
cd yaml-cpp
mkdir build
cd build
cmake ..
make & make install
```
安装openssl开发库
```shell
sudo apt-get install libssl-dev
```
安装Apache ab测试工具
```shell
sudo apt install apache2-utils
```
其他依赖
```shell
sudo apt install cmake graphviz doxygen
```
启用
```shell
mkdir build
cd build
cmake ..
make
../bin/test-  (如：../bin/test_config)
```

WebServer项目启动(端口默认是8020 需要提前创建数据库，数据库信息都在db配置文件中有记录)
```
../bin/tiny_webserver
```