CC = g++ -std=c++11 -g -fPIC -shared
IFLAG = -I/usr/local/include/ -I.
LFLAG = -L/usr/local/lib
LFLAG += -ljsoncpp -lpthread
CPP = VideoInfoJsonManager.cpp filenameio.cpp


all:
	$(CC) $(IFLAG) $(CPP) $(LFLAG) -o libjsonparser.so

clean:
	rm -rf *.o libjsonparser.so

