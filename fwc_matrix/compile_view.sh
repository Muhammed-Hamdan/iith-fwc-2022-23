#!/bin/bash
#ext="_assignment"
dir=$1
echo $dir
#python3 $dir/codes/$dir.py
cd $dir
texfot pdflatex $dir.tex
cd ..
cp $dir/$dir.pdf /sdcard/fwc_termux/
termux-open /sdcard/fwc_termux/$dir.pdf
