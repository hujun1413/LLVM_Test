#!/bin/bash
#Usage: ./run.sh targetName

if [ $# != 1 ]; then
	echo "Usage: $0 targetName"
	exit;
fi

targetName=$1

echo -e "===== start compile =====\n"
make clean; make
echo -e "===== complete compile =====\n"

echo -e "===== start instrument =====\n"
cd Release+Asserts/bin
./$targetName ../../hello.bc
echo -e "===== complete instrument =====\n"

llc update.bc -o update.s
gcc -c update.s -o update.o
gcc update.o ../../libmyprint.a -o update

echo -e "===== start run program =====\n"
./update
echo -e "===== complete run program =====\n"
