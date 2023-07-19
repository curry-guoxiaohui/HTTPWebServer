# 日志模块

## 日志模块概述
用于格式化输出程序日志，方便从日志中定位程序运行过程中出现的问题。这里的日志除了日志内容本身之外，还应该包括文件名/行号，时间戳，线程/协程号，模块名称，日志级别等额外信息，甚至在打印致命的日志时，还应该附加程序的栈回溯信息，以便于分析和排查问题。

从设计上看，一个完整的日志模块应该具备以下功能：

1. 区分不同的级别，比如常的DEBUG/INFO/WARN/ERROR等级别。日志模块可以通过指定级别实现只输出某个级别以上的日志，这样可以灵活开关一些不重要的日志输出，比如程序在调试阶段可以设置一个较低的级别，以便看到更多的调度日志信息，程序发布之后可以设置一个较高的级别，以减少日志输出，提高性能。

2. 区分不同的输出地。不同的日志可以输出到不同的位置，比如可以输出到标准输出，输出到文件，输出到syslog，输出到网络上的日志服务器等，甚至同一条日志可以同时输出到多个输出地。

3. 区分不同的类别。日志可以分类并命名，一个程序的各个模块可以使用不同的名称来输出日志，这样可以很方便地判断出当前日志是哪个程序模块输出的。

4. 日志格式可灵活配置。可以按需指定每条日志是否包含文件名/行号、时间戳、线程/协程号、日志级别、启动时间等内容。

5. 可通过配置文件的方式配置以上功能。

## log4cpp示例
这里先通过一个典型的C++日志框架log4cpp来分析以上几点的运行。关于log4cpp的介绍和使用可参考官网：http://log4cpp.sourceforge.net，下面是一个它的示例：
```CPP
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>
 
int main() {
    // 日志类别
    log4cpp::Category &root_logger = log4cpp::Category::getRoot(); // 获取root日志器，root日志器名称默认为空
    log4cpp::Category &file_logger = log4cpp::Category::getInstance("file_logger"); // 获取指定名称为file_logger的日志器
 
    // 日志输出地
    log4cpp::Appender *coutAppender = new log4cpp::OstreamAppender("cout", &std::cout); // 输出到终端
    log4cpp::FileAppender *fileAppender = new log4cpp::FileAppender("file", "./log.txt"); // 输出到文件
 
    // 日志格式
    log4cpp::PatternLayout *coutPattern = new log4cpp::PatternLayout();
    coutPattern->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S:%l}] %p %m%n"); // 格式：[年:月:日 时:分:秒:毫秒] 日志级别 日志内容   
    log4cpp::PatternLayout *filePattern = new log4cpp::PatternLayout();
    filePattern->setConversionPattern("[%d{%Y-%m-%d %H:%M:%S:%l}] %R %c %p %m%n"); // 格式：[年:月:日 时:分:秒:毫秒] UTC秒数 日志器名称 日志级别 日志内容  
 
    // 绑定日志器和appender，设置日志器的输出级别
    coutAppender->setLayout(coutPattern);
    root_logger.addAppender(coutAppender);
    root_logger.setPriority(log4cpp::Priority::INFO);
    fileAppender->setLayout(filePattern);
    file_logger.addAppender(fileAppender);
    file_logger.setPriority(log4cpp::Priority::ERROR);
 
    // 支持c风格的日志打印和流式的日志打印
    root_logger.debug("debug msg");
    root_logger.debugStream() << "debug msg by stream";
    root_logger.info("info msg");
    root_logger.infoStream() << "info msg by stream";
    root_logger.error("error msg");
    root_logger.errorStream() << "err msg by stream";
 
    file_logger.debug("debug msg");
    file_logger.debugStream() << "debug msg by stream";
    file_logger.info("info msg");
    file_logger.infoStream() << "info msg by stream";
    file_logger.error("error msg");
    file_logger.errorStream() << "err msg by stream";
 
    return 0;
}
```
上面的代码在终端输出如下：
```CPP
[2021-06-25 09:33:59:734] INFO info msg
[2021-06-25 09:33:59:734] INFO info msg by stream
[2021-06-25 09:33:59:734] ERROR error msg
[2021-06-25 09:33:59:734] ERROR err msg by stream
[2021-06-25 09:33:59:734] ERROR error msg
[2021-06-25 09:33:59:734] ERROR err msg by stream
```
日志文件内容如下：
```CPP
[2021-06-25 09:33:59:734] 1624584839 file_logger ERROR error msg
[2021-06-25 09:33:59:734] 1624584839 file_logger ERROR err msg by stream
```
对于log4cpp总结如下：

