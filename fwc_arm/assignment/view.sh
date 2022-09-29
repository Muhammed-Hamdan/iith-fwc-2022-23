#!/bin/bash
dir=$1
echo $dir
cp docs/main.pdf /sdcard/fwc_termux/$1.pdf
termux-open /sdcard/fwc_termux/$1.pdf
