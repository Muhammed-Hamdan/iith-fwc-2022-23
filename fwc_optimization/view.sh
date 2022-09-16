#!/bin/bash
ext="_assignment"
dir=$1$ext
echo $dir
cp $dir/$dir.pdf /sdcard/fwc_termux/
termux-open /sdcard/fwc_termux/$dir.pdf
