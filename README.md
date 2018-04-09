# Random Poker Test

随机数的扑克检测.

compile:
make

run:

dd if=/dev/urandom bs=1024 count=100 |hexdump -v -e '/ "%02x"'|./poker -m4 
