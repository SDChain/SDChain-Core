**SDChain-Core用到的第三方库**

**1 加密安全算法库**

**1.1 secp256k1 椭圆曲线加密法**

椭圆曲线密码是一种公钥密码算法，公钥密码算法最根本的原理是利用信息的不对称性：即掌握私钥的人在整个通信过程中掌握最多的信息，一切的运算对他而言都是没有秘密的。为了让别人能给自己发送加密的信息，私钥拥有者把信息的一部分公开披露，披露的信息记为公钥。

详细见： [https://en.bitcoin.it/wiki/Secp256k1](https://en.bitcoin.it/wiki/Secp256k1)，

实现及测试程序可参考:https://github.com/bitcoin-core/secp256k1

**1.2 ed25519-donna 数字签名算法**

是一个数字签名算法，签名和验证的性能都极高， 一个4核2.4GHz 的 Westmere cpu，每秒可以验证 71000 个签名，安全性极高，等价于RSA约3000-bit。签名过程不依赖随机数生成器，不依赖hash函数的防碰撞性，没有时间通道攻击的问题，并且签名很小，只有64字节，公钥也很小，只有32字节，

实现及测试程序可考:

[https://github.com/floodyberry/ed25519-donna](https://github.com/floodyberry/ed25519-donna)

**1.3 OpenSSL**

OpenSSL 是一个安全 [套接字](https://baike.baidu.com/item/%E5%A5%97%E6%8E%A5%E5%AD%97)层密码库，囊括主要的 [密码算法](https://baike.baidu.com/item/%E5%AF%86%E7%A0%81%E7%AE%97%E6%B3%95)、常用的 [密钥](https://baike.baidu.com/item/%E5%AF%86%E9%92%A5)和证书封装管理功能及 [SSL](https://baike.baidu.com/item/SSL)协议，并提供丰富的应用程序供测试或其它目的使用。

**2 压缩算法库**

**2.1 LZ4**

最快的压缩和解压速度，尽管压缩比一般，单核压缩速度能达到400m/s，单核解压速度能达到1G/s

项目： [http://code.google.com/p/lz4/](http://code.google.com/p/lz4/)

**2.2 Snappy**

是一个 C++ 的用来压缩和解压缩的开发包。其目标不是最大限度压缩或者兼容其他压缩格式，而是旨在提供高速压缩速度和合理的压缩率。Snappy 比 zlib 更快，但文件相对要大 20% 到 100%。在 64位模式的 Core i7 处理器上，可达每秒 250~500兆的压缩速度。Snappy 在 Google 内部被广泛的使用，从 BigTable 到 MapReduce 以及内部的 RPC 系统。[1]
snappy 是一个压缩库。它可以利用单颗Intel Corei7处理器内核处理至少每秒250MB~500MB的数据流。

snappy 的前身是Zippy。虽然只是一个数据压缩库，它却被Google用于许多内部项目程，其中就包括BigTable，MapReduce和RPC。Google宣称它在这个库本身及其算法做了数据处理速度上的优化，作为代价，并没有考虑输出大小以及和其他类似工具的兼容性问题。Snappy特地为64位x86处理器做了优化，在单个Intel Core i7处理器内核上能够达到至少每秒250MB的压缩速率和每秒500MB的解压速率。

如果允许损失一些压缩率的话，那么可以达到更高的压缩速度，虽然生成的压缩文件可能会比其他库的要大上20%至100%，但是，相比其他的压缩库，Snappy却能够在特定的压缩率下拥有惊人的压缩速度，&quot;压缩普通文本文件的速度是其他库的1.5-1.7倍，HTML能达到2-4倍，但是对于JPEG、PNG以及其他的已压缩的数据，压缩速度不会有明显改善&quot;。

Google极力赞扬Snappy的各种优点，Snappy从一开始就被&quot;设计为即便遇到损坏或者恶意的输入文件都不会崩溃&quot;，而且被Google在生产环境中用于压缩PB级的数据。其健壮性和稳定程度可见一斑。
Snappy也可以用于和其他压缩库-zlib、LZO、LZF、FastLZ和QuickLZ-做对比测试，前提是你在机器上安装了这些压缩库。Snappy是一个C++的库，你可以在产品中使用，不过也有一些其他语言的版本，例如Haskell、Java、Perl、Python和Ruby。

**3  数据库**

**3.1**  **RocksDB**

项目起源于Facebook的一个实验项目,该项目旨在开发一个与快速存储器（尤其是闪存）存储数据性能相当的数据库软件，以应对高负载服务。这是一个c++库,可用于存储键和值,可以是任意大小的字节流。它支持原子读和写。
RocksDB具有高度灵活的配置功能,可以通过配置使其运行在各种各样的生产环境,包括纯内存,Flash,硬盘或HDFS。它支持各种压缩算法，并提供了便捷的生产环境维护和调试工具。
RocksDB借鉴了开源项目LevelDB的重要代码和Apache HBase项目的重要思想。最初的代码来源于开源项目leveldb 1.5分叉。它借鉴了了Facebook的代码和思想。

**3.2**  **NuDB**

是专门针对SSD或同等高IOPS设备的随机读取性能进行了优化，可用在各个平台上，只能插入不能删除的KV数据库，NuDB最常见的应用程序是内容可寻址存储，其中数据的加密摘要被用作密钥。读取性能和内存使用情况与数据库的大小无关。

参考: https://github.com/vinniefalco/NuDB

**3.3**  **Sqlite**

是一款轻型的数据库，是遵守ACID的关系型数据库管理系统，它包含在一个相对小的C库中。它是D.RichardHipp建立的公有领域项目。它的设计目标是嵌入式的，而且目前已经在很多嵌入式产品中使用了它，它占用资源非常的低，在嵌入式设备中，可能只需要几百K的内存就够了。它能够支持Windows/Linux/Unix等等主流的操作系统，同时能够跟很多程序语言相结合，比如 Tcl、C#、PHP、Java等，还有ODBC接口，同样比起Mysql、PostgreSQL这两款开源的世界著名数据库管理系统来讲，它的处理速度比他们都快。

**3.4**  **soci**

是一个用C++封装的数据库访问库，目前通过&quot;前端(应用程序)/核心/后端(数据库适配)&quot;模式支持firebird,mysql,sqlite3,oracle,postgresql,odbc多种数据库;作为扩展，您也可以自己针对具体不同库或应用编写自定义后端代码。

参考soci。 [https://github.com/SOCI/soci](https://github.com/SOCI/soci)

**4 基础C++ 库**

**4.1 Boost**

Boost库是为C++语言标准库提供扩展的一些C++程序库的总称，由Boost社区组织开发、维护。Boost库可以与C++标准库完美共同工作，并且为其提供扩展功能。

**4.2 Protobuf**

Protobuf (Google Protocol Buffer)是一种轻便高效的结构化数据存储格式，平台无关、语言无关、可扩展，解析速度快，比对应的XML快约20-100倍,序列化数据非常简洁、紧凑，与XML相比，其序列化之后的数据量约为1/3到1/10,可用于通讯协议和数据存储等领域,
