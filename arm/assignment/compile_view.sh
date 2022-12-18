#!/bin/bash
dir=$1
echo $dir
cd docs/
texfot pdflatex main.tex
cd ..
cp docs/main.pdf /sdcard/fwc_termux/$1.pdf
termux-open /sdcard/fwc_termux/$1.pdf
