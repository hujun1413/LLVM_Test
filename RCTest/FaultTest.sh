#!/bin/bash
#Usage ./FaultTest.sh drivername

inc=1
count=1
driverfile=$1".ko"
sudo cp ../../PairChecker/PairFile /tmp/PairFile
sudo dmesg -c
myfile="/tmp/BugFile"
sudo cat /dev/null > $myfile
cat func.txt | while read LINE
do
	arr=( $LINE )
	func=${arr[0]}
    caller=${arr[1]}
	sub=`expr index $func "*"`
	if [ $sub -ne 0 ]; then
		continue
	fi
	echo $LINE > current.txt
	pairfile="/tmp/PairFile"
	./CompileDriver.sh -f $func $caller ../../PairChecker/PairFile
	echo -e "\n=== Fault-inject function is: $func in $caller ===\n"
	sudo echo "## Fault-injected function ##" >> $myfile
	sudo echo -e "$func in $caller" >> $myfile
	sudo echo "## Unhandled requiring functions: ##" >> $myfile
	sudo insmod ../../Monitor/MyMonitor.ko run_mode=2 bugfile=$myfile
	sudo insmod $driverfile
	echo -e "=== Run Workload ===\n"	
	##### add a workload #####
	sleep 1
	ping -c 1 166.111.70.1
	##########################
	sleep 0.5
	sudo rmmod $driverfile
	sudo rmmod MyMonitor.ko
	sudo dmesg -c
	sleep 2
	count=`expr $count + $inc`
	sudo echo -e "\n\n" >> $myfile
done
sudo cp $myfile PairBugFile
