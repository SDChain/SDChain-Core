### 1	环境要求
```
操作系统：Ubuntu 14.04_64、 Ubuntu 16.04_64
CPU：3GHz+ processor with 4 cores 
内存：2G+
硬盘：40G+
编译环境：gcc version 5.4.1
资源限制：
core file size          (blocks, -c) 0
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 31449
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) unlimited
open files                      (-n) 10240
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) 31449
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```

### 2	软件下载
https://github.com/SDChain/SDChain-Core/releases/download/0.4.1-c/sdchaind-v0.4.1-c.tar.gz
下载 sdchaind-v0.4.1-c.tar.gz 文件  
解压后包含如下文件：
```
libprotobuf.so.8
libprotobuf.so.8.0.0
libstdc++.so.6
sdchaind
```
### 3	安装部署
#####  (1)	部署可执行程序目录
``` 
mkdir /usr/local/sdchaind
mv sdchaind-v0.4.1-c.tar.gz /usr/local/sdchaind
tar -zxvf sdchaind-v0.4.1-c.tar.gz
```

##### (2)	部署日志文件目录
```
mkdir ./log/
```

##### (3)	部署依赖库文件目录
```
mv libprotobuf.so.8 /usr/lib/x86_64-linux-gnu/libprotobuf.so.8
mv libprotobuf.so.8.0.0 /usr/lib/x86_64-linux-gnu/libprotobuf.so.8.0.0
mv libstdc++.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6
```

### 4	脚本启动运行
脚本文件路径 https://github.com/SDChain/SDChain-Core/bin/start.sh 
```
chmod +x start.sh    //添加可执行权限给启动脚本
```

```
./start.sh    //执行启动脚本
```
```
./sdchaind stop     //关闭服务
```
```
./sdchaind  peers       //验证是否启动成功
//返回数据有其它SDChain-Core节点服务器的信息，说明已经成功连接上区块链公共服务网络。
```
