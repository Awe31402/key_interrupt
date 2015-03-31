#!/bin/bash

PRE_LINECOUNT=`dmesg | grep -i "Scan\|irq_handler" | wc -l`
NEXT_LINECOUNT=$PRE_LINECOUNT

while [ 1 ]
do
	NEXT_LINECOUNT=`dmesg | grep -i "Scan\|irq_handler" | wc -l`
	dmesg | grep -i "Scan\|irq_handler" | tail -n 10
	sleep 1
	PRE_LINECOUNT=$NEXT_LINECOUNT
done
