#!/bin/bash
 ql_symbiflow -compile -src ./$1 -d ql-eos-s3 -P PU64 -v $2.v -t $2 -p quickfeather.pcf -dump binary
