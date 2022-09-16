#!/bin/bash
ext="_assignment"
dir=$1$ext
echo $dir
python3 $dir/codes/main.py
cd $dir
texfot pdflatex $dir.tex
cd ..
cp $dir/$dir.pdf /sdcard/fwc_termux/
termux-open /sdcard/fwc_termux/$dir.pdf
