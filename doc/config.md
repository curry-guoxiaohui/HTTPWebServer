# 配置模块

## 配置模块概述
用于定义/声明配置项，并且从配置文件中加载用户配置。



一般而言，一项配置应该包括以下要素：

1. 名称，对应一个字符串，必须唯一，不能与其他配置项产生冲突。

2. 类型，可以是基本类型，但也应该支持复杂类型和自定义类型。

3. 默认值，考虑到用户不一定总是会显式地给配置项赋值，所以配置项最好有一个默认值。

4. 配置变更通知，一旦用户更新了配置值，那么应该通知所有使用了这项配置的代码，以便于进行一些具体的操作，比如重新打开文件，重新起监听端口等。

5. 校验方法，更新配置时会调用校验方法进行校验，以保证用户不会给配置项设置一个非法的值。



一个配置模块应具备的基本功能：

1. 支持定义/声明配置项，也就是在提供配置名称、类型以及可选的默认值的情况下生成一个可用的配置项。由于一项配置可能在多个源文件中使用，所以配置模块还应该支持跨文件声明配置项的方法。

2. 支持更新配置项的值。这点很好理解，配置项刚被定义时可能有一个初始默认值，但用户可能会有新的值来覆盖掉原来的值。

2. 支持从预置的途径中加载配置项，一般是配置文件，也可以是命令行参数，或是网络服务器。这里不仅应该支持基本数据类型的加载，也应该支持复杂数据类型的加载，比如直接从配置文件中加载一个map类型的配置项，或是直接从一个预定格式的配置文件中加载一个自定义结构体。

3. 支持给配置项注册配置变更通知。配置模块应该提供方法让程序知道某项配置被修改了，以便于进行一些操作。比如对于网络服务器而言，如果服务器端口配置变化了，那程序应该重新起监听端口。这个功能一般是通过注册回调函数来实现的，配置使用方预先给配置项注册一个配置变更回调函数，配置项发生变化时，触发对应的回调函数以通知调用方。由于一项配置可能在多个地方引用，所以配置变更回调函数应该是一个数组的形式。

4. 支持给配置项设置校验方法。配置项在定义时也可以指定一个校验方法，以保证该项配置不会被设置成一个非法的值，比如对于文件路径类的配置，可以通过校验方法来确保该路径一定存在。

5. 支持导出当前配置。

## gflags介绍
gflags是谷歌开源的一个基于命令行的C++配置库，项目地址：https://github.com/gflags/gflags，文档地址：https://gflags.github.io/gflags/。

gflags在配置定义和使用上与sylar的有几分类似，除了侧重于命令行这点外，其他功能差不多。并且，gflags也支持从配置文件中加载配置。sylar与gflags的另外一个不同点是，gflags支持配置参数校验，但sylar不支持。在学习sylar的配置模块之前，可以先通过gflags完整的文档和示例代码感受一下配置模块的使用。

## sylar配置模块设计
采用约定优于配置的思想，关于约定优于配置的描述可参考以下链接：

[维基百科：约定优于配置](https://zh.wikipedia.org/wiki/%E7%BA%A6%E5%AE%9A%E4%BC%98%E4%BA%8E%E9%85%8D%E7%BD%AE)

简单来说，约定优于配置的背景条件是，一般来说，程序所依赖的配置项都有一个公认的默认值，也就是所谓的约定。这点有可许多可以参考的例子，比如对于一个http网络服务器，服务端口通常都是80端口，对于配置文件夹路径，一般都是conf文件夹，对于数据库目录，一般都是db或data文件夹。对于这些具有公认约定的配置，就不需要麻烦程序员在程序跑起来后再一项一项地指定了，而是可以初始时就将配置项设置成对应的值。这样，程序员就可以只修改那些约定之外的配置项，然后以最小的代价让程序跑起来。

约定优于配置的方式可以减少程序员做决定的数量，获得简单的好处，同时兼顾灵活性。

在代码上，约定优于配置的思路体现为所有的配置项在定义时都带一个的默认值，以下是一个sylar配置项的示例，这是一个int类型的配置项，名称为tcp.connect.timeout，初始值为5000。
```CPP
static sylar::ConfigVar<int>::ptr g_tcp_connect_timeout = sylar::Config::Lookup("tcp.connect.timeout", 5000, "tcp connect timeout");
```
sylar的配置项定义之后即可使用，比如上面的配置项可以直接使用g_tcp_connect_timeout->getValue()获取参数的值，这里获取的为默认值5000。

sylar使用YAML做为配置文件，配置名称大小写不敏感，并且支持级别格式的数据类型，比如上面的配置项对应的YAML配置文件内容如下：
```CPP
tcp:
    connect:
        timeout: 10000
```
这里指定了配置名称为 tcp.connect.timeout的配置项的值为10000。由于配置文件指定的值与默认值不一样，当配置文件加载后，对应的配置项值会被自动更新为10000，如果配置项还注册了配置变更回调函数的话，会一并调用配置变更回调函数以通知配置使用方。

sylar支持STL容器（vector, list, set, map等等），支持自定义类型（需要实现序列化和反序列化方法)。

