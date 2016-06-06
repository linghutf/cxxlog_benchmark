#!/bin/bash

fast_log="./log/*.log"
stt_log="./sync_log/*.log"

echo "FAST:"

if [[ -e $fast_log ]];then
    echo `wc -c $fast_log`
    rm $fast_log
fi

echo "STT:"

if [[ -e $stt_log ]];then
    echo `wc -c ${stt_log}`
    rm $stt_log
fi
