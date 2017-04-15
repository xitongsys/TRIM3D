#!/bin/bash
sed -n '3,$p' 1keV.xyz | awk '{print $1,$4}' | go run sta.go > a.txt
