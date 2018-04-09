#!/bin/bash

make

echo =====test.dat====
./poker -m4 test.dat

echo =====test.dat debug mode====
./poker -m4 test.dat -d


echo =====test.dat hex====
make hexdump

cat test.dat |./hexdump |./poker -m4 -x

echo ====/dev/urandom 100k====
dd if=/dev/urandom bs=1024 count=100|./poker -m2
