#!/bin/bash

host="localhost 4242"
user=usr1
range=12

netcat  -i 1 $host <<x23LimitStringx23
/cmd;:;123;:;800;:;usr1;:;nc
/cmd;:;123;:;800;:;usr1;:;dcp
/cmd;:;123;:;800;:;usr1;:;c
/cmd;:;123;:;800;:;usr1;:;cc
/cmd;:;123;:;800;:;usr2;:;nc
/cmd;:;123;:;800;:;usr2;:;dcp
/cmd;:;123;:;800;:;usr2;:;c
/cmd;:;123;:;800;:;usr2;:;cc
x23LimitStringx23
#done
exit 0
