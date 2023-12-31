# 环境变量模块

## 概述
提供程序运行时的环境变量管理功能。这里的环境变量不仅包括系统环境变量，还包括程序自定义环境变量，命令行参数，帮助选项与描述，以及程序运行路径相关的信息。

所谓环境变量就是程序运行时可直接获取和设置的一组变量，它们往往代表一些特定的含义。所有的环境变量都以key-value的形式存储，key和value都是字符串形式。这里可以参考系统环境变量来理解，在程序运行时，可以通过调用getenv()/setenv()接口来获取/设置系统环境变量，比如getenv("PWD")来获取当前路径。在shell中可以通过printenv命令来打印当前所有的环境变量，并且在当前shell中运行的所有程序都共享这组环境变量值。

其他类型的环境变量也可以类比系统环境变量，只不过系统环境变量由shell来保存，而其他类型的环境变量由程序自己内部存储，但两者效果是一样的。具体地，sylar定义了以下几类环境变量：

1. 系统环境变量，由shell保存，sylar环境变量模块提供getEnv()/setEnv()方法用于操作系统环境变量。

2. 程序自定义环境变量，对应get()/add()/has()/del()接口，自定义环境变量保存在程序自己的内存空间中，在内部实现为一个std::map<std::string, std::string>结构。

3. 命令行参数，通过解析main函数的参数得到。所有参数都被解析成选项-选项值的形式，选项只能以-开头，后面跟选项值。如果一个参数只有选项没有值，那么值为空字符串。命令行参数也保存在程序自定义环境变量中。

4. 帮助选项与描述。这里是为了统一生成程序的命令行帮助信息，在执行程序时如果指定了-h选项，那么就打印这些帮助信息。帮助选项与描述也是存储在程序自己的内存空间中，在内部实现为一个std::vector<std::pair<std::string, std::string>>结构。

5. 与程序运行路径相关的信息，包括记录程序名，程序路径，当前路径，这些由单独的成员变量来存储。

## sylar环境变量模块设计
与环境变量相关的类只有一个class Env，并且这个类被包装成了单例模式。通过单例可以保证程序的环境变量是全局唯一的，便于统一管理。Env类提供以下方法：

1. init: 环境变量模块初始化，需要将main函数的参数原样传入init接口中，以便于从main函数参数中提取命令行选项与值，以及通过argv[0]参数获取命令行程序名称。

2. add/get/has/del：用于操作程序自定义环境变量，参数为key-value，get操作支持传入默认值，在对应的环境变量不存在时，返回这个默认值。

3. setEnv/getEnv: 用于操作系统环境变量，对应标准库的setenv/getenv操作。

3. addHelp/removeHelp/printHelp: 用于操作帮助选项和描述信息。

4. getExe/getCwd/getAbsolutePath: 用于获取程序名称，程序路径，绝对路径。

5. getConfigPath: 获取配置文件夹路径，配置文件夹路径由命令行-c选项传入。

## sylar环境变量模块实现
这里只描述环境变量环境的一些实现细节。

- 获取程序的bin文件绝对路径是通过/proc/$pid/目录下exe软链接文件指向的路径来确定的，用到了readlink(2)系统调用。
- 通过bin文件绝对路径可以得到bin文件所在的目录，只需要将最后的文件名部分去掉即可。
- 通过argv[0]获得命令行输入的程序路径，注意这里的路径可能是以./开头的相对路径。
- 通过setenv/getenv操作系统环境变量，参考setenv(3), getenv(3)。
- 提供getAbsolutePath方法，传入一个相对于bin文件的路径，返回这个路径的绝对路径。比如默认的配置文件路径就是通过getAbsolutePath(get("c", "conf"))来获取的，也就是配置文件夹默认在bin文件所在目录的conf文件夹。
- 按使用惯例，main函数执行的第一条语句应该就是调用Env的init方法初始化命令行参数。

## 待补充和完善
sylar在解析命令行参数时，没有使用getopt()/getopt_long()接口，而是使用了自己编写的解析代码，这就导致sylar的命令行参数不支持长选项和选项合并，像ps -aux这样的多个选项组合在一起的命令行参数以及ps --help这样的长选项是不支持的。