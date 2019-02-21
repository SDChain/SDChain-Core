**Ubuntu上编译安装SDChaind-Core**** 流程**

**1、下载源码**

从github上sdchain项目库下载sdchain-core源码

**2、编译源码**

**(1) 安装依赖包**

在sdchaind/Builds/Ubuntu目录下，输入命令：

chmod +x install\_boost.sh_

chmod +x install\_sdchaind\_depends\_ubuntu.sh_

  在sdchaind目录下，输入命令：

 sudo Builds/Ubuntu/install\_sdchaind\_depends\_ubuntu.sh_

 sudo Builds/Ubuntu/install\_boost.sh_

**(2) 更改系统配置文件**

/etc/profile文件中，添加以下两行：

 BOOST\_ROOT=/path/to/sdchaind/boost\_1\_63\_0

 export BOOST\_ROOT

更新配置，输入命令：

source /etc/profile

**(3) 编译**

在sdchaind目录下，输入命令:scons

  等待10分钟左右，当显示&quot;scons:done building targets&quot;时，编译完成。

**(4) 创建配置文件**

在/etc/opt目录下创建sdchain文件夹。复制配置文件

cp /sdchaind/doc/sdchaind-example.cfg /etc/opt/sdchain/sdchaind.cfg

 cp /sdchaind/doc/validators-example.txt /etc/opt/sdchain/validators.txt

**(5) 优化系统配置**

vim /etc/security/limits.conf

             在文件尾部增加下面两行

     \*     soft     nofile     65535

     \*     hard     nofile    65535

**(5) 测试查看是否编译完成**

 在sdchaind目录下，输入命令: ./build/sdchaind

**3、运行sdchaind**

在sdchaind目录下，输入命令: ./build/sdchaind -a --start
