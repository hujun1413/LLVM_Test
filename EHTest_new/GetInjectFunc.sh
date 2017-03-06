#!/bin/bash
#Usage: ./GetInjectFunc.sh drivername

logfile="/tmp/LogFile"
driverfile=$1".ko"

# compile the source code
echo -e "\n### Generate MyMonitor.ko ... ###\n"
cd ../../Monitor/
make

echo -e "\n### Compile the driver code ... ###\n"
cd -
./CompileDriver.sh -r

# start runtime monitoring
echo -e "\n### Install MyMonitor.ko ... ###\n"
sudo cat /dev/null > $logfile
sudo insmod ../../Monitor/MyMonitor.ko run_mode=1

# install the driver
echo -e "\n### Install the driver ... ###\n"
sudo insmod $driverfile

echo -e "\n### Run workload ... ###\n"
##### add a workload #####
sleep 1
ping -c 1 166.111.70.1
#########################
sleep 0.5

# remove the driver
echo -e "\n### Remove the driver ... ###\n"
sudo rmmod $driverfile

# end runtime monitoring
echo -e "\n### Remove MyMonitor.ko ... ###\n"
sudo rmmod MyMonitor.ko 

# filter repeated functions in the logfile
echo -e "\n### Extract fault-injected functions ... ###\n"
../../NoRepeat/NoRepeat $logfile temp.txt

# select the functions whose return value is checked in the code
cat /dev/null > func.txt
for file in mytest/*
do
	../../EHTest -a $file temp.txt func.txt
done

# count the numbers of original functions and remaining functions
original_num=`cat temp.txt | wc -l`
inject_num=`cat func.txt | wc -l`
echo -e "\n### The number of original functions: $original_num ###"
echo -e "### The number of fault-injected functions: $inject_num ###"
rm temp.txt

echo -e "\n===== Fault-injected functions are generated in func.txt ====="
echo -e "===== Please have a check =====\n"
