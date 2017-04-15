#!/bin/bash

for f in *xyz;do
	echo -n ${f%%keV.xyz}" "
	cat $f | awk 'BEGIN{num=0}{if(NR>2 && $1=="C" && $4>=-20 && $4<=20)num++;}END{print num}' 
done
