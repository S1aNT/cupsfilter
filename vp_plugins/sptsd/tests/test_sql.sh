#!/bin/bash

host="localhost 4242"
user=usr1
range=12

netcat -i 9 $host <<x23LimitStringx23
/sql;:;123;:;202;:;select * from reports;
x23LimitStringx23
#done
exit 0
