#!/bin/sh
dbPathLocal="./db"
dbPathEtc="/etc/opt/sdchain/db"
now=`date "+%Y%m%d%H%M%S"`
log_file="log_"$now".log"

#if [! -d "$dbPathLocal"] &&[ ! -d "$dbPathEtc"]; then
if [ ! -d "$dbPathEtc" ] && [ ! -d "$dbPathLocal" ];then
        echo    "db dir not exist"
        echo $log_file
        mkdir log
        nohup ./sdchaind >./log/$log_file &
else
        echo    "db dir exist"
        echo $log_file
        nohup ./sdchaind --load >./log/$log_file &
fi