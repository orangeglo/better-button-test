#!/bin/sh -l

export CPATH="/usr/lib/gbdk/include"
alias gbdkcc='/usr/lib/gbdk/bin/lcc'

cd ./src

gbdkcc -Wm-yn"BETTERBUTTON" -Wm-yc -Wl-yt0x1B -Wl-ya1 -D_ram_func=0xD000 -o better-button-test.gb *.c
