#!/bin/bash

cd -- "$(dirname "$BASH_SOURCE")"

if [[ -d DEVLOGS ]] ; then
	touch ./DEVFUNC/num.txt
	rm ./DEVFUNC/num.txt
else
	mkdir DEVLOGS
	mkdir DEVLOGS/COPY
	mkdir DEVLOGS/TEMP
fi

find . -type d \! -name . -prune \! -type d -o -name '*.cpp' -type f | grep -q . || touch main.cpp

for f in ./*.cpp
do
	if [[ -e $f ]] ; then
		cp $f ./DEVLOGS/COPY/$f
	fi
done

for f in ./*.hpp
do
	if [[ -e $f ]] ; then
		cp $f ./DEVLOGS/COPY/$f
	fi
done

for f in ./*.h
do
	if [[ -e $f ]] ; then
		cp $f ./DEVLOGS/COPY/$f
	fi
done

vi *.cpp

g++ -std=c++0x *.cpp -o RUNyes -Wall 2> ./DEVLOGS/TEMP/errCopy.txt

if [[ -e RUNyes ]] ; then
	echo 'OUTPUT:'
	./RUNyes | tee ./DEVLOGS/TEMP/outCopy.txt
	cp RUNyes RUN
	rm RUNyes
else
	echo 'compiler error!'
fi

g++ -std=c++0x ./DEVFUNC/Format/*.cpp -o ./DEVLOGS/TEMP/tempFormat

cd DEVLOGS
./TEMP/tempFormat
cd ..

mv ./DEVLOGS/output.txt ./DEVFUNC/output.txt

open -n ./DEVFUNC/UI/UI.app --args -AppCommandLineArg