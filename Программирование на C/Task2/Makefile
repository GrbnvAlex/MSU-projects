#all: test
#all: clear dict_std.txt clean util.o wordsplit countd dict.txt sorted_dict.txt hashtable.o freqdict freqdict_run
all: clear data dict_std.txt clean util.o wordsplit countd dict.txt sorted_dict.txt hashtable.o freqdict freqdict_run

#Abstract targets PHONY
#.PHONY: test dict_std.txt dict.txt clear clean sorted_dict.txt
.PHONY: test dict_std.txt dict.txt clear clean data sorted_dict.txt

clear:
	clear
	
data:
	wget `cat books` -O data

#To run test it is necessary to run <make test> in command line from terminal
test: dict_std.txt dict.txt

#Standard utilites
#tr -cs "[:alnum:]" "[\n*]" => skip all num and alphabets symbols, other symbols -> '\n'
#tr "[:upper:]" '[:lower:]' => all upper symbols -> lower symbols
#uniq -c		    => deletes and counts repeating words
dict_std.txt: books data
	echo "1: Used time for frequence dictionary by standard utilites:"	
	time -p cat data | tr -cs "[:alnum:]" "[\n*]" | tr "[:upper:]" '[:lower:]' | sort | uniq -c >./dict_std.txt



#============================

clean:
	rm -f *.o
	
util.o: util.c util.h 
	gcc -c util.c -std=c99 -o util.o

wordsplit: wordsplit.c util.o
	gcc wordsplit.c util.o -std=c99 -o wordsplit

countd: countd.c util.o
	gcc countd.c util.o -std=c99 -o countd

dict.txt: books data
	echo "2: Used time for frequence dictionary by own utilites:"	
	time -p cat data | ./wordsplit -l | ./linesort -m | ./countd >./dict.txt 

#===============================

#sort -nr => sort in decreasing order
sorted_dict.txt:
	sort -nr <./dict_std.txt >./sorted_dict.txt
	echo "0: Sorted_dict.txt has been created"

#===============================

hashtable.o: hashtable.c hashtable.h
	gcc -c hashtable.c -std=c99 -o hashtable.o

freqdict: freqdict.c util.o hashtable.o
	gcc freqdict.c util.o hashtable.o -std=c99 -o freqdict

freqdict_run: 
	echo "3: Used time for frequence dictionary by hash table:"
	time -p cat data | ./freqdict >./dict_hash.txt





	