与配置模块相关的类：

ConfigVarBase: 配置项基类，虚基类，定义了配置项公有的成员和方法。sylar对每个配置项都包括名称和描述两项成员，以及toString/fromString两个纯虚函数方法。ConfigVarBase并不包含配置项类型和值，这些由继承类实现，由继承类实现的还包括具体类型的toString/fromString方法，用于和YAML字符串进行相互转换。

ConfigVar: 具体的配置参数类，继承自ConfigVarBase，并且是一个模板类，有3个模板参数。第一个模板参数是类型T，表示配置项的类型。另外两个模板参数是FromStr和ToStr，这两个参数是仿函数，FromStr用于将YAML字符串转类型T，ToStr用于将T转YAML字符串。这两个模板参数具有默认值LexicalCast<std::string, T>和LexicalCast<T, std::string>，根据不同的类型T，FromStr和ToStr具有不同的偏特化实现。

ConfigVar类在ConfigVarBase上基础上包含了一个T类型的成员和一个变更回调函数数组，此外，ConfigVar还提供了setValue/getValue方法用于获取/更新配置值（更新配置时会一并触发全部的配置变更回调函数），以及addListener/delListener方法用于添加或删除配置变更回调函数。

Config: ConfigVar的管理类，负责托管全部的ConfigVar对象，单例模式。提供Lookup方法，用于根据配置名称查询配置项。如果调用Lookup查询时同时提供了默认值和配置项的描述信息，那么在未找到对应的配置时，会自动创建一个对应的配置项，这样就保证了配置模块定义即可用的特性。除此外，Config类还提供了LoadFromYaml和LoadFromConfDir两个方法，用于从YAML对象或从命令行-c选项指定的配置文件路径中加载配置。Config的全部成员变量和方法都是static类型，保证了全局只有一个实例。

## sylar配置模块实现
挑几个重点讲讲。

