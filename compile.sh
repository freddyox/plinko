#!/bin/bash

echo "Compiling..."
echo " "
cd src/

g++ -c main.cpp Plinko.cpp


echo "Linking..."
echo " "

if [ -e main.cpp~ ];
then
    rm main.cpp~
fi

mv *.o ../linkers
cd ../linkers

g++ main.o Plinko.o -o plinko -lsfml-graphics -lsfml-window -lsfml-system

mv plinko ../
cd ../

if [ -e compile.sh~ ] ;
then 
    rm compile.sh~ 
fi

cd include/
if [ -e *.hh~ ];
then
    rm *.hh~
fi

cd ..
echo "Executing..."
echo " "
./plinko