1. Category对应日志类别，使用不同的名称来区别日志器，使用 log4cpp::Category::getInstance() 获取指定名称的日志器实例，如果两个日志器名称相同，那么对应同一个日志器实例。

2. 使用Category的的setPriority()方法设置日志器的日志级别，日志级别使用log4cpp::Priority枚举值来表示，一共有FATAL/ALERT/CRIT/ERROR/WARN/NOTICE/INFO/DEBUG/NOTSET几个等级。

3. 使用Appender来表示日志输出地，Appender可以细分为OstreamAppender和FileAppender等不同类型。一个Category可以有多个Appender，这样一条日志就可以输出到多个地方，通过addAppender()方法为Category新增Appender。

4. 使用PatternLayout来表示日志的格式，格式通过模板字符串来指定，比如%d表示时间（后面可用{}指定具体的时间格式），%R表示UTC秒数，%c表示日志器名称，%p表示日志级别，%m表示日志消息等。

5. PatternLayout和Appender绑定，Priority和Category绑定，一条日志经过Category判断级别通过后由Appender输出，Appender输出的格式由PatternLayout指定。

除了在程序中指定日志配置，log4cpp也支持通过配置文件指定，这点参考官方示例即可

## sylar日志格式设计
下面开始sylar的日志模块设计。

首先是日志级别，这个参考log4cpp即可，一共定义以下几个级别：
```cpp
enum Level {
    /// 致命情况，系统不可用
    FATAL  = 0,
    /// 高优先级情况，例如数据库系统崩溃
    ALERT  = 100,
    /// 严重错误，例如硬盘错误
    CRIT   = 200,
    /// 错误
    ERROR  = 300,
    /// 警告
    WARN   = 400,
    /// 正常但值得注意
    NOTICE = 500,
    /// 一般信息
    INFO   = 600,
    /// 调试信息
    DEBUG  = 700,
    /// 未设置
    NOTSET = 800,
};
```
接下来是几个关键的类：
```cpp
class LogFormatter;
class LogAppender;
class Logger;
class LogEvent;
class LogEventWrap;
class LogManager;
```
关于这几个类的设计如下：

LogFormatter: 日志格式器，与log4cpp的PatternLayout对应，用于格式化一个日志事件。该类构建时可以指定pattern，表示如何进行格式化。提供format方法，用于将日志事件格式化成字符串。

LogAppender: 日志输出器，用于将一个日志事件输出到对应的输出地。该类内部包含一个LogFormatter成员和一个log方法，日志事件先经过LogFormatter格式化后再输出到对应的输出地。从这个类可以派生出不同的Appender类型，比如StdoutLogAppender和FileLogAppender，分别表示输出到终端和文件。

Logger: 日志器，负责进行日志输出。一个Logger包含多个LogAppender和一个日志级别，提供log方法，传入日志事件，判断该日志事件的级别高于日志器本身的级别之后调用LogAppender将日志进行输出，否则该日志被抛弃。

LogEvent: 日志事件，用于记录日志现场，比如该日志的级别，文件名/行号，日志消息，线程/协程号，所属日志器名称等。

