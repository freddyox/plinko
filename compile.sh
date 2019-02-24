#!/bin/bash

echo "Compiling..."
echo " "
cd src/

g++ -c main.cpp Plinko.cpp -pthread -m64 -std=gnu++11 -I/home/freddy/Documents/root/include -L/home/freddy/Documents/root/lib -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic


echo "Linking..."
echo " "

if [ -e main.cpp~ ];
then
    rm main.cpp~
fi

mv *.o ../linkers
cd ../linkers

g++ main.o Plinko.o -o plinko -lsfml-graphics -lsfml-window -lsfml-system -I/home/freddy/Documents/root/include -L/home/freddy/Documents/root/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic

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

