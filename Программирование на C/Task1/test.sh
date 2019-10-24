#!/bin/bash
script_name=$0
echo ""
echo "script_name = $script_name"
echo ""

wget `cat book_urls`

gcc linesort.c -o linesort
time cat *txt | head -n 100 | ./linesort -m

time cat *txt | head -n 100 | ./linesort -b

gcc linesort_tree.c -o linesort_tree
time cat *txt | head -n 100 | ./linesort_tree   # in 5 files ~ 45'000 lines


#1'000
#time cat *txt | head -n 1000 | ./linesort -m
#time cat *txt | head -n 1000 | ./linesort -b
#time cat *txt | head -n 1000 | ./linesort_tree

#10'000
#time cat *txt | head -n 10000 | ./linesort -m
#time cat *txt | head -n 10000 | ./linesort -b
#time cat *txt | head -n 10000 | ./linesort_tree

#100'000
#time cat *txt *txt *txt | head -n 100000 | ./linesort -m
#time cat *txt *txt *txt | head -n 100000 | ./linesort -b
#time cat *txt *txt *txt | head -n 100000 | ./linesort_tree

#1'000'000
#time cat *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt | head -n 1000000 | ./linesort -m
#time cat *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt | head -n 1000000 | ./linesort -b
#time cat *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt *txt | head -n 1000000 | ./linesort_tree

exit 0