LogEventWrap: 日志事件包装类，其实就是将日志事件和日志器包装到一起，因为一条日志只会在一个日志器上进行输出。将日志事件和日志器包装到一起后，方便通过宏定义来简化日志模块的使用。另外，LogEventWrap还负责在构建时指定日志事件和日志器，在析构时调用日志器的log方法将日志事件进行输出。

LogManager: 日志器管理类，单例模式，用于统一管理所有的日志器，提供日志器的创建与获取方法。LogManager自带一个root Logger，用于为日志模块提供一个初始可用的日志器。



至此，日志模块的设计就基本结束了，总结一下日志模块的工作流程：

1. 初始化LogFormatter，LogAppender, Logger。

2. 通过宏定义提供流式风格和格式化风格的日志接口。每次写日志时，通过宏自动生成对应的日志事件LogEvent，并且将日志事件和日志器Logger包装到一起，生成一个LogEventWrap对象。

3. 日志接口执行结束后，LogEventWrap对象析构，在析构函数里调用Logger的log方法将日志事件进行输出。

sylar日志模块关键实现
- **LogEvent**

    日志事件，用于记录日志现场，具体包括以下内容：

    - 日志内容
    - 日志器名称
    - 日志级别
    - 文件名，对应__FILE__宏
    - 行号，对应__LINE__宏
    - 程序运行时间，通过sylar::GetElapsedMS()获取
    - 线程ID
    - 协程ID
    - UTC时间戳，对应time(0)
    - 线程名称

    日志事件的构造需要通过宏来简化，否则，每次生成一个日志事件时都要对上面这些内容进行赋值，够麻烦的。

- **LogFormatter**

日志格式器，用于格式化一个日志事件，将其转化成一串字符串。由于一个日志事件包括了很多的内容（参考上一节，这里将每个内容称为一个item），但实际上用户并不希望每次输出日志时都将这些items全部进行输出，而是希望可以自由地选择要输出的item。并且，用户还可能需要在每条日志里增加一些指定的字符，比如在文件名和行号之间加上一个冒号的情况。为了实现这项功能，LogFormatter使用了一个模板字符串来指定格式化的方式，这个模板字符串是一串像下面这样的字符：
```
"%d{%Y-%m-%d %H:%M:%S} [%rms]%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
```
模板字符串由普通字符和转义字符构成，转义字符以%开头，比如%m，%p等。除了转义字符，剩下的全部都是普通字符，包括空格。

LogFormatter根据模板字符串来格式化日志事件。首先，在构造LogFormatter对象时会指定一串模板字符，LogFormatter会首先解析该模板字符串，将其中的转义字符和普通字符解析出来。然后，在格式化日志事件时，根据模板字符串，将其中的转义字符替换成日志事件的具体内容，普通字符保持不变。

当前实现支持以下转义字符：
```
%m 消息
%p 日志级别
%c 日志器名称
%d 日期时间，后面可跟一对括号指定时间格式，比如%d{%Y-%m-%d %H:%M:%S}，这里的格式字符与C语言strftime一致
%r 该日志器创建后的累计运行毫秒数
%f 文件名
%l 行号
%t 线程id
%F 协程id
%N 线程名称
%% 百分号
%T 制表符
%n 换行
```
假设一个LogFormatter的模板字符串为 [%c] [%p] %t %F %f:%l %m%n ，那么它输出的日志可能是像下面这样的一行字符串：
```
[root] [info] 1000 0 main.cpp:12 hello world
```
LogFormatter的实现重点是解析模板字符串，提取出其中的转义字符和普通字符，这里可以参考log.cpp，其中的LogFormatter::init()提供了一个简单的基于状态机的解析方案，支持解析上面列举的转义字符，并且支持将连续的普通字符合并成一个字符串。

- **LogAppender**

日志输出器，用于输出一个日志事件。这是一个虚类，可以派生出不同的具体实现，比如往输出到终端的StdoutLogAppender，以及输出到文件的FileLogAppender。

