#!/bin/bash

PRE_LINECOUNT=`dmesg | grep -i scan | wc -l`
NEXT_LINECOUNT=$PRE_LINECOUNT

while [ 1 ]
do
	NEXT_LINECOUNT=`dmesg | grep -i scan | wc -l`
	dmesg | grep -i scan | tail -n $[ PRE_LINECOUNT - NEXT_LINECOUNT ]
	PRE_LINECOUNT=$NEXT_LINECOUNT
done
