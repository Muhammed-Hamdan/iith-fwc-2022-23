#!/bin/bash
ip="192.168.43.91"
uname="mh"
path="/home/$uname/flash_vaman"
cd $1/GCC_Project
make
cd ../..
scp $1/GCC_Project/output/bin/$1.bin $uname@$ip:$path
