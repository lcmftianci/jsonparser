#! /bin/sh

echo "make .o"
make

echo "make .so"
g++ -g -shared -o libjsonparser.so filenameio.o VideoInfoJsonManager.o base64.o

echo "copy"
cp libjsonparser.so /home/VideoAnalysis/Bin/


echo "delete img"
rm -r /home/image/*

cd /home/VideoAnalysis/Bin/

./test

