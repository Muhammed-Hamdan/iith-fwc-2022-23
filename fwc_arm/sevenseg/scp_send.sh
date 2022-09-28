#!/bin/bash
ip="192.168.43.91"
uname="mh"
path="/home/$uname/flash_vaman"
scp $1/GCC_Project/output/bin/$1.bin $uname@$ip:$path