LogAppender的实现包含了一个用户指定的LogFormatter和一个默认的LogFormatter，以及log方法，不同类型的Appender通过重载log方法来实现往不同的目的地进行输出，这部分直接阅读源码即可，不难理解。

- **Logger**

日志器，用于输出日志。这个类是直接与用户进行交互的类，提供log方法用于输出日志事件。

Logger的实现包含了日志级别，日志器名称，创建时间，以及一个LogAppender数组，日志事件由log方法输出，log方法首先判断日志级别是否达到本Logger的级别要求，是则将日志传给各个LogAppender进行输出，否则抛弃这条日志。

- **LogEventWarp**

日志事件包装类，在日志现场构造，包装了日志器和日志事件两个对象，在日志记录结束后，LogEventWrap析构时，调用日志器的log方法输出日志事件。

- **LogManager**

日志器管理类，单例模式，用于统一管理全部的日志器，提供getLogger()方法用于创建/获取日志器。内部维护一个名称到日志器的map，当获取的日志器存在时，直接返回对应的日志器指针，否则创建对应的日志器并返回。

## 工具宏
sylar定义了一系列工具宏用于简化编码以及实现流式风格的日志输出和格式化风格的日志输出，下面以流式风格的实现为例分析一下宏的设计：
```CPP
/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 * @details 构造一个LogEventWrap对象，包裹包含日志器和日志事件，在对象析构时调用日志器写日志事件
 */
#define SYLAR_LOG_LEVEL(logger , level) \
    if(level <= logger->getLevel()) \
        sylar::LogEventWrap(logger, sylar::LogEvent::ptr(new sylar::LogEvent(logger->getName(), \
            level, __FILE__, __LINE__, sylar::GetElapsedMS() - logger->getCreateTime(), \
            sylar::GetThreadId(), sylar::GetFiberId(), time(0), sylar::GetThreadName()))).getLogEvent()->getSS()
 
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)
 
#define SYLAR_LOG_ALERT(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ALERT)
 
#define SYLAR_LOG_CRIT(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::CRIT)
 
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)
 
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)
 
#define SYLAR_LOG_NOTICE(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::NOTICE)
 
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)
 
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)
```
假如调用语句如下：
```CPP
sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
SYLAR_LOG_INFO(g_logger) << "info msg";
```
那么，这个宏将展开成下面的形式：
```CPP

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
if(sylar::LogLevel::INFO <= g_logger->getLevel)
    sylar::LogEventWrap(logger, sylar::LogEvent::ptr(new sylar::LogEvent(logger->getName(),
    level, __FILE__, __LINE__, sylar::GetElapsedMS() - logger->getCreateTime(),
    sylar::GetThreadId(), sylar::GetFiberId(), time(0), sylar::GetThreadName()))).getLogEvent()->getSS() << "info msg";
```
这里要实现通过g_logger打印一条INFO级别的消息。那么，首先判断INFO级别是否高于g_logger本身的日志级别（这里的设计与原版sylar相反，数字越小，优先级越高），如果不高于，那if语句执行不到，这条日志也不会打印，否则，临时构造一个LogEventWrap对象，传入日志器g_logger，以及现场构造的日志事件。通过LogEventWrap的getLogEvent()方法拿到日志事件，再用日志事件的流式日志消息成员输出日志消息。由于LogEventWrap是在if语句内部构建的，一旦if语句执行结束，LogEventWrap对象就会析构，日志事件也就会被g_logger进行输出，这个设计可以说是非常巧妙。

## 待补充与完善
目前来看，sylar日志模块已经实现了一个完整的日志框架，并且配合后面的配置模块，可用性很高，待补充与完善的地方主要存在于LogAppender，目前只提供了输出到终端与输出到文件两类LogAppender，但从实际项目来看，以下几种类型的LogAppender都是非常有必要的：

- Rolling File Appender，循环覆盖写文件
- Rolling Memory Appender，循环覆盖写内存缓冲区
- 支持日志文件按大小分片或是按日期分片
- 支持网络日志服务器，比如syslog