sylar的配置模块使用了yaml-cpp作为YAML解析库，关于yaml-cpp的编译和使用可参考GitHub链接：[jbeder/yaml-cpp: A YAML parser and emitter in C++](https://github.com/jbeder/yaml-cpp)，关于YAML格式的介绍可参考YAML [入门教程 | 菜鸟教程](https://www.runoob.com/w3cnote/yaml-intro.html)。

sylar的配置模块实现的一大难点是类型转换类（仿函数）的偏特化实现。对于每种类型的配置，在对应的ConfigVar模板类实例化时都要提供其FromStr和ToStr两个仿函数，用于实现该类型和YAML字符串的相互转换。由于配置项的类型众多，包括全部的基本数据类型（int, float, double, string等），以及vector/list/set/unordered_set/map/unordered_map这几个复杂数据类型，还有用户自定义的类型。为了简化代码编写，sylar从一个基本类型的转换类开始，特化出了剩余类型的转换类，这个基本类型如下：
```CPP
/**
 * @brief 类型转换模板类(F 源类型, T 目标类型)
 */
template <class F, class T>
class LexicalCast {
public:
    /**
     * @brief 类型转换
     * @param[in] v 源类型值
     * @return 返回v转换后的目标类型
     * @exception 当类型不可转换时抛出异常
     */
    T operator()(const F &v) {
        return boost::lexical_cast<T>(v);
    }
};
```
这里的LexicalCast类是一个仿函数，它支持LexicalCast<F, T>()(const F &v)调用，可将传入的F类型的参数v进行转换，并返回T类型的结果。实际的转换语句是boost::lexical_cast<T>(v)。但是，受限于boost::lexical_cast, LexicalCast当前只能实现基本数据类型和std::string的相互转换，不能实现复杂类型的转换，下面的代码可用于演示当前LexicalCast的功能：
```CPP
std::string str1 = LexicalCast<int, std::string>()(123);     // ok, str1等于"123"
int int1 = LexicalCast<std::string, int>()("123");           // ok, int1等于123
std::string str2 = LexicalCast<float, std::string>()(3.14);  // ok，str2等于"3.14"
float float2 = LexicalCast<std::string, float>()("3.14");    // ok，float2等于3.14
 
vector<int> v = LexicalCast<std::string, vector<int>>()(...); // 错误，LexicalCast目前还不支持实例化T类型为vector<int>的模板参数
std::string s = LexicalCast<vector<int>, std::string>()(...); // 错误，同上
```
为了实现YAML字符串和vector/list/set/unordered_set/map/unordered_map的相互转换，就要对每个类型都进行特化，分别实现其转换类，下面是YAML字符串和vector的相互转换实现：
```CPP

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::vector<T>)
 */
template <class T>
class LexicalCast<std::string, std::vector<T>> {
public:
    std::vector<T> operator()(const std::string &v) {
        YAML::Node node = YAML::Load(v);
        typename std::vector<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};
 
/**
 * @brief 类型转换模板类片特化(std::vector<T> 转换成 YAML String)
 */
template <class T>
class LexicalCast<std::vector<T>, std::string> {
public:
    std::string operator()(const std::vector<T> &v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for (auto &i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};
```
上面分别实现了LexicalCast<std::string, std::vector<T>>和LexicalCast<std::vector<T>, std::string>，其中在转换单个的数组元素时，再次用到了LexicalCast<std::string, T>和LexicalCast<T, std::string>，如果这里T是基本数据类型，那么就可以用最开始的基本类型的转换类进行模板实例化并完成转换了，下面是针对vector和YAML字符串相互转换的示例：

```CPP

std::vector<int> v = LexicalCast<std::string, std::vector<int>>()("[1, 2, 3]");  // ok, v等于[1, 2, 3]
std::string s = LexicalCast<std::vector<int>, std::string>()(std::vector<int>{1, 2, 3}); // ok，s等于YAML格式的数组[1,2,3],如下:
                                                                                         // - 1
                                                                                         // - 2
                                                                                         // - 3
```
另外，由于这里的模板实例化是可以嵌套的，由vector和vector组合出来的全部类型都可以顺利地实现和YAML的转化，以下是一个二维数组的示例：
```CPP
std::vector<std::vector<int>> vv = LexicalCast<std::string, std::vector<std::vector<int>>>()("[[1,2,3],[4,5,6]]");
std::string ss = LexicalCast<std::vector<std::vector<int>>, std::string>()(vv);
```
上面的代码运行之后，vv将具有二维数组{{1,2,3},{4,5,6}}的值，而字符串ss则是与之对应YAML格式的二维数组，如下：
```CPP
-
  - 1
  - 2
  - 3
-
  - 4
  - 5
  - 6
```
其他复杂类型的偏特化与vector类型，参考源码理解即可。

每实现一个新类型的转换，那这个类型和之前已实现的类型组合出的数据类型也可以顺利实现转换，比如vector<set>, set<vector>，set<map>, map<set>, map<map>这种。这种基于偏特化实现类型转换的方法可以说是非常巧妙了，代码可以做到高度简化，但功能却非常强大，这也变相展示了泛型程序设计的强大之处吧。

## 待补充和完善
整合配置文件与命令行参数，配置项可以用命令行选项进行覆盖，并且在导出时进行标注。

配置项支持校验，参考gflags。