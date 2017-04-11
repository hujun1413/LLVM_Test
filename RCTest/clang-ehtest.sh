#!/bin/bash	 
#PATH_GCC="echo gcc"
 	 	 
PATH_GCC=gcc
#PATH_CLANG="echo clang"
PATH_CLANG=clang
#PATH_LLDIS="echo llvm-dis"
PATH_LLDIS=llvm-dis
#PATH_EHTEST="echo /tmp/EHTest"
PATH_EHTEST=/home/disk1/llvm-3.2/projects/RCTest/Release+Asserts/bin/RCTest
#PATH_MKDIR="echo mkdir"
PATH_MKDIR=mkdir
#PATH_COPY="echo cp"
PATH_COPY=cp

PATH_ORITARGET="/tmp/ori_target.txt"
PATH_FUNC="/tmp/func.txt"
PATH_PAIR="/tmp/PairFile"
PATH_ENTRY="/tmp/entry_exit.txt"
PATH_LLTMP="/tmp/mytest"

TARGET_O=""
TARGET_N=""
TARGET_L=""

# get -r or -f option
OPT_RF=$1
shift
if [[ "$OPT_RF" != "-r" && "$OPT_RF" != "-f" && "$OPT_RF" != "-o" ]]; then
	echo "Use -r or -f or -o option"
	exit 1
fi
	 
# find -o option
TARGET_FLAG=0
for opt in $* ; do
	if [ $TARGET_FLAG == 1 ]; then
		TARGET_O=$opt
	fi
	if [ "$opt" == "-o" ]; then
		TARGET_FLAG=1
	else
		TARGET_FLAG=0
	fi
done
	 
if [ "${TARGET_O%%*.o}" == "" ]; then
	if [ "${TARGET_O%%*.mod.o}" != "" ]; then
		TARGET_N="${TARGET_O%.*}"
		TARGET_L="${TARGET_N##*/}"
	fi
fi
	  	 	 
# should not handle case
if [ "$TARGET_N" == "" ]; then
    $PATH_GCC $*
    exit
fi
 
# clang .c to .ll
MKPARAM="-S -emit-llvm -std=gnu89 -O0 -g"
for opt in $*; do
    if [ "$opt" == "-O2" ]; then
        continue
    fi
    if [ "$opt" == "-g" ]; then
        continue;
    fi
    if [ "$opt" == $TARGET_O ]; then
        MKPARAM+=" $TARGET_N.ll"
    else
        MKPARAM+=" $opt"
    fi
done
 
#echo ${MKPARAM}
$PATH_CLANG ${MKPARAM}
 
# mkdir ehtest
$PATH_MKDIR -p "${PATH_LLTMP}"
 
# cp ?.ll ehtest/?.ll
$PATH_COPY "${TARGET_N}.ll" "${PATH_LLTMP}/"
 
# EHTest -r ?.ll | EHTest -f ?.ll FUNC PAIR ENTRY
if [ "$OPT_RF" == "-r" ]; then
    $PATH_EHTEST -r "${TARGET_N}.ll" "${PATH_ORITARGET}"
elif [ "$OPT_RF" == "-f" ]; then
    $PATH_EHTEST -f "${TARGET_N}.ll" "${PATH_FUNC}" "${PATH_PAIR}" "${PATH_ENTRY}"
#elif [ "$OPT_RF" == "-o" ]; then
fi
 
# llvm-dis ?.ll -o ?.ll
if [ "$OPT_RF" != "-o" ]; then
    $PATH_LLDIS "${TARGET_N}.ll" -o "${TARGET_N}.ll"
fi
 
# clang .ll to .s
$PATH_CLANG -O2 -mno-sse -mno-mmx -mcmodel=kernel -S "${TARGET_N}.ll" -o "${TARGET_N}.s"
 
# gcc .s to .o
$PATH_GCC -mcmodel=kernel -c "${TARGET_N}.s" -o "${TARGET_O}"
