**Ubuntu上编译安装SDChaind-Core**** 流程**

**1、下载源码**

从github上sdchain项目库下载sdchain-core源码

**2、编译源码**

**(1) 安装依赖包**

在sdchaind/Builds/Ubuntu目录下，输入命令：

_ chmod +x install\_boost.sh_

_         chmod +x install\_sdchaind\_depends\_ubuntu.sh_

  在sdchaind目录下，输入命令：

_ sudo Builds/Ubuntu/install\_sdchaind\_depends\_ubuntu.sh_

_ sudo Builds/Ubuntu/install\_boost.sh_

**(2) 更改系统配置文件**

/etc/profile文件中，添加以下两行：

_ BOOST\_ROOT=/path/to/sdchaind/boost\_1\_63\_0_

_ export BOOST\_ROOT_

更新配置，输入命令：

_source /etc/profile_

**(3) 编译**

在sdchaind目录下，输入命令:_scons_

  等待10分钟左右，当显示&quot;scons:done building targets&quot;时，编译完成。

**(4) 创建配置文件**

在/etc/opt目录下创建sdchain文件夹。复制配置文件

_cp /sdchaind/doc/sdchaind-example.cfg /etc/opt/sdchain/sdchaind.cfg_

_ cp /sdchaind/doc/validators-example.txt /etc/opt/sdchain/validators.txt_

**(5) 优化系统配置**

_vim /etc/security/limits.conf_

             在文件尾部增加下面两行

_     \*     soft     nofile     65535_

_     \*     hard     nofile    65535_

**(5) 测试查看是否编译完成**

 在sdchaind目录下，输入命令: _./build/sdchaind_

**3、运行sdchaind**

在sdchaind目录下，输入命令: _./build/sdchaind -a --start_