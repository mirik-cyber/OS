#!/usr/bin/env bash

# edit the code below and add your code
# отредактируйте код ниже и добавьте свой

# Переменная с номером варианта (константа):
TASKID=19

# Дополнительные переменные (должны вычисляться динамически):
VAR_1=$(wc -l < dns-tunneling.log)

awk -F '\t' '
{
	ip = $5;
	port = $6;
	#if (port > 1900 && port < 2000 && port % 2 == 0){
	#	#print ip " " port;
	#	head;
		key = ip "\t" port;
		count[key]++;
	#}
}
END {
	for (d in count) {
		#if(count[d] == 498){
			print d "\t" count[d];
		#}
	}
}
' dns-tunneling.log | cat
# sort -t$'\t' -k3,3nr -k1,1 -k2,2n > results.txt

VAR_2=$(tail -n 1 results.txt | awk -F'\t' ' {print $3}')
