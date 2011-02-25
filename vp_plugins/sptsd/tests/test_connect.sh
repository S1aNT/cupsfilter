#!/bin/bash

host="localhost 4242"
user=usr1
range=12

netcat  -i 1 $host <<x23LimitStringx23
/cmd;:;123;:;400;:;usr1;:;nc
/cmd;:;123;:;400;:;usr1;:;dcp
/cmd;:;123;:;400;:;usr1;:;c
/cmd;:;123;:;400;:;usr1;:;cc
/cmd;:;123;:;400;:;usr2;:;nc
/cmd;:;123;:;400;:;usr2;:;dcp
/cmd;:;123;:;400;:;usr2;:;c
/cmd;:;123;:;400;:;usr2;:;cc
/cmd;:;123;:;100;:;sl9prt.qqqq;:;NC;:;usr2
/cmd;:;123;:;100;:;sl9prt.qqqq;:;DCP;:;usr2
/cmd;:;123;:;100;:;sl9prt.qqqq;:;C;:;usr2
/cmd;:;123;:;100;:;sl9prt.qqqq;:;CC;:;usr2
/cmd;:;123;:;100;:;sl9prt.qqqq;:;NC;:;usr3
/cmd;:;123;:;100;:;sl9prt.qqqq;:;DCP;:;usr3
/cmd;:;123;:;100;:;sl9prt.qqqq;:;C;:;usr3
/cmd;:;123;:;100;:;sl9prt.qqqq;:;CC;:;usr3
/cmd;:;123;:;600;:;usr1
/cmd;:;123;:;600;:;usr2
/cmd;:;123;:;600;:;usr3
/cmd;:;123;:;300;:;NC
/cmd;:;123;:;300;:;DCP
/cmd;:;123;:;300;:;C
/cmd;:;123;:;300;:;CC
x23LimitStringx23
#done
exit 0
