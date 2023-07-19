# 项目模块
- 日志模块([log](./log.md)) 
----
    支持流式日志风格和格式化风格写日志，支持日志格式的自定义，日志级别，多日志分离等功能。流式日志的使用：GXH_LOG_INFO(g_logger) << "this is a log";格式化日志的使用：GXH_LOG_INFO(g_logger,"%s","this is a log");支持时间，线程id，线程名称，日志级别，日志名称，文件名等内容的自由配置(eg:%%d{%%Y-%%m-%%d %%H:%%M:%%S}%%T%%t%%T%%N%%T%%F%%T[%%p]%%T[%%c]%%T%%f:%%l%%T%%m%%n)
- 环境变量模块([env](./env.md))
----
提供程序运行时的环境变量管理功能。这里的环境变量不仅包括系统环境变量，还包括程序自定义环境变量，命令行参数，帮助选项与描述，以及程序运行路径相关的信息。

提供管理环境变量管理功能，包括系统环境变量，自定义环境变量，命令行参数，帮助信息，exe名称与程序路径相关的信息。环境变量全部以key-value的形式进行存储，key和value都是字符串格式。提供add/get/has/del接口用于操作自定义环境变量和命令行选项与参数，提供setEnv/getEnv用于操作系统环境变量，提供addHelp/removeHelp/printHelp用于操作帮忙信息，提供getExe/getCwd用于获取程序名称及程序路径，提供getAbsolutePath/getAbsoluteWorkPath/getConfigPath用于获取路径相关的信息。

待改进： 使用getopt系列接口解析命令行选项与参数，这样可以支持解析选项合并和长选项，比如像"ps -auf"和"ps --help"。
- 配置模块([config](./config.md))
----
采用约定优于配置的思想。定义即可使用。支持变更通知功能。使用YAML文件做为配置内容，配置名称大小写不敏感。支持级别格式的数据类型，支持STL容器(vector,list,set,map等等),支持自定义类型的支持（需要实现序列化和反序列化方法)。
使用方式：
```CPP
static gxh::ConfigVar<int>::ptr g_tcp_connect_timeout = 
    gxh::Config::Lookup("tcp.connect.timeout", 5000, "tcp connect timeout");
```
定义了一个tcp连接超时参数，可以直接使用g_tcp_connect_timeout->getValue()获取参数的值，当配置修改重新加载，该值自动更新（并触发对应的值更新回调函数），上述配置格式如下：
```CPP
tcp:
    connect:
            timeout: 10000
``
- 线程模块([thread](./thread.md))
线程模块，封装了pthread里面的一些常用功能，Thread,Semaphore,Mutex,RWMutex,Spinlock等对象，可以方便开发中对线程日常使用。为什么不使用c++11里面的thread。 本框架是使用C++11开发，不使用thread，是因为thread其实也是基于pthread实现的。并且C++11里面没有提供读写互斥量，RWMutex，Spinlock等，在高并发场景，这些对象是经常需要用到的。所以选择了自己封装pthread。

线程模块相关的类：

Thread：线程类，构造函数传入线程入口函数和线程名称，线程入口函数类型为void()，如果带参数，则需要用std::bind进行绑定。线程类构造之后线程即开始运行，构造函数在线程真正开始运行之后返回。

线程同步类（这部分被拆分到mutex.h)中：

Semaphore: 计数信号量，基于sem_t实现
Mutex: 互斥锁，基于pthread_mutex_t实现
RWMutex: 读写锁，基于pthread_rwlock_t实现
Spinlock: 自旋锁，基于pthread_spinlock_t实现
CASLock: 原子锁，基于std::atomic_flag实现

待改进： 线程取消及线程清理
- Util与Marco模块
----
工具接口与工具类，功能宏定义。包括获取时间，日期时间格式转换，栈回溯，文件系统操作接口，类型转换接口，以及GXH_ASSERT接口。详细接口参考util.h,macro.h接口。
- 协程模块([fiber](./fiber.md))
-----
    协程：用户态的线程，相当于线程中的线程，更轻量级。后续配置socket hook，可以把复杂的异步调用，封装成同步操作。降低业务逻辑的编写复杂度。 目前该协程是基于ucontext_t来实现的，后续将支持采用boost.context里面的fcontext_t的方式实现。

协程原语：

resume：恢复，使协程进入执行状态
yield: 让出，协程让出执行权

yield和resume是同步的，也就是，一个协程的resume必然对应另一个协程的yield，反之亦然，并且，一条线程同一时间只能有一个协程是执行状态。
- 协程调度模块([Scheduler](./Scheduler.md))
-----
协程调度器，管理协程的调度，内部实现为一个线程池，支持协程在多线程中切换，也可以指定协程在固定的线程中执行。是一个N-M的协程调度模型，N个线程，M个协程。重复利用每一个线程。

限制：
一个线程只能有一个协程调度器 潜在问题：
调度器在idle情况下会疯狂占用CPU，所以，创建了几个线程，就一定要有几个类似while(1)这样的协程参与调度。


- IO协程调度模块
- 定时器模块
- hook模块
- Address模块
- Socket模块
- ByteArray模块
- Stream模块
- TcpServer类
- HTTP模块
- 守护进程