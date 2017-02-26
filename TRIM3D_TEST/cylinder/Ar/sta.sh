#!/bin/bash

for f in *eV.txt;do
num=`cat $f | awk '{if($7==4){print $0}}' | wc -l`
E=${f%eV.txt}
echo $E	$num
done
