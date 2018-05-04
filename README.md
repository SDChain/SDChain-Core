# Official C++ implementation of the Six Domain Chain


***How to compile SDChain-Core in Ubuntu***

**1、Download source code**

Download SDChain-Core src from https://github.com/SDChain/SDChain-Core.git

**2、Compile**

**(1) Install depends**

    cd sdchaind/Builds/Ubuntu/

    chmod +x install\_boost.sh

    chmod +x install\_sdchaind\_depends\_ubuntu.sh

    cd sdchaind

    sudo Builds/Ubuntu/install\_sdchaind\_depends\_ubuntu.sh

    sudo Builds/Ubuntu/install\_boost.sh

**(2) Edit profile**

    vi /etc/profile
Add 
    BOOST\_ROOT=/path/to/sdchaind/boost\_1\_63\_0

    export BOOST\_ROOT
to the tail,
Save and then:

    source /etc/profile

**(3) compile**

    cd sdchain/ 
    scons

  this step may take 10 minutes，When show "scons:done building targets"，means compiled。

**(4) Add config files**

    mkdir /etc/opt/sdchain/

    cp sdchaind/doc/sdchaind-example.cfg /etc/opt/sdchain/sdchaind.cfg

    cp sdchaind/doc/validators-example.txt /etc/opt/sdchain/validators.txt

**(5) Optimize operation system**

    vim /etc/security/limits.conf

Add

    *     soft     nofile     65535

    *     hard     nofile    65535

to the tail,then save.


**3、Run sdchaind**

    cd sdchain/

    ./build/sdchaind -a --start
