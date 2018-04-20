#!/bin/bash

x="0.0"
for i in `seq 1 20`;
do
	res=$(/home/achavez/Documents/University/UCSPparalelos/Paralelos/a.out)
	x=`bc -l <<< "$res + $x"`
done
echo "time = `bc -l <<< "$x / 20.0"`"